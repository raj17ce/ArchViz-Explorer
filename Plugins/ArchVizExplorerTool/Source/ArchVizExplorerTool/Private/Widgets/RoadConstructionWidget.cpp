// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/RoadConstructionWidget.h"

void URoadConstructionWidget::ShowUndoButton() {
	if (IsValid(UndoRoadButton)) {
		UndoRoadButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void URoadConstructionWidget::HideUndoButton() {
	if (IsValid(UndoRoadButton)) {
		UndoRoadButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}	
