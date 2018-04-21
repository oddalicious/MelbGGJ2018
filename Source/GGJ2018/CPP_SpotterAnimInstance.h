// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPP_SpotterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2018_API UCPP_SpotterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(BlueprintReadWrite)
	bool isAlert = false;
	
};
