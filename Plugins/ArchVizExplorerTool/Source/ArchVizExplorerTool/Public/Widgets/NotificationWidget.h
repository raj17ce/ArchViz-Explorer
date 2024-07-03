// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "NotificationWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UNotificationWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NotificationMessage;

	void ShowNotificationMessage();
	void HideNotificationMessage();

	void ShowErrorMessage(FText Message, float Time);
	void ShowSuccessMessage(FText Message, float Time);

	void DisplayNotificationMessageTimer(float Time);

	FTimerHandle TimerHandle;
};
