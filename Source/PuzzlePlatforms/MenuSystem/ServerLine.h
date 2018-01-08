// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerLine.generated.h"

class UTextBlock;
class UButton;
class UMainMenu;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerLine : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ServerLineButton;
	
	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;
	
	void Setup(class UMainMenu* Parent, uint32 Index);

private:
	UPROPERTY()
	UMainMenu* Owner;

	UFUNCTION()
	void ButtonPressed();
	
	uint32 MyIndex;
};
