// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UMGPlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class THIRDPERSON_CPP_API AUMGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	/** 用作菜单的控件实例。*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UUserWidget* CurrentWidget;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 ZOrder = 0);
};
