// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Spotter.h"
#include "Engine.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


// Sets default values
ACPP_Spotter::ACPP_Spotter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	defaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = defaultRoot;
	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	skeletalMesh->SetupAttachment(RootComponent);
	spotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	spotLight->SetupAttachment(skeletalMesh);
	regularTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RegularTarget"));
	regularTarget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPP_Spotter::BeginPlay()
{
	Super::BeginPlay();
	SubscribeTriggers();
}

// Called every frame
void ACPP_Spotter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Spotter::SubscribeTriggers()
{
	for (ATriggerBox* box : triggerZones)
	{
		if (box)
		{
			box->OnActorBeginOverlap.AddUniqueDynamic(this,&ACPP_Spotter::ActorBeginOverlap);
			box->OnActorEndOverlap.AddUniqueDynamic(this,&ACPP_Spotter::ActorEndOverlap);
		}
	}
}

void ACPP_Spotter::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACPP_CupCharacter* cup = dynamic_cast<ACPP_CupCharacter*>(OtherActor);
	if (cup)
	{
		cupCharacter = cup;	
		activeTriggers++;

		FString triggerText = "Trigger Connected: " + activeTriggers;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, triggerText);
		}
	}
}

void ACPP_Spotter::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACPP_CupCharacter* cup = dynamic_cast<ACPP_CupCharacter*>(OtherActor);
	if (!cup)
	{
		return;
	}

	cupCharacter = cup;
	activeTriggers = FMath::Max(activeTriggers - 1, 0);
	if (activeTriggers == 0)
	{
		cupCharacter = nullptr;
	}	
	FString triggerText = "Trigger Disconnected: " + activeTriggers;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0,1.f,FColor::Orange, triggerText);
	}
}

void ACPP_Spotter::CheckPlayerLoss()
{
	if (currentSpotTime >= lossTime)
	{
		if (cupCharacter != nullptr)
		{
			cupCharacter->ShowLoseWidget();
		}
	}
}

void ACPP_Spotter::SpottingLogicSplit(TriggerLogicSplitEnum& Branches)
{
	if (AnyTriggersActive())
	{
		if (IsPlayerVisible() && IsPlayerNoticeable())
		{
			Branches = TriggerLogicSplitEnum::TriggersActiveAndNoticeable;
		}
		else
		{
			Branches = TriggerLogicSplitEnum::TriggersActiveAndHidden;
		}
	}
	else
	{
		Branches = TriggerLogicSplitEnum::TriggersInactive;
	}


}

bool ACPP_Spotter::IsPlayerVisible()
{
	if (!AnyTriggersActive())
	{
		return false;
	}

	int hitRays = 0;
	FVector direction;
	FVector spotLocation = spotLight->K2_GetComponentLocation();
	FRotator towardsCup = UKismetMathLibrary::FindLookAtRotation( spotLocation,cupCharacter->GetActorLocation());
	float halfRadius = cupCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	FVector rightVector = UKismetMathLibrary::GetRightVector(towardsCup) * halfRadius;

	FHitResult outResult;

	for (int ray : rays)
	{
		direction = cupCharacter->GetActorLocation() + (rightVector * ray);
		
		if (GetWorld()->LineTraceSingleByChannel(outResult, spotLocation, direction,ECollisionChannel::ECC_Visibility))
		{
			if (outResult.Actor == cupCharacter)
			{
				DrawDebugLine(GetWorld(), spotLocation, direction, FColor::Red);
				hitRays++;
			}
			else
			{
				DrawDebugLine(GetWorld(), spotLocation, direction, FColor::Green);
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), spotLocation, direction, FColor::Green);
		}
	}
	

	return hitRays > 1;
}