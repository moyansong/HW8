// Copyright Epic Games, Inc. All Rights Reserved.


#include "ThirdPerson_CPPGameMode.h"
#include "ThirdPerson_CPPCharacter.h"
#include "ThirdPerson_CPP.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

AThirdPerson_CPPGameMode::AThirdPerson_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	/*if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}

void AThirdPerson_CPPGameMode::StartPlay()
{
	Super::StartPlay();
	//ChangeMenuWidget(StartingWidgetClass);
	//check(GEngine != nullptr);

}

void AThirdPerson_CPPGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 ZOrder)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport(ZOrder);
        }
    }
}

void AThirdPerson_CPPGameMode::OnRep_CurrentWidget()
{
    
}

void AThirdPerson_CPPGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AThirdPerson_CPPGameMode, CurrentWidget);
}