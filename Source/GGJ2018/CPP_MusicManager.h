// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "CPP_MusicManager.generated.h"

UCLASS()
class GGJ2018_API ACPP_MusicManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_MusicManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float timeSinceLastScare = 0.f;
	int spotCount = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TriggerBeenSpotted();
	
	void LoseSpot();
	void SpotPlayer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* regularMusic;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* spottedMusic;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* spotSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAudioComponent* audio;
};
