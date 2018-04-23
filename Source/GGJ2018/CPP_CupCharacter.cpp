// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_CupCharacter.h"
#include "Engine.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"


// Sets default values
ACPP_CupCharacter::ACPP_CupCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACPP_CupCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_CupCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_CupCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ACPP_CupCharacter::IsAnimating() const
{
	UCPP_CupAnimInstance* cupAnim = dynamic_cast<UCPP_CupAnimInstance*>(GetMesh()->GetAnimInstance());
	if (cupAnim)
	{
		return cupAnim->isAnimating;
	}
	return false;
}

void ACPP_CupCharacter::ShowLoseWidget()
{
	if (loseGameWidget)
	{
		loseGameWidget->SetVisibility(ESlateVisibility::Visible);
		loseGameWidget->AddToViewport();

		APlayerController * controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (controller != nullptr)
		{
			UWidgetBlueprintLibrary::SetInputMode_UIOnly(controller, loseGameWidget, false);
			controller->bShowMouseCursor = true;
			UGameplayStatics::SetGamePaused(this, true);
		}
		
		
	}
}

void ACPP_CupCharacter::Fill(float DeltaTime, float Length)
{
	fillAmount = FMath::Clamp(fillAmount + (DeltaTime / Length),0.f,1.f);
}

bool ACPP_CupCharacter::IsNoticeable() const
{
	return isMoving || IsAnimating() || isRotating;
}

void ACPP_CupCharacter::ShowWinWidget()
{
	winGameWidget->SetVisibility(ESlateVisibility::Visible);
	winGameWidget->AddToViewport();
	
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (controller != nullptr)
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnly(controller, winGameWidget, false);
		controller->bShowMouseCursor = true;
		UGameplayStatics::SetGamePaused(this, true);
	}
}