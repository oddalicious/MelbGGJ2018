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
		APlayerController * controller = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		if (controller != nullptr)
		{
			UWidgetBlueprintLibrary::SetInputMode_UIOnly(controller, loseGameWidget, false);
			controller->bShowMouseCursor = true;
		}
		
		UGameplayStatics::SetGamePaused(this, true);
	}
}

