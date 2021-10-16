#include "stdafx.h"
#include "GameOverScreen.h"
#include "Components.h"
#include "ContentManager.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "TextRenderer.h"
#include "OverlordGame.h"
#include "SpriteFont.h"

GameOverScreen::GameOverScreen()
	: GameScene(L"GameOverScreen")
{
}

void GameOverScreen::Initialize()
{
	const auto gameContext = GetGameContext();

	gameContext.pGameTime->ForceElapsedUpperbound(true);

	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Immortal_64.fnt");
	
	auto* pMenuObject = new GameObject();
	auto* pSpriteComp = new SpriteComponent(L"Resources/Textures/GameOverMenu.png");
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
	
	InputAction newInput = InputAction(GameOverMenuInput::RESTART, InputTriggerState::Released, -1, -1, XINPUT_GAMEPAD_A, activeController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(GameOverMenuInput::MAINMENU, InputTriggerState::Released, -1, -1, XINPUT_GAMEPAD_B, activeController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(GameOverMenuInput::EXIT, InputTriggerState::Released, -1, -1, XINPUT_GAMEPAD_X, activeController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(GameOverMenuInput::CLICKED, InputTriggerState::Released, -1, VK_LBUTTON);
	gameContext.pInput->AddInputAction(newInput);
}

void GameOverScreen::Update()
{
	const auto gameContext = GetGameContext();

	if (gameContext.pInput->IsActionTriggered(GameOverMenuInput::RESTART))
		SceneManager::GetInstance()->SetActiveGameScene(L"ControlsMenu");
	else if (gameContext.pInput->IsActionTriggered(GameOverMenuInput::MAINMENU))
		SceneManager::GetInstance()->SetActiveGameScene(L"MainMenuScene");
	else if (gameContext.pInput->IsActionTriggered(GameOverMenuInput::EXIT))
		PostQuitMessage(0);
	else if (gameContext.pInput->IsActionTriggered(GameOverMenuInput::CLICKED))
		HandleMouseClick(gameContext);
}

void GameOverScreen::Draw()
{
	const auto windowSettings = OverlordGame::GetGameSettings().Window;
	const auto widthPos = (float)windowSettings.Width / 2.f - 200.f;
	
	TextRenderer::GetInstance()->DrawText(m_pFont, m_ResultsText, DirectX::XMFLOAT2(widthPos, 20), DirectX::XMFLOAT4(0, 0, 0, 1));
}

void GameOverScreen::SetResultText(const std::wstring& text)
{
	m_ResultsText = text;
}

void GameOverScreen::HandleMouseClick(const GameContext& gameContext)
{
	auto mousePos = gameContext.pInput->GetMousePosition();
	
	if (271 < mousePos.x && mousePos.x < 625 &&
		241 < mousePos.y && mousePos.y < 335)
		SceneManager::GetInstance()->SetActiveGameScene(L"ControlsMenu");
	else if (271 < mousePos.x && mousePos.x < 625 &&
		363 < mousePos.y && mousePos.y < 454)
		SceneManager::GetInstance()->SetActiveGameScene(L"MainMenuScene");
	else if (271 < mousePos.x && mousePos.x < 625 &&
		482 < mousePos.y && mousePos.y < 581)
		PostQuitMessage(0);
}
