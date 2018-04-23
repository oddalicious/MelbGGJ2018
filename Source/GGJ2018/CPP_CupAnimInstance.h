// Fill out your copyright notice in the Description page of Project Settings.
/*
* @Author: Tim Patullock
* @Date: 21/3/2018
* Description: This provides a C++ base for the other classes to interact with easily.
*/
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "CPP_CupAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2018_API UCPP_CupAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(BlueprintReadWrite)
	bool isAnimating;
	UPROPERTY(BlueprintReadWrite)
	bool canRun;
	UPROPERTY(BlueprintReadWrite)
	bool isInAir;
	UPROPERTY(BlueprintReadWrite)
	bool hasJumped;

	
};
