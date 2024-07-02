// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NotificationWidget.h"

void UNotificationWidget::ShowNotificationMessage() {
	if (IsValid(NotificationMessage)) {
		NotificationMessage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UNotificationWidget::HideNotificationMessage() {
	if (IsValid(NotificationMessage)) {
		NotificationMessage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UNotificationWidget::ShowErrorMessage(FText Message) {
	if (IsValid(NotificationMessage)) {
		NotificationMessage->SetText(Message);
		NotificationMessage->SetColorAndOpacity(FSlateColor{ FLinearColor{} });
		DisplayNotificationMessageTimer(3.0f);
	}
}

void UNotificationWidget::ShowSuccessMessage(FText Message) {
	if (IsValid(NotificationMessage)) {
		NotificationMessage->SetText(Message);
		NotificationMessage->SetColorAndOpacity(FSlateColor{ FLinearColor{} });
		DisplayNotificationMessageTimer(3.0f);
	}
}

void UNotificationWidget::DisplayNotificationMessageTimer(float Time) {
	ShowNotificationMessage();

	if (GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle) != -1.f) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UNotificationWidget::HideNotificationMessage, Time, false);
}
