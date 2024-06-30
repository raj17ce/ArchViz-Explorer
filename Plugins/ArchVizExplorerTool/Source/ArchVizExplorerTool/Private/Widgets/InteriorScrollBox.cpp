// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorScrollBox.h"
#include "UMG/Public/Components/ScrollBoxSlot.h"

void UInteriorScrollBox::PopulateWidget(UInteriorDataAsset* DataAsset) {
	if (IsValid(DataAsset) && IsValid(ScrollBox)) {
		ScrollBox->ClearChildren();

		TArray<FInteriorAssetData> Assets = DataAsset->InteriorAssets;

		for (const auto& Asset : Assets) {
			if (InteriorItemWidgetClass) {
				if (auto* ItemWidget = CreateWidget<UInteriorItemWidget>(this, InteriorItemWidgetClass)) {
					ItemWidget->SetItemData(Asset);
					ItemWidget->OnItemButtonClicked.BindUObject(this, &UInteriorScrollBox::HandleItemButtonClicked);
					auto* PanelSlot = ScrollBox->AddChild(ItemWidget);
					if (auto* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot)) {
						ScrollBoxSlot->SetPadding(FMargin{8.0});
					}
				}
			}
		}
	}	
}

void UInteriorScrollBox::HandleItemButtonClicked(FInteriorAssetData Asset) {
	OnItemSelected.ExecuteIfBound(Asset);
}

void UInteriorScrollBox::NativeConstruct() {
	Super::NativeConstruct();

	/*if (!InteriorItemWidgetClass) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InteriorItemWidgetClass is not set!"));
		}
	}
	else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("InteriorItemWidgetClass is set!"));
		}
	}*/
}
