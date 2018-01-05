// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"

#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool bSuccess = Super::Initialize();

	if (ButtonCancel)
	{
		ButtonCancel->OnClicked.AddDynamic(this, &UInGameMenu::Teardown);
	}

	if (ButtonQuit)
	{
		ButtonQuit->OnClicked.AddDynamic(this, &UInGameMenu::QuitGame);
	}

	return true;
}

void UInGameMenu::Teardown()
{
	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	Super::Teardown(World);
}

void UInGameMenu::QuitGame()
{
	if (MenuInterface)
	{
		MenuInterface->LeaveGame();
	}
}
