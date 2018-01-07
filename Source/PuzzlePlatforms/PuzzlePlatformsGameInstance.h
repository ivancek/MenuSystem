// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;
class UMainMenu;
class UInGameMenu;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec, BlueprintCallable)
	virtual void LeaveGame() override;

	UFUNCTION(Exec)
	void Host() override;


	UFUNCTION(Exec)
	void Join(uint32 Index) override;
	
	virtual void FindSessions() override;

private:
	TSubclassOf<UUserWidget> MenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	UMainMenu* MainMenuWidget;
	
	UInGameMenu* InGameMenuWidget;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateNewSession();

};
