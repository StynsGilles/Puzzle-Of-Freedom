#include "stdafx.h"
#include "PauseMenu.h"

#include "SpriteComponent.h"

PauseMenu::PauseMenu()
{
}

void PauseMenu::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	auto* pPauseMenu = new SpriteComponent(L"Resources/Textures/PauseMenu.png");
	pPauseMenu->SetDraw(false);
	AddComponent(pPauseMenu);
}

void PauseMenu::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void PauseMenu::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
