// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_CoffeeFiller.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
ACPP_CoffeeFiller::ACPP_CoffeeFiller()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	standCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StandingCube"));
	RootComponent = standCube;

	boxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	boxTrigger->SetupAttachment(RootComponent);

	coffeeCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoffeeCylinder"));
	coffeeCylinder->SetupAttachment(RootComponent);

	beganFill = false;

	cylinderMaxDimensions = FVector(0.2f,0.2f,5.f);


}

// Called when the game starts or when spawned
void ACPP_CoffeeFiller::BeginPlay()
{
	Super::BeginPlay();
	standCube->SetActive(false);

	if (winPoint == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, FString("CoffeeFiller::WinPoint is Missing!"));
		}
	}
	else if (winPoint->GetRootComponent() == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, FString("CoffeeFiller::WinPoint's RootComponent is Missing!"));
		}
	}
	if (fillSound == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, FString("CoffeeFiller::FillSound is Missing!"));
		}
	}
	if (prepareSound == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, FString("CoffeeFiller::FillSound is Missing!"));
		}
	}
	if (lights.Num() == 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, FString("CoffeeFiller::Lights is Empty!"));
		}
	}
}


/*
* Tick is structured this way so that it allows the player to set down before properly 'activating', instead of locking them in place
	where they might be awkwardly positioned away from the point
*/
void ACPP_CoffeeFiller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!canFill || !CupReady())
	{
		return;
	}

	if (isFilling)
	{
		cupCharacter->Fill(DeltaTime,fillSound->Duration);
		PerformFill(DeltaTime);
	}
	else
	{
		if (!beganFill)
		{
			beganFill = true;
			BeginFill();
		}
		FillLights(DeltaTime);
	}

}

//Froth the coffee
void ACPP_CoffeeFiller::PerformStage1()
{
	preparing = true;
	if (cupCharacter)
		cupCharacter->canMove = false;
	if (prepareSound)
		UGameplayStatics::PlaySoundAtLocation(this,prepareSound, GetActorLocation());
}

//Setup pouring the coffee
void ACPP_CoffeeFiller::PerformStage2()
{
	preparing = false;
	if (fillSound)
		UGameplayStatics::PlaySound2D(this, fillSound);
	isFilling = true;

	GetWorldTimerManager().SetTimer(delayHandle, this, &ACPP_CoffeeFiller::FinishStages, fillSound->Duration);
}

//Pour the coffee
void ACPP_CoffeeFiller::PerformFill(float DeltaTime)
{
	fillTime += DeltaTime;
	FVector dimensions = cylinderMaxDimensions;
	float fillPercent = FMath::Sin(FMath::Clamp(fillTime / fillSound->Duration, 0.f, 1.f));
	
	dimensions.X = cylinderMaxDimensions.X * fillPercent;
	dimensions.Y = cylinderMaxDimensions.Y * fillPercent; 
	coffeeCylinder->SetWorldScale3D(dimensions);
}

// Set each light as 'done' as the coffee froths
void ACPP_CoffeeFiller::FillLights(float DeltaTime)
{
	if (!preparing)
		return;

	if (lights.Num() == 0)
		return;

	lightTime += DeltaTime;

	float percent = FMath::Clamp(lightTime / prepareSound->Duration, 0.f, 1.f);
	
	//Fit the percent to the number of lights
	percent *= (lights.Num() - 1);

	//Truncate that percent to an applicable light index
	int lightIndex = FMath::Clamp(UKismetMathLibrary::FTrunc(percent),0,lights.Num() -1);

	lights[lightIndex]->SetLightColor(doneColor);
}

bool ACPP_CoffeeFiller::CupReady() const
{
	bool cupCanFill;
	bool notMoving;
	bool withinRange;
	FVector cupLocation;
	FVector actorLocation;

	if (!cupCharacter)
	{
		return false;
	}

	cupCanFill = cupCharacter->fillAmount < 1.f;

	notMoving = !(cupCharacter->isMoving || cupCharacter->IsAnimating());

	actorLocation = GetActorLocation();
	actorLocation.Z = 0.f;
	cupLocation = cupCharacter->GetActorLocation();
	cupLocation.Z = 0.f;
	withinRange = UKismetMathLibrary::VSize(actorLocation - cupLocation) < tolerance;

	return (cupCanFill && notMoving && withinRange);
}

void ACPP_CoffeeFiller::BeginFill()
{
	PerformStage1();
	GetWorldTimerManager().SetTimer(delayHandle, this, &ACPP_CoffeeFiller::PerformStage2,prepareSound->Duration);
}

void ACPP_CoffeeFiller::FinishStages()
{
	isFilling = false;
	if (cupCharacter)
		cupCharacter->canMove = true;
	if (coffeeCylinder)
		coffeeCylinder->SetVisibility(false);
	if (winPoint && winPoint->GetRootComponent())
		winPoint->GetRootComponent()->SetActive(true);
	else if (!winPoint)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10,1.f, FColor::Red,FString("WinPoint is Missing!"));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, FString("WinPoint's RootComponent is Missing!"));
		}
	}
}