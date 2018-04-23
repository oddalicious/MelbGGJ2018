// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "CPP_CupCharacter.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/Light.h"
#include "CPP_CoffeeFiller.generated.h"

UCLASS()
class GGJ2018_API ACPP_CoffeeFiller : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_CoffeeFiller();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PerformStage1();

	void PerformStage2();

	void FinishStages();

	UFUNCTION(BlueprintCallable)
	void PerformFill(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void FillLights(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* standCube = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* boxTrigger = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* coffeeCylinder = nullptr;

	UFUNCTION(BlueprintPure)
	bool CupReady() const;

	void BeginFill();

	FTimerHandle delayHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite)
	ACPP_CupCharacter* cupCharacter = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* winPoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* fillSound = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* prepareSound = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ALight*> lights;

	UPROPERTY(BlueprintReadOnly)
	FLinearColor doneColor = FColor::Green;
	

	

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector cylinderMaxDimensions;
	UPROPERTY(BlueprintReadWrite)
	bool canFill = true;
	UPROPERTY(BlueprintReadWrite)
	bool preparing = false;
	UPROPERTY(BlueprintReadWrite)
	bool isFilling = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float tolerance = 50.f;
	float fillTime = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float lightTime = 0.f;
	int stage = 0;
	bool beganFill = false;
};
