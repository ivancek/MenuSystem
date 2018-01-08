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

void UMainMenu::CreateServerList(TArray<FServerData> ServerNames)
{
	if (!ensure(ServerLineClass)) return;

	if (UWorld* World = GetWorld())
	{
		ServerListBox->ClearChildren();

		uint32 i = 0;

		for (const auto& ServerData : ServerNames)
		{
			if (UServerLine* ServerLine = CreateWidget<UServerLine>(World, ServerLineClass))
			{
				FText ServerName = FText::FromString(FString::Printf(TEXT("%s"), *ServerData.Name));
				FText HostUsername = FText::FromString(FString::Printf(TEXT("%s"), *ServerData.HostUsername));
				FText NumConnections = FText::FromString(FString::Printf(TEXT("%d / %d"), ServerData.CurrentPlayers, ServerData.MaxPlayers));
				
				ServerLine->ServerName->SetText(ServerName);
				ServerLine->HostUsername->SetText(HostUsername);
				ServerLine->PlayersNumBox->SetText(NumConnections);
				
				ServerLine->Setup(this, i);
				i++;
				
				ServerListBox->AddChild(ServerLine);
			}
		}
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;

	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerListBox->GetChildrenCount(); i++)
	{
		auto Child = Cast<UServerLine>(ServerListBox->GetChildAt(i));
		
		if (Child) // Check whether the child is null. It could be of other type, so cast can fail.
		{
			Child->bSelected = SelectedIndex.IsSet() && SelectedIndex .GetValue() == i;
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
	//CreateServerList({ "Test1 ", "Test2", "Test3" });
	
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
	if (SelectedIndex.IsSet() && MenuInterface)
	{
		MenuInterface->Join(SelectedIndex.GetValue());
		UE_LOG(LogTemp, Warning, TEXT("Selected index: %d"), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set."));
	}
}
