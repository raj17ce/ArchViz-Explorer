// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/EditableText.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/BackgroundBlur.h"
#include "SaveGameItemWidget.h"
#include "UMG/Public/Components/EditableTextBox.h"
#include "SaveGameWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnSaveSlotReceived, const FString&)
DECLARE_DELEGATE_OneParam(FOnSaveSlotDeleteReceived, const FString&)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API USaveGameWidget : public UUserWidget {
	GENERATED_BODY()

public:
	FOnSaveSlotReceived OnSaveSlotReceived;
	FOnSaveSlotDeleteReceived OnSaveSlotDeleteReceived;

	void PopulateSavedSlotsList(TArray<FString> SavedSlots);

	virtual void NativeConstruct() override;

	// Save and Load Menu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* NewProjectButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* LoadProjectButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SaveProjectButton;

	//Save Popup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* SavePopupBorderBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SavePopupCloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SavePopupSaveButton;

	UFUNCTION()
	void HandleSavePopupCloseButtonClick();

	// Load Popup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* LoadPopupBorderBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* LoadPopupCloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* SlotsListScrollBox;

	UFUNCTION()
	void HandleLoadProjectButtonClick();

	UFUNCTION()
	void HandleLoadPopupCloseButtonClick();

	//BackGround Blur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBackgroundBlur* BackgroundBlur;

	void HideAllPopup();

	void ShowSavePopup();
	void HideSavePopup();
	void ShowLoadPopup();
	void HidelLoadPopup();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot Item")
	TSubclassOf<USaveGameItemWidget> SaveGameItemWidgetClass;
};
