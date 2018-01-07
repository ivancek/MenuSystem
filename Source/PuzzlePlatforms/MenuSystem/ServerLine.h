// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerLine.generated.h"

class UTextBlock;
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
	
};
