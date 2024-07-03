// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/TextBlock.h"
#include "SaveGameItemWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnSlotSelected, const FString&)
DECLARE_DELEGATE_OneParam(FOnSlotDeleted, const FString&)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API USaveGameItemWidget : public UUserWidget {
	GENERATED_BODY()
public:
	void SetSlotName(const FString& NewSlotName);
	const FString& GetSlotName() const;

	FOnSlotSelected OnSlotSelected;
	FOnSlotDeleted OnSlotDeleted;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SlotNameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SlotDeleteButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SlotNameTextBlock;

	virtual void NativeOnInitialized() override;

private:
	FString SlotName;

	UFUNCTION()
	void HandleSlotNameButtonClick();

	UFUNCTION()
	void HandleSlotDeleteButtonClick();
};
