// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_MusicManager.h"


// Sets default values
ACPP_MusicManager::ACPP_MusicManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_MusicManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_MusicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_MusicManager::TriggerBeenSpotted()
{

}

