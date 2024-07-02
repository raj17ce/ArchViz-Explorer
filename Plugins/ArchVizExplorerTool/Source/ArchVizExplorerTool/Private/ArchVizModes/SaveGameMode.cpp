// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/SaveGameMode.h"
#include "Widgets/SaveGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/ArchVizSlotSaveGame.h"
#include "ArchVizActors/ArchVizActor.h"
#include "EngineUtils.h"
#include "SaveGame/ArchVizSaveGame.h"

USaveGameMode::USaveGameMode(): CurrentSlotName{ TEXT("") } {

}

void USaveGameMode::Setup() {
	SlotsList = GetSavedSlotsList();

	if (IsValid(WidgetClass) && !IsValid(Widget)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass, "Save Game Widget");

		if (auto* SaveGameWidget = Cast<USaveGameWidget>(Widget)) {
			SaveGameWidget->PopulateSavedSlotsList(SlotsList);
			SaveGameWidget->NewProjectButton->OnClicked.AddDynamic(this, &USaveGameMode::HandleNewProjectButtonClick);
			SaveGameWidget->SavePopupSaveButton->OnClicked.AddDynamic(this, &USaveGameMode::HandleSaveButtonClick);
			SaveGameWidget->OnSaveSlotReceived.BindUObject(this, &USaveGameMode::HandleSlotItemNameButtonClick);
			SaveGameWidget->OnSaveSlotDeleteReceived.BindUObject(this, &USaveGameMode::HandleSlotItemDeleteButtonClick);
		}
	}
}

void USaveGameMode::Cleanup() {
	HideWidget();
}

void USaveGameMode::SetupInputComponent() {
	//No-Implementation
}

void USaveGameMode::EnterMode() {
	ShowWidget();
}

void USaveGameMode::ExitMode() {
	HideWidget();
}

void USaveGameMode::HandleSaveButtonClick() {
	if (auto* SaveGameWidget = Cast<USaveGameWidget>(Widget)) {
		FString SlotName = SaveGameWidget->SaveSlotName->GetText().ToString();

		if (SlotName.IsEmpty()) {
			// To-Do :: Notify the name is empty
			return;
		}

		if (GetSavedSlotsList().Contains(SlotName) && SlotName != CurrentSlotName) {
			// To-Do :: Notify("This Name Already Exists. Please Choose Differnet Name");
			return;
		}

		if (GetSavedSlotsList().Contains(SlotName) && SlotName == CurrentSlotName) {
			
		}

		CurrentSlotName = SlotName;
		SaveGame(CurrentSlotName);

		SaveGameWidget->HandleSavePopupCloseButtonClick();
		//SaveGameWidget->SaveSlotName->SetText(FText{});
	}
}

void USaveGameMode::HandleNewProjectButtonClick() {
	//if (!CurrentSlotName.IsEmpty()) {
	//	// To-Do :: Notify the name is empty
	//	return;
	//}
	//SaveGame(CurrentSlotName);

	//CurrentSlotName = "";

}

void USaveGameMode::HandleSlotItemNameButtonClick(const FString& SlotName) {
	if (auto* SaveGameWidget = Cast<USaveGameWidget>(Widget)) {
		if (CurrentSlotName == SlotName) {
			// Notify("Project is Already Opened.");
			return;
		}

		CurrentSlotName = SlotName;
		LoadGame(CurrentSlotName);

		SaveGameWidget->HandleLoadPopupCloseButtonClick();
		// To-Do :: Success
	}
}

void USaveGameMode::HandleSlotItemDeleteButtonClick(const FString& SlotName) {
	if (SlotName == CurrentSlotName) {
		// To-Do :: Notify("Can Not Delete Running Project.");
		return;
	}

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) {
		if (UGameplayStatics::DeleteGameInSlot(SlotName, 0)) {
			DeleteSlotData(SlotName);

			// To-Do :: Notify("Project " + SlotInfo + " Deleted");
		}
	}
}

