// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"

#include "ServerLine.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerLineClassBP(TEXT("/Game/MenuSystem/WBP_ServerLine"));
	if (ensure(ServerLineClassBP.Class))
	{
		ServerLineClass = ServerLineClassBP.Class;
	}
}

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
		ButtonQuit->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);
	}

	return true;
}

void UMainMenu::CreateServerList(TArray<FString> ServerNames)
{
	if (!ensure(ServerLineClass)) return;

	if (UWorld* World = GetWorld())
	{
		ServerListBox->ClearChildren();

		for (auto& ServerName : ServerNames)
		{
			if (UServerLine* ServerLine = CreateWidget<UServerLine>(World, ServerLineClass))
			{
				ServerLine->ServerName->SetText(FText::FromString(ServerName));
				ServerListBox->AddChild(ServerLine);
			}
		}
	}
}

void UMainMenu::QuitPressed()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		const FString& Command = "Quit";

		PlayerController->ConsoleCommand(Command);
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
		
		MenuInterface->FindSessions();
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
		MenuInterface->Join("");
	}
}
