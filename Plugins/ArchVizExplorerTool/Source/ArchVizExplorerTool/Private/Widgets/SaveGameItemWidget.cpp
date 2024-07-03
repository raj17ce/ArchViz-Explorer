// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SaveGameItemWidget.h"

void USaveGameItemWidget::SetSlotName(const FString& NewSlotName) {
	SlotName = NewSlotName;

	if (IsValid(SlotNameTextBlock)) {
		SlotNameTextBlock->SetText(FText::FromString(SlotName));
	}
}

const FString& USaveGameItemWidget::GetSlotName() const {
	return SlotName;
}

void USaveGameItemWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (IsValid(SlotNameButton)) {
		SlotNameButton->OnClicked.AddDynamic(this, &USaveGameItemWidget::HandleSlotNameButtonClick);
	}

	if (IsValid(SlotDeleteButton)) {
		SlotDeleteButton->OnClicked.AddDynamic(this, &USaveGameItemWidget::HandleSlotDeleteButtonClick);
	}
}

void USaveGameItemWidget::HandleSlotNameButtonClick() {
	OnSlotSelected.ExecuteIfBound(SlotName);
}

void USaveGameItemWidget::HandleSlotDeleteButtonClick() {
	OnSlotDeleted.ExecuteIfBound(SlotName);
}
