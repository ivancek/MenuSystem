// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;


protected:
	UFUNCTION()
	void QuitGame();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonQuit;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonCancel;

	UFUNCTION()
	void Teardown();
};
