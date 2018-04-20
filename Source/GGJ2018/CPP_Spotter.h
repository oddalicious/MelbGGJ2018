// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/TriggerBox.h"
#include "CPP_CupCharacter.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SpotLightComponent.h"
#include "CPP_Spotter.generated.h"

UENUM(BlueprintType)
enum class TriggerLogicSplitEnum : uint8
{
	TriggersActiveAndNoticeable,
	TriggersActiveAndHidden,
	TriggersInactive,
};

UCLASS()
class GGJ2018_API ACPP_Spotter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Spotter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SubscribeTriggers();
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void CheckPlayerLoss();

	UFUNCTION(BlueprintPure)
	bool AnyTriggersActive() const
	{
		return activeTriggers > 0;
	}

	UFUNCTION(BlueprintPure)
	bool IsPlayerNoticeable() const
	{
		if (cupCharacter)
		{
			return cupCharacter->isRotating || cupCharacter->isMoving || cupCharacter->IsAnimating();
		}
		return false;
	}

	UFUNCTION(BlueprintCallable, Category = CustomNodes,  meta = (ExpandEnumAsExecs = Branches))
	void SpottingLogicSplit(TriggerLogicSplitEnum& Branches);
	UFUNCTION(BlueprintCallable)
	bool IsPlayerVisible();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USpotLightComponent* spotLight = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* skeletalMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* regularTarget = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* defaultRoot = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float timeToNoticePlayer = 0.7f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float timeSinceSpot = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float lossTime = 3.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float reduceSpeed = 2.0f;
	UPROPERTY(BlueprintReadWrite)
	float currentSpotTime = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<float> rays = {-1.f, -.5f, 0.f, .5f, 1.f};
	UPROPERTY(BlueprintReadWrite)
	bool canBeAnnoyed = false;
	UPROPERTY(BlueprintReadWrite)
	int activeTriggers = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundBase*> huhSounds;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundBase*> giveUpSounds;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ATriggerBox*> triggerZones;
	UPROPERTY(BlueprintReadWrite)
	ACPP_CupCharacter* cupCharacter = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor safeColor = FLinearColor::Green;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor dangerColor = FLinearColor::Red;
	UPROPERTY(BlueprintReadWrite)
	bool spotting = false;
};
