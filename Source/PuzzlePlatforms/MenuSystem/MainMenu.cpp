// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
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
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	}

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}

	if (ButtonQuit)
	{
		ButtonQuit->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	}

	return true;
}

void UMainMenu::QuitGame()
{
	if (MenuInterface)
	{
		MenuInterface->QuitApplication();
	}
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MainMenuSwitcher)
	{
		UE_LOG(LogTemp, Warning, TEXT("Switching to join menu."));
		MainMenuSwitcher->SetActiveWidget(JoinMenu);
	}
}

void UMainMenu::OpenMainMenu()
{
	if (MainMenuSwitcher)
	{
		UE_LOG(LogTemp, Warning, TEXT("Switching to main menu."));
		MainMenuSwitcher->SetActiveWidget(MainMenu);
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface)
	{
		if (IPAddressField)
		{
			const FString& Address = IPAddressField->GetText().ToString();
			MenuInterface->Join(Address);
			
			UE_LOG(LogTemp, Warning, TEXT("Joining server IP: %s"), *Address);
		}
	}
}
