// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"


void UMenuWidget::SetMenuInterface(IMenuInterface* Interface)
{
	MenuInterface = Interface;
}

void UMenuWidget::Setup()
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

void UMenuWidget::Teardown(UWorld * InWorld)
{
	this->RemoveFromViewport();

	if (APlayerController* PlayerController = InWorld->GetFirstPlayerController())
	{
		FInputModeGameOnly Params;
		PlayerController->SetInputMode(Params);
		PlayerController->bShowMouseCursor = false;
	}
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	Teardown(InWorld);
}
