// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerLine.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "MainMenu.h"

void UServerLine::Setup(UMainMenu * Parent, uint32 Index)
{
	MyIndex = Index;
	Owner = Parent;

	if (ServerLineButton)
	{
		ServerLineButton->OnClicked.AddDynamic(this, &UServerLine::ButtonPressed);
	}
}

void UServerLine::ButtonPressed()
{
	if (Owner)
	{
		Owner->SelectIndex(MyIndex);
	}
}
