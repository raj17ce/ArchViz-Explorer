// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NotificationWidget.h"

void UNotificationWidget::ShowNotificationMessage() {
	if (IsValid(NotificationMessage)) {
		NotificationMessage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UNotificationWidget::HideNotificationMessage() {
	if (IsValid(NotificationMessage)) {
		NotificationMessage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UNotificationWidget::ShowErrorMessage(FText Message, float Time) {
	if (IsValid(NotificationMessage)) {
		NotificationMessage->SetText(Message);
		NotificationMessage->SetColorAndOpacity(FSlateColor{ FLinearColor{0.938686, 0.078187, 0.030713}});
		DisplayNotificationMessageTimer(Time);
	}
}

void UNotificationWidget::ShowSuccessMessage(FText Message, float Time) {
	if (IsValid(NotificationMessage)) {
		NotificationMessage->SetText(Message);
		NotificationMessage->SetColorAndOpacity(FSlateColor{ FLinearColor{0.016807, 0.558341, 0.084376}});
		DisplayNotificationMessageTimer(Time);
	}
}

void UNotificationWidget::DisplayNotificationMessageTimer(float Time) {
	ShowNotificationMessage();

	if (GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle) != -1.0f) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UNotificationWidget::HideNotificationMessage, Time, false);
}
