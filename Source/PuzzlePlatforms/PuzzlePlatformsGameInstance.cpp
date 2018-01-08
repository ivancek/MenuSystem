// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"


#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

const static FName SESSION_NAME = TEXT("My Session Game");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBP.Class)) return;
	
	MenuClass = MenuBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBP.Class)) return;

	InGameMenuClass = InGameMenuBP.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *InGameMenuClass->GetName());

	if (IOnlineSubsystem* OSubsys = IOnlineSubsystem::Get())
	{
		FString SubsysName = OSubsys->GetSubsystemName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("Online subsystem name: %s"), *SubsysName);

		SessionInterface = OSubsys->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Online subsystem not found"));
	}
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass)) return;
	
	MainMenuWidget = CreateWidget<UMainMenu>(this, MenuClass);
	if (MainMenuWidget)
	{
		MainMenuWidget->SetMenuInterface(this);
		MainMenuWidget->Setup();
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass)) return;
	
	InGameMenuWidget = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (InGameMenuWidget)
	{
		InGameMenuWidget->SetMenuInterface(this);
		InGameMenuWidget->Setup();
	}
}

void UPuzzlePlatformsGameInstance::LeaveGame()
{
	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
	}

	UE_LOG(LogTemp, Warning, TEXT("Leaving game."));
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		if (SessionInterface->GetNamedSession(SESSION_NAME))
		{
			SessionInterface->DestroySession(SESSION_NAME, nullptr);
		}
		else
		{
			CreateNewSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) { return; }

	if (!SessionSearch.IsValid()) { return; }

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session name: %s"), *SessionName.ToString());

		if (UEngine* Engine = GetEngine())
		{
			Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
		}

		if (UWorld* World = GetWorld())
		{
			World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
		}
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		CreateNewSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful && SessionSearch.IsValid() && MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished search."));
		
		TArray<FString> ServerNames;
		for (const auto& Result : SessionSearch->SearchResults)
		{
			if (Result.IsValid())
			{
				auto Name = Result.GetSessionIdStr();
				ServerNames.Add(Name);
			}
		}

		MainMenuWidget->CreateServerList(ServerNames);
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) { return; }

	FString ConnectInfo;
	if (!SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}
	
	if (UEngine* Engine = GetEngine())
	{
		Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *ConnectInfo));
	}

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerController = GetFirstLocalPlayerController())
		{
			PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
		}
	}

}

void UPuzzlePlatformsGameInstance::CreateNewSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bUsesPresence = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::FindSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Looking for sessions..."));

		SessionSearch->MaxSearchResults = 100;
		SessionSearch->bIsLanQuery = true;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}
