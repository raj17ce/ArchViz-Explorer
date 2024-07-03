// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MaterialItemWidget.h"

void UMaterialItemWidget::SetItemData(const FMaterialAssetData& NewItemData) {
	ItemData = NewItemData;

	if (IsValid(ItemData.Thumbnail)) {
		ItemImage->SetBrushResourceObject(ItemData.Thumbnail);
	}
}

void UMaterialItemWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (IsValid(ItemButton)) {
		ItemButton->OnClicked.AddDynamic(this, &UMaterialItemWidget::HandleItemButtonClick);
	}
}

void UMaterialItemWidget::HandleItemButtonClick() {
	OnItemButtonClicked.ExecuteIfBound(ItemData);
}
