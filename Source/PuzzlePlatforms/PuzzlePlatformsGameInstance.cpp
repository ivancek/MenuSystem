// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBP.Class)) return;
	
	MenuClass = MenuBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBP.Class)) return;

	InMenuClass = InGameMenuBP.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *InMenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass)) return;

	if (UMenuWidget* MainMenuWidget = CreateWidget<UMenuWidget>(this, MenuClass))
	{
		MainMenuWidget->SetMenuInterface(this);
		MainMenuWidget->Setup();
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(InMenuClass)) return;

	if (UMenuWidget* MainMenuWidget = CreateWidget<UMenuWidget>(this, InMenuClass))
	{
		MainMenuWidget->SetMenuInterface(this);
		MainMenuWidget->Setup();
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
	if (UEngine* Engine = GetEngine())
	{
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	}

	if (UWorld* World = GetWorld())
	{
		World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	if (UEngine* Engine = GetEngine())
	{
		Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	}
	
	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
	
}
