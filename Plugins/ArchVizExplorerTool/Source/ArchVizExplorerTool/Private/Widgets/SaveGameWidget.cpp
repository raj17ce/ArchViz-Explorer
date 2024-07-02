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
					ItemWidget->OnSlotSelected.BindLambda(
						[this](const FString& SlotName) {
							OnSaveSlotReceived.ExecuteIfBound(SlotName);
						});

					ItemWidget->OnSlotDeleted.BindLambda(
						[this](const FString& SlotName) {
							OnSaveSlotDeleteReceived.ExecuteIfBound(SlotName);
						});

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
	if (IsValid(NewProjectButton)) {
		NewProjectButton->OnClicked.AddDynamic(this, &USaveGameWidget::HandleNewProjectButtonClick);
	}
	if (IsValid(LoadProjectButton)) {
		LoadProjectButton->OnClicked.AddDynamic(this, &USaveGameWidget::HandleLoadProjectButtonClick);
	}
	if (IsValid(SaveProjectButton)) {
		SaveProjectButton->OnClicked.AddDynamic(this, &USaveGameWidget::HandleSaveProjectButtonClick);
	}

	if (IsValid(SavePopupCloseButton)) {
		SavePopupCloseButton->OnClicked.AddDynamic(this, &USaveGameWidget::HandleSavePopupCloseButtonClick);
	}
	if (IsValid(LoadPopupCloseButton)) {
		LoadPopupCloseButton->OnClicked.AddDynamic(this, &USaveGameWidget::HandleLoadPopupCloseButtonClick);
	}
}

void USaveGameWidget::HandleNewProjectButtonClick() {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "New Button Clicked");
}

void USaveGameWidget::HandleSaveProjectButtonClick() {
	if (IsValid(SavePopupBorderBox)) {
		SavePopupBorderBox->SetVisibility(ESlateVisibility::Visible);
		if(IsValid(BackgroundBlur)) {
			BackgroundBlur->SetIsEnabled(true);
			BackgroundBlur->SetBlurStrength(2.0f);
		}
	}
}

void USaveGameWidget::HandleSavePopupCloseButtonClick() {
	HideAllPopup();
	if (IsValid(SavePopupBorderBox)) {
		SavePopupBorderBox->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(BackgroundBlur)) {
			BackgroundBlur->SetIsEnabled(false);
			BackgroundBlur->SetBlurStrength(0.0f);
		}
	}
}

void USaveGameWidget::HandleLoadProjectButtonClick() {
	if (IsValid(LoadPopupBorderBox)) {
		LoadPopupBorderBox->SetVisibility(ESlateVisibility::Visible);
		if (IsValid(BackgroundBlur)) {
			BackgroundBlur->SetIsEnabled(true);
			BackgroundBlur->SetBlurStrength(2.0f);
		}
	}
}

void USaveGameWidget::HandleLoadPopupCloseButtonClick() {
	HideAllPopup();
	if (IsValid(LoadPopupBorderBox)) {
		LoadPopupBorderBox->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(BackgroundBlur)) {
			BackgroundBlur->SetIsEnabled(false);
			BackgroundBlur->SetBlurStrength(0.0f);
		}
	}
}

void USaveGameWidget::HideAllPopup() {
	if (IsValid(SavePopupBorderBox)) {
		SavePopupBorderBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(LoadPopupBorderBox)) {
		LoadPopupBorderBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}