void USaveGameMode::DeleteSlotData(const FString& SlotName) {
	if (auto* ArchVizSlotSaveGame = Cast<UArchVizSlotSaveGame>(UGameplayStatics::LoadGameFromSlot("SavedSlotNames", 0))) {
		ArchVizSlotSaveGame->SlotsNames.Remove(SlotName);
		SlotsList = ArchVizSlotSaveGame->SlotsNames;
		PopulateSlotsList();

		UGameplayStatics::SaveGameToSlot(ArchVizSlotSaveGame, "SavedSlotNames", 0);
	}
}

TArray<FString> USaveGameMode::GetSavedSlotsList() {
	auto* Slots = Cast<UArchVizSlotSaveGame>(UGameplayStatics::LoadGameFromSlot("SavedSlotNames", 0));

	if (Slots) {
		return Slots->SlotsNames;
	}

	return TArray<FString>{};
}

void USaveGameMode::SaveGame(const FString& SlotName) {
	/*FString SaveDirectory = FPaths::ProjectSavedDir();
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*SaveDirectory)) {
		UE_LOG(LogTemp, Warning, TEXT("Save directory does not exist. Attempting to create: %s"), *SaveDirectory);
		if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*SaveDirectory)) {
			UE_LOG(LogTemp, Error, TEXT("Failed to create save directory: %s"), *SaveDirectory);
			return;
		}
	}*/

	auto* SaveGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveGame::StaticClass()));

	if (!IsValid(SaveGameInstance)) {
		return;
	}

	for (TActorIterator<ARoadActor> It(GetWorld()); It; ++It) {
		auto* RoadActor = *It;

		FRoadData RoadData;
		RoadData.ID = RoadActor->GetID();
		RoadData.Transform = RoadActor->GetActorTransform();
		RoadData.SplinePoints = RoadActor->GetSplinePoints();
		RoadData.RoadType = RoadActor->GetRoadType();
		RoadData.Width = RoadActor->GetWidth();
		RoadData.Material = RoadActor->GetMaterial();
		if (IsValid(RoadActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(RoadActor->GetAttachParentActor())) {
				RoadData.ParentActorID = ParentActor->GetID();
			}
		}
		else {
			RoadData.ParentActorID = -1;
		}
		SaveGameInstance->RoadActors.Add(RoadData);
	}

	for (TActorIterator<AWallActor> It(GetWorld()); It; ++It) {
		auto* WallActor = *It;

		FWallData WallData;
		WallData.ID = WallActor->GetID();
		WallData.Transform = WallActor->GetActorTransform();
		WallData.Length = WallActor->GetLength();
		WallData.Material = WallActor->GetMaterial();

		if (IsValid(WallActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(WallActor->GetAttachParentActor())) {
				WallData.ParentActorID = ParentActor->GetID();
			}
		}
		else {
			WallData.ParentActorID = -1;

		}

		SaveGameInstance->WallActors.Add(WallData);
	}

	for (TActorIterator<AFloorActor> It(GetWorld()); It; ++It) {
		auto* FloorActor = *It;

		FFloorData FloorData;
		FloorData.ID = FloorActor->GetID();
		FloorData.Transform = FloorActor->GetActorTransform();
		FloorData.Material = FloorActor->GetMaterial();
		FloorData.StartPoint = FloorActor->GetStartPoint();
		FloorData.EndPoint = FloorActor->GetEndPoint();
		FloorData.Dimensions = FloorActor->GetDimensions();
		FloorData.Dimensions = FloorActor->GetOffset();
		if (IsValid(FloorActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(FloorActor->GetAttachParentActor())) {
				FloorData.ParentActorID = ParentActor->GetID();
			}
		}
		else {
			FloorData.ParentActorID = -1;

		}
		SaveGameInstance->FloorActors.Add(FloorData);
	}

	for (TActorIterator<ARoofActor> It(GetWorld()); It; ++It) {
		auto* RoofActor = *It;

		FRoofData RoofData;
		RoofData.ID = RoofActor->GetID();
		RoofData.Transform = RoofActor->GetActorTransform();
		RoofData.Material = RoofActor->GetMaterial();
		RoofData.StartPoint = RoofActor->GetStartPoint();
		RoofData.EndPoint = RoofActor->GetEndPoint();
		RoofData.Dimensions = RoofActor->GetDimensions();
		RoofData.Offset = RoofActor->GetOffset();
		if (IsValid(RoofActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(RoofActor->GetAttachParentActor())) {
				RoofData.ParentActorID = ParentActor->GetID();
			}
		}
		else {
			RoofData.ParentActorID = -1;

		}
		SaveGameInstance->RoofActors.Add(RoofData);
	}

	for (TActorIterator<ADoorActor> It(GetWorld()); It; ++It) {
		auto* DoorActor = *It;

		FDoorData DoorData;
		DoorData.ID = DoorActor->GetID();
		DoorData.Transform = DoorActor->GetActorTransform();
		DoorData.bIsOpen = DoorActor->GetIsOpen();
		DoorData.ParentComponentIndex = DoorActor->GetParentWallComponentIndex();

		if (IsValid(DoorActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(DoorActor->GetAttachParentActor())) {
				DoorData.ParentActorID = ParentActor->GetID();
			}
		}
		else {
			DoorData.ParentActorID = -1;

		}
		SaveGameInstance->DoorActors.Add(DoorData);
	}


	for (TActorIterator<AInteriorActor> It(GetWorld()); It; ++It) {
		auto* InteriorActor = *It;

		FInteriorData InteriorData;
		InteriorData.ID = InteriorActor->GetID();
		InteriorData.Transform = InteriorActor->GetActorTransform();
		InteriorData.AssetData = InteriorActor->GetActorAssetData();
		if (IsValid(InteriorActor->GetAttachParentActor())) {
			if (auto ParentActor = Cast<AArchVizActor>(InteriorActor->GetAttachParentActor())) {
				InteriorData.ParentActorID = ParentActor->GetID();
			}
		}
		else {
			InteriorData.ParentActorID = -1;
		}
		SaveGameInstance->InteriorActors.Add(InteriorData);
	}

	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0)) {
		auto* SlotSaveGameInstance = Cast<UArchVizSlotSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizSlotSaveGame::StaticClass()));

		auto* LoadSavedSlotNames = Cast<UArchVizSlotSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SavedSlotNames"), 0));
		if (IsValid(LoadSavedSlotNames) && !LoadSavedSlotNames->SlotsNames.Contains(CurrentSlotName)) {
			SlotSaveGameInstance->SlotsNames = LoadSavedSlotNames->SlotsNames;

			FString NewSlotName;
			NewSlotName = SlotName;
			SlotSaveGameInstance->SlotsNames.Add(NewSlotName);

			SlotsList = SlotSaveGameInstance->SlotsNames;
			PopulateSlotsList();
		}

		UGameplayStatics::SaveGameToSlot(SlotSaveGameInstance, "SavedSlotNames", 0);
	}
}

