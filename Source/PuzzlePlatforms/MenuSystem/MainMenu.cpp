// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Engine/World.h"

bool UMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();

	if (!bSuccess) { return false; }

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* Interface)
{
	MenuInterface = Interface;
}

void UMainMenu::Setup()
{
	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	this->AddToViewport();

	if (APlayerController* PlayerController = World->GetFirstPlayerController())
	{
		FInputModeUIOnly Params;
		Params.SetWidgetToFocus(this->TakeWidget());
		Params.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(Params);
		PlayerController->bShowMouseCursor = true;
	}
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	this->RemoveFromViewport();

	if (APlayerController* PlayerController = InWorld->GetFirstPlayerController())
	{
		FInputModeGameOnly Params;
		PlayerController->SetInputMode(Params);
		PlayerController->bShowMouseCursor = false;
	}
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface)
	{
		FString Address;
		MenuInterface->Join(Address);
	}
}
