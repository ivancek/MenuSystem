// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	
	
protected:
	
	

private:
	
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IPAddressField;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MainMenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();
	
	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

};
