// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MaterialScrollBox.h"
#include "UMG/Public/Components/ScrollBoxSlot.h"

void UMaterialScrollBox::NativeConstruct() {
	Super::NativeConstruct();
}

void UMaterialScrollBox::PopulateWidget(UMaterialDataAsset* DataAsset) {
	if (IsValid(DataAsset) && IsValid(ScrollBox)) {
		ScrollBox->ClearChildren();

		TArray<FMaterialAssetData> Assets = DataAsset->MaterialAssets;

		for (const auto& Asset : Assets) {
			if (MaterialItemWidgetClass) {
				if (auto* ItemWidget = CreateWidget<UMaterialItemWidget>(this, MaterialItemWidgetClass)) {
					ItemWidget->SetItemData(Asset);
					ItemWidget->OnItemButtonClicked.BindUObject(this, &UMaterialScrollBox::HandleItemButtonClicked);
					auto* PanelSlot = ScrollBox->AddChild(ItemWidget);
					if (auto* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot)) {
						ScrollBoxSlot->SetPadding(FMargin{ 8.0 });
					}
				}
			}
		}
	}
}

void UMaterialScrollBox::HandleItemButtonClicked(FMaterialAssetData Asset) {
	OnItemSelected.ExecuteIfBound(Asset);
}
