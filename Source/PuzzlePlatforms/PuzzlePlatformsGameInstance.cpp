// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBP.Class)) return;
	
	MenuClass = MenuBP.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass)) return;

	if (UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(this, MenuClass))
	{
		MainMenuWidget->AddToViewport();

		if (APlayerController* PlayerController = GetFirstLocalPlayerController())
		{
			FInputModeUIOnly Params;
			Params.SetWidgetToFocus(MainMenuWidget->TakeWidget());
			Params.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			
			PlayerController->SetInputMode(Params);
			PlayerController->bShowMouseCursor = true;
		}
	}
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
