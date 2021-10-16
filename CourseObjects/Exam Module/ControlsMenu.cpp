#include "stdafx.h"
#include "ControlsMenu.h"
#include "Components.h"
#include "GameObject.h"
#include "SceneManager.h"

ControlsMenu::ControlsMenu()
	:GameScene(L"ControlsMenu")
{
}

void ControlsMenu::Initialize()
{
	const auto gameContext = GetGameContext();

	gameContext.pGameTime->ForceElapsedUpperbound(true);

	auto* pMenuObject = new GameObject();
	auto* pSpriteComp = new SpriteComponent(L"Resources/Textures/ControlsMenu.png");
	pMenuObject->AddComponent(pSpriteComp);
	AddChild(pMenuObject);

	//Input
	gameContext.pInput->RefreshControllerConnections();
	GamepadIndex activeController = GamepadIndex::PlayerOne;
	if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerOne))
		activeController = GamepadIndex::PlayerOne;
	else if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerTwo))
		activeController = GamepadIndex::PlayerTwo;
	else if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerThree))
		activeController = GamepadIndex::PlayerThree;
	else if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerFour))
		activeController = GamepadIndex::PlayerThree;

	InputAction newInput = InputAction(ControlsMenuInput::START, InputTriggerState::Released, VK_RETURN, -1, XINPUT_GAMEPAD_A, activeController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(ControlsMenuInput::EXIT, InputTriggerState::Released, VK_ESCAPE, -1, XINPUT_GAMEPAD_B, activeController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(ControlsMenuInput::CLICKED, InputTriggerState::Released, -1, VK_LBUTTON);
	gameContext.pInput->AddInputAction(newInput);
}

void ControlsMenu::Update()
{
	const auto gameContext = GetGameContext();

	if (gameContext.pInput->IsActionTriggered(ControlsMenuInput::START))
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"TutorialLevel");
		return;
	}

	if (gameContext.pInput->IsActionTriggered(ControlsMenuInput::EXIT))
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"MainMenuScene");
		return;
	}

	if (gameContext.pInput->IsActionTriggered(ControlsMenuInput::CLICKED))
	{
		auto mousePos = gameContext.pInput->GetMousePosition();
		HandleMouseClick(mousePos);
	}
}

void ControlsMenu::Draw()
{
}

void ControlsMenu::HandleMouseClick(POINT mousePos)
{
	if (1078 < mousePos.x && mousePos.x < 1265 &&
		141 < mousePos.y && mousePos.y < 237)
		SceneManager::GetInstance()->SetActiveGameScene(L"TutorialLevel");
	else if (1078 < mousePos.x && mousePos.x < 1265 &&
		270 < mousePos.y && mousePos.y < 365)
		SceneManager::GetInstance()->SetActiveGameScene(L"MainMenuScene");
}