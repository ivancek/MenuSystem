// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableTextBox;
class UScrollBox;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	FString HostUsername;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;

};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;
	
	void CreateServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

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
	UButton* ButtonQuit;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ServerListBox;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MainMenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	TSubclassOf<UUserWidget> ServerLineClass;

	UFUNCTION()
	void QuitPressed();

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();
	
	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();
};