void USaveGameMode::LoadGame(const FString& SlotName) {
	auto* LoadGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (LoadGameInstance) {

		ClearWorld();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		TMap<int32, AActor*> IDToActorMap;
		TMap<AActor*, int32> ActorToPIDMap;

		for (const auto& RoadData : LoadGameInstance->RoadActors) {
			auto* RoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorClass, RoadData.Transform, SpawnParams);
			RoadActor->SetActorTransform(RoadData.Transform);
			RoadActor->SetSplinePoints(RoadData.SplinePoints);
			RoadActor->SetRoadType(RoadData.RoadType);
			RoadActor->SetWidth(RoadData.Width);
			RoadActor->SetMaterial(RoadData.Material);
			RoadActor->UpdatePropertyPanelValues();
			RoadActor->UpdateRoad();
			IDToActorMap.Add(RoadData.ID, RoadActor);

			if (RoadData.ParentActorID != -1) {
				ActorToPIDMap.Add(RoadActor, RoadData.ParentActorID);
			}
		}


		for (const auto& WallData : LoadGameInstance->WallActors) {
			auto* WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorClass, WallData.Transform, SpawnParams);
			WallActor->SetActorTransform(WallData.Transform);
			WallActor->SetLength(WallData.Length);
			WallActor->SetMaterial(WallData.Material);
			WallActor->GenerateWallSegments();
			WallActor->UpdateLengthSpinBoxValue();
			IDToActorMap.Add(WallData.ID, WallActor);

			if (WallData.ParentActorID != -1) {
				ActorToPIDMap.Add(WallActor, WallData.ParentActorID);
			}
		}

		for (const auto& FloorData : LoadGameInstance->FloorActors) {
			auto* FloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorClass, FloorData.Transform, SpawnParams);
			FloorActor->SetActorTransform(FloorData.Transform);
			FloorActor->SetStartPoint(FloorData.StartPoint);
			FloorActor->SetEndPoint(FloorData.EndPoint);
			FloorActor->SetDimensions(FloorData.Dimensions);
			FloorActor->SetOffset(FloorData.Offset);
			FloorActor->SetMaterial(FloorData.Material);
			FloorActor->UpdateSpinBoxValue();
			FloorActor->GenerateFloor();
			IDToActorMap.Add(FloorData.ID, FloorActor);

			if (FloorData.ParentActorID != -1) {
				ActorToPIDMap.Add(FloorActor, FloorData.ParentActorID);
			}
		}

		for (const auto& RoofData : LoadGameInstance->RoofActors) {
			auto* RoofActor = GetWorld()->SpawnActor<ARoofActor>(RoofActorClass, RoofData.Transform, SpawnParams);
			RoofActor->SetActorTransform(RoofData.Transform);
			RoofActor->SetStartPoint(RoofData.StartPoint);
			RoofActor->SetEndPoint(RoofData.EndPoint);
			RoofActor->SetDimensions(RoofData.Dimensions);
			RoofActor->SetOffset(RoofData.Offset);
			RoofActor->SetMaterial(RoofData.Material);
			RoofActor->UpdateSpinBoxValue();
			RoofActor->AdjustDimensionAndOffset();
			RoofActor->GenerateRoof();
			IDToActorMap.Add(RoofData.ID, RoofActor);

			if (RoofData.ParentActorID != -1) {
				ActorToPIDMap.Add(RoofActor, RoofData.ParentActorID);
			}
		}


		for (const auto& DoorData : LoadGameInstance->DoorActors) {
			ADoorActor* DoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorClass, DoorData.Transform, SpawnParams);
			DoorActor->SetActorTransform(DoorData.Transform);
			DoorActor->SetParentWallComponentIndex(DoorData.ParentComponentIndex);
			DoorActor->SetIsOpen(DoorData.bIsOpen);
			IDToActorMap.Add(DoorData.ID, DoorActor);

			if (DoorData.ParentActorID != -1) {
				ActorToPIDMap.Add(DoorActor, DoorData.ParentActorID);
			}
		}

		for (const FInteriorData& InteriorData : LoadGameInstance->InteriorActors) {
			AInteriorActor* InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorClass, InteriorData.Transform, SpawnParams);
			InteriorActor->SetActorTransform(InteriorData.Transform);
			InteriorActor->SetActorAssetData(InteriorData.AssetData);
			IDToActorMap.Add(InteriorData.ID, InteriorActor);

			if (InteriorData.ParentActorID != -1) {
				ActorToPIDMap.Add(InteriorActor, InteriorData.ParentActorID);
			}
		}

		for (auto& [Actor, ParentActorID] : ActorToPIDMap) {
			if (auto ParentActorPtr = IDToActorMap.Find(ParentActorID)) {
				if (auto ParentActor = *ParentActorPtr; IsValid(ParentActor)) {
					if (Actor->IsA(ADoorActor::StaticClass())) {
						if (auto ParentWall = Cast<AWallActor>(ParentActor); IsValid(ParentWall)) {
							auto Door = Cast<ADoorActor>(Actor);
							if (IsValid(Door)) {
								auto WallSegments = ParentWall->GetWallSegments();
								ParentWall->AttachDoorComponent(WallSegments[Door->GetParentWallComponentIndex()], Door);
								ParentWall->UpdateDoorSegments();
							}
						}
					}
					else {
						Actor->AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
					}
				}
			}
		}
	}
}

void USaveGameMode::PopulateSlotsList() {
	if (auto* SaveGameWidget = Cast<USaveGameWidget>(Widget)) {
		SaveGameWidget->PopulateSavedSlotsList(SlotsList);
	}
}

void USaveGameMode::ClearWorld() {
	for (TActorIterator<AArchVizActor> It{ GetWorld() }; It; ++It) {
		if (auto* Actor = *It; IsValid(Actor)) {
			Actor->Destroy();
		}
	}
}
