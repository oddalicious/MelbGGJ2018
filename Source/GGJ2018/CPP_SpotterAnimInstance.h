// Fill out your copyright notice in the Description page of Project Settings.
/*
* @Author: Tim Patullock
* @Date: 21/3/2018
* Description: C++ base to allow the other C++ classes to affect my Animation blueprint.
*/
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
