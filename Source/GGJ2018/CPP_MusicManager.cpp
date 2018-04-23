// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_MusicManager.h"
#include "Engine.h"


// Sets default values
ACPP_MusicManager::ACPP_MusicManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	audio = CreateDefaultSubobject<UAudioComponent>("Audio");
	RootComponent = audio;
}

// Called when the game starts or when spawned
void ACPP_MusicManager::BeginPlay()
{
	Super::BeginPlay();
	LoseSpot();

	if (!spotSound)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(5,2.f,FColor::Red,FString("MusicManager::SpotSound is Missing"));
	}
	if (!spottedMusic)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Red, FString("MusicManager::SpottedMusic is Missing"));
	}
	if (!regularMusic)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Red, FString("MusicManager::RegularMusic is Missing"));
	}
}

// Called every frame
void ACPP_MusicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timeSinceLastScare = FMath::Max(timeSinceLastScare - DeltaTime, 0.f);
}

void ACPP_MusicManager::TriggerBeenSpotted()
{
	if (timeSinceLastScare == 0)
	{

		if (spotSound != nullptr)
			UGameplayStatics::PlaySound2D(this, spotSound);
		timeSinceLastScare = 1.5f;
	}
	SpotPlayer();
}

void ACPP_MusicManager::LoseSpot()
{
	spotCount = FMath::Max(spotCount - 1, 0);

	if (spotCount <= 0 && audio->Sound != regularMusic && regularMusic != nullptr)
	{
		audio->Stop();
		audio->Sound = regularMusic;
		audio->Play();
	}
}


void ACPP_MusicManager::SpotPlayer()
{
	if (spottedMusic != nullptr)
	{
		audio->Stop();
		audio->Sound = spottedMusic;
		audio->Play();
		spotCount++;
	}
}