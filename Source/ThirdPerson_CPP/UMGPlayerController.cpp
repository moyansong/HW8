// Fill out your copyright notice in the Description page of Project Settings.

#include "UMGPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ThirdPerson_CPP.h"

void AUMGPlayerController::BeginPlay()
{
    Super::BeginPlay();
    //�������ͬʱ���õ�UI����Ϸ��
    //SetInputMode(FInputModeGameAndUI());
}

void AUMGPlayerController::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 ZOrder)
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