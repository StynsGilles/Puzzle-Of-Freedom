#include "stdafx.h"
#include "MainMenuScene.h"
#include "Components.h"
#include "GameObject.h"
#include "SceneManager.h"

MainMenuScene::MainMenuScene()
	: GameScene(L"MainMenuScene")
{
}

void MainMenuScene::Initialize()
{
	const auto gameContext = GetGameContext();
	
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	auto* pMenuObject = new GameObject();
	auto* pSpriteComp = new SpriteComponent(L"Resources/Textures/MainMenu.png");
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
	
	InputAction newInput = InputAction(MainMenuInput::START, InputTriggerState::Released, VK_RETURN, -1, XINPUT_GAMEPAD_A, activeController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(MainMenuInput::EXIT, InputTriggerState::Released, VK_ESCAPE, -1, XINPUT_GAMEPAD_B, activeController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(MainMenuInput::CLICKED, InputTriggerState::Released, -1, VK_LBUTTON);
	gameContext.pInput->AddInputAction(newInput);
}

void MainMenuScene::Update()
{
	const auto gameContext = GetGameContext();
	
	if ( gameContext.pInput->IsActionTriggered(MainMenuInput::START))
		SceneManager::GetInstance()->SetActiveGameScene(L"ControlsMenu");
	else if (gameContext.pInput->IsActionTriggered(MainMenuInput::EXIT))
		PostQuitMessage(0);
	else if (gameContext.pInput->IsActionTriggered(MainMenuInput::CLICKED))
	{
		auto mousePos = gameContext.pInput->GetMousePosition();
		HandleMouseClick(mousePos);
	}
}

void MainMenuScene::Draw()
{
}

void MainMenuScene::HandleMouseClick(POINT mousePos)
{
	if (339 < mousePos.x && mousePos.x < 521 &&
		247 < mousePos.y && mousePos.y < 339)
		SceneManager::GetInstance()->SetActiveGameScene(L"ControlsMenu");

	else if (339 < mousePos.x && mousePos.x < 521 &&
		430 < mousePos.y && mousePos.y < 520)
		PostQuitMessage(0);
}