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

	spotLight->AttachToComponent(skeletalMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("LeftEyeSocket"));

	eyeTarget = FVector(0.f, 0.f, 0.f);
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
	ScaleEyesToTarget();

	float spotPercent = CheckTime(DeltaTime);

	CheckPlayerLoss();

	spotLight->SetLightColor(UKismetMathLibrary::LinearColorLerp(safeColor, dangerColor, spotPercent));

	if (AnyTriggersActive())
	{
		timeSinceSpot += DeltaTime;
	}
}

void ACPP_Spotter::SubscribeTriggers()
{
	// Disabled since the blueprint doesn't recognize the newly-created C++ event subscriptions, apparently it's not uncommon: https://answers.unrealengine.com/questions/502064/onbeginoverlap-from-c-not-working.html
	//for (ATriggerBox* box : triggerZones)
	//{
	//	if (box)
	//	{
	//		box->OnActorBeginOverlap.AddUniqueDynamic(this,&ACPP_Spotter::ActorBeginOverlap);
	//		box->OnActorEndOverlap.AddUniqueDynamic(this,&ACPP_Spotter::ActorEndOverlap);
	//	}
	//}
}

void ACPP_Spotter::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACPP_CupCharacter* cup = dynamic_cast<ACPP_CupCharacter*>(OtherActor);
	if (cup)
	{
		cupCharacter = cup;	
		activeTriggers++;
	}
}

void ACPP_Spotter::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACPP_CupCharacter* cup = dynamic_cast<ACPP_CupCharacter*>(OtherActor);
	if (cup)
	{
		cupCharacter = cup;
		activeTriggers = FMath::Max(activeTriggers - 1, 0);
		if (activeTriggers == 0)
		{
			cupCharacter = nullptr;
		}	
	}
}

void ACPP_Spotter::CheckPlayerLoss()
{
	if (currentSpotTime >= lossTime)
	{
		if (cupCharacter)
		{
			//Player loses
			cupCharacter->ShowLoseWidget();
		}
	}
}

//A Holdover from Blueprints with multi-ended macros, didn't seem worth removing right now.
TriggerLogicSplitEnum ACPP_Spotter::SpottingLogicSplit()
{
	TriggerLogicSplitEnum path;
	if (AnyTriggersActive())
	{
		if (IsPlayerVisible() && IsPlayerNoticeable())
		{
			path = TriggerLogicSplitEnum::TriggersActiveAndNoticeable;
		}
		else
		{
			path = TriggerLogicSplitEnum::TriggersActiveAndHidden;
		}
	}
	else
	{
		path = TriggerLogicSplitEnum::TriggersInactive;
	}
	return path;
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

void ACPP_Spotter::ScaleEyesToTarget()
{
	FVector targetLocation = FVector::ZeroVector;
	if (AnyTriggersActive() && currentSpotTime > 1.f)
	{
		targetLocation = cupCharacter->GetActorLocation();
	}
	else
	{
		targetLocation = GetActorTransform().InverseTransformPosition(eyeTarget);
	}
	FRotator towardsPlayer  = UKismetMathLibrary::FindLookAtRotation( spotLight->GetComponentLocation(), targetLocation);
	spotLight->SetWorldRotation(towardsPlayer);
}

void ACPP_Spotter::SpotPlayer()
{
	TArray<AActor*> managers;
	ACPP_MusicManager* musicManager;

	// Get the MusicManager and tell it the spotter has noticed the player
	UGameplayStatics::GetAllActorsOfClass(this,ACPP_MusicManager::StaticClass(),managers);
	if (managers.Num() > 0){

		musicManager = dynamic_cast<ACPP_MusicManager*>(managers[0]);
		if (musicManager != nullptr)
		{
			musicManager->TriggerBeenSpotted();
		}
	}
	if (huhSounds.Num() > 0)
	{
		int index = UKismetMathLibrary::RandomInteger(huhSounds.Num());
		UGameplayStatics::PlaySoundAtLocation(this, huhSounds[index], GetActorLocation());
	}

	UCPP_SpotterAnimInstance* spotterAnim = dynamic_cast<UCPP_SpotterAnimInstance*>(skeletalMesh->GetAnimInstance());
	spotterAnim->isAlert = true;

	timeSinceSpot = 0.f;
}

void ACPP_Spotter::GiveUp()
{
	TArray<AActor*> managers;
	ACPP_MusicManager* musicManager;

	if (canBeAnnoyed && giveUpSounds.Num() > 0)
	{
		int index = UKismetMathLibrary::RandomInteger(giveUpSounds.Num());
		UGameplayStatics::PlaySoundAtLocation(this,giveUpSounds[index],GetActorLocation());
	}
	// Get the MusicManager and tell it the spotter has lost the player
	UGameplayStatics::GetAllActorsOfClass(this, ACPP_MusicManager::StaticClass(), managers);
	if (managers.Num() > 0) {

		musicManager = dynamic_cast<ACPP_MusicManager*>(managers[0]);
		if (musicManager != nullptr)
		{
			musicManager->LoseSpot();
		}
	}
	UCPP_SpotterAnimInstance* spotterAnim = dynamic_cast<UCPP_SpotterAnimInstance*>(skeletalMesh->GetAnimInstance());
	spotterAnim->isAlert = false;
}

//Used partially for calculating loss and partially for a light that should illuminate the player when spotted, the return value is used to calculate the red value of the 'spot color'
float ACPP_Spotter::CheckTime(float DeltaTime)
{
	float currentSpotPercent = FMath::Clamp(currentSpotTime / lossTime,0.f,1.f);

	TriggerLogicSplitEnum currentState = SpottingLogicSplit();

	if (currentState == TriggerLogicSplitEnum::TriggersActiveAndNoticeable)
	{
		if (currentSpotTime < timeToNoticePlayer && currentSpotTime + DeltaTime >= timeToNoticePlayer)
		{
			SpotPlayer();
		}
		currentSpotTime += DeltaTime;
		canBeAnnoyed = currentSpotTime > canBeAnnoyedThreshold;
	}
	else
	{
		float max = FMath::Max(currentSpotTime - (DeltaTime / reduceSpeed),0.f);
		if (currentSpotTime > 0.f && max <= 0.f)
		{
			GiveUp();
		}
		currentSpotTime = max;
	}
	return FMath::Clamp(currentSpotPercent,0.f,1.f);
}