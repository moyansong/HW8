// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Array.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPerson_CPPGameMode.generated.h"

UCLASS(minimalapi)
class AThirdPerson_CPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThirdPerson_CPPGameMode();

private:
	virtual void StartPlay() override;

    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const override;

public:
    /** 移除当前菜单控件，并在指定类（如有）中新建控件。*/
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
        void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 ZOrder = 0);

protected:
    //virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

    /** 用作菜单的控件实例。*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        UUserWidget* CurrentWidget;
    UFUNCTION()
        void OnRep_CurrentWidget();
};



