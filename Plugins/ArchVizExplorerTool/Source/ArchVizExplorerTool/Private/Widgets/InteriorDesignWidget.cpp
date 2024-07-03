// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorDesignWidget.h"

void UInteriorDesignWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (IsValid(ChairButton)) {
		ChairButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleChairButtonClick);
	}
	if (IsValid(TableButton)) {
		TableButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleTableButtonClick);
	}
	if (IsValid(BedButton)) {
		BedButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleBedButtonClick);
	}
	if (IsValid(CeilingLightButton)) {
		CeilingLightButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleCeilingLightButtonClick);
	}
	if (IsValid(LampButton)) {
		LampButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleLampButtonClick);
	}
	if (IsValid(FrameButton)) {
		FrameButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::HandleFrameButtonClick);
	}

	if (IsValid(InteriorScrollBox) && IsValid(ChairDataAsset)) {
		InteriorScrollBox->PopulateWidget(ChairDataAsset);
	}
}

void UInteriorDesignWidget::ShowScrollBox() {
	if (IsValid(InteriorScrollBox)) {
		InteriorScrollBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInteriorDesignWidget::HideScrollBox() {
	if (IsValid(InteriorScrollBox)) {
		InteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInteriorDesignWidget::HandleButtonClick(UInteriorDataAsset* DataAsset) {
	if (IsValid(InteriorScrollBox) && IsValid(DataAsset)) {
		InteriorScrollBox->PopulateWidget(DataAsset);
		ShowScrollBox();
		OnInteriorTypeChange.ExecuteIfBound();
	}
}

void UInteriorDesignWidget::HandleChairButtonClick() {
	HandleButtonClick(ChairDataAsset);
}

void UInteriorDesignWidget::HandleTableButtonClick() {
	HandleButtonClick(TableDataAsset);
}

void UInteriorDesignWidget::HandleBedButtonClick() {
	HandleButtonClick(BedDataAsset);
}

void UInteriorDesignWidget::HandleCeilingLightButtonClick() {
	HandleButtonClick(CeilingLightDataAsset);
}

void UInteriorDesignWidget::HandleLampButtonClick() {
	HandleButtonClick(LampDataAsset);
}

void UInteriorDesignWidget::HandleFrameButtonClick() {
	HandleButtonClick(FrameDataAsset);
}

