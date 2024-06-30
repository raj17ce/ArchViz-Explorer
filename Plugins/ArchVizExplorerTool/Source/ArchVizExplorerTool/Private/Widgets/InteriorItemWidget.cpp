// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorItemWidget.h"

void UInteriorItemWidget::SetItemData(const FInteriorAssetData& NewItemData) {
	ItemData = NewItemData;

	if (IsValid(ItemData.Thumbnail)) {
		ItemImage->SetBrushResourceObject(ItemData.Thumbnail);
	}
}

void UInteriorItemWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (IsValid(ItemButton)) {
		ItemButton->OnClicked.AddDynamic(this, &UInteriorItemWidget::HandleItemButtonClick);
	}
}

void UInteriorItemWidget::HandleItemButtonClick() {
	OnItemButtonClicked.ExecuteIfBound(ItemData);
}	
