// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SaveGameWidget.h"
#include "UMG/Public/Components/ScrollBoxSlot.h"

void USaveGameWidget::PopulateSavedSlotsList(TArray<FString> SavedSlots) {
	if (IsValid(SlotsListScrollBox)) {
		SlotsListScrollBox->ClearChildren();

		for (const auto& SavedSlot : SavedSlots) {
			if (SaveGameItemWidgetClass) {
				if (auto* ItemWidget = CreateWidget<USaveGameItemWidget>(this, SaveGameItemWidgetClass)) {
					ItemWidget->SetSlotName(SavedSlot);
					ItemWidget->OnSlotSelected.BindUObject(this, &USaveGameWidget::HandleSlotSelected);
					ItemWidget->OnSlotDeleted.BindUObject(this, &USaveGameWidget::HandleSlotDeleted);

					auto* PanelSlot = SlotsListScrollBox->AddChild(ItemWidget);
					if (auto* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot)) {
						ScrollBoxSlot->SetPadding(FMargin{ 4.0 });
					}
				}
			}
		}
	}
}

void USaveGameWidget::NativeConstruct() {
	if (IsValid(LoadProjectButton)) {
		LoadProjectButton->OnClicked.AddDynamic(this, &USaveGameWidget::HandleLoadProjectButtonClick);
	}

	if (IsValid(SavePopupCloseButton)) {
		SavePopupCloseButton->OnClicked.AddDynamic(this, &USaveGameWidget::HandleSavePopupCloseButtonClick);
	}
	if (IsValid(LoadPopupCloseButton)) {
		LoadPopupCloseButton->OnClicked.AddDynamic(this, &USaveGameWidget::HandleLoadPopupCloseButtonClick);
	}
}

void USaveGameWidget::HandleSavePopupCloseButtonClick() {
	HideSavePopup();
}

void USaveGameWidget::HandleLoadProjectButtonClick() {
	ShowLoadPopup();
}

void USaveGameWidget::HandleLoadPopupCloseButtonClick() {
	HidelLoadPopup();
}

void USaveGameWidget::HideAllPopup() {
	if (IsValid(SavePopupBorderBox)) {
		SavePopupBorderBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(LoadPopupBorderBox)) {
		LoadPopupBorderBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USaveGameWidget::ShowSavePopup() {
	HideAllPopup();
	if (IsValid(SavePopupBorderBox)) {
		SavePopupBorderBox->SetVisibility(ESlateVisibility::Visible);
		if (IsValid(BackgroundBlur)) {
			BackgroundBlur->SetIsEnabled(true);
			BackgroundBlur->SetBlurStrength(2.0f);
		}
	}
}

void USaveGameWidget::HideSavePopup() {
	if (IsValid(SavePopupBorderBox)) {
		SavePopupBorderBox->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(BackgroundBlur)) {
			BackgroundBlur->SetIsEnabled(false);
			BackgroundBlur->SetBlurStrength(0.0f);
		}
	}
}

void USaveGameWidget::ShowLoadPopup() {
	HideAllPopup();
	if (IsValid(LoadPopupBorderBox)) {
		LoadPopupBorderBox->SetVisibility(ESlateVisibility::Visible);
		if (IsValid(BackgroundBlur)) {
			BackgroundBlur->SetIsEnabled(true);
			BackgroundBlur->SetBlurStrength(2.0f);
		}
	}
}

void USaveGameWidget::HidelLoadPopup() {
	if (IsValid(LoadPopupBorderBox)) {
		LoadPopupBorderBox->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(BackgroundBlur)) {
			BackgroundBlur->SetIsEnabled(false);
			BackgroundBlur->SetBlurStrength(0.0f);
		}
	}
}

void USaveGameWidget::HandleSlotSelected(const FString& SlotName) {
	OnSaveSlotReceived.ExecuteIfBound(SlotName);
}

void USaveGameWidget::HandleSlotDeleted(const FString& SlotName) {
	OnSaveSlotDeleteReceived.ExecuteIfBound(SlotName);
}
