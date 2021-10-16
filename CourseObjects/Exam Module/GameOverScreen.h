#pragma once
#include "GameScene.h"

class SpriteFont;

class GameOverScreen : public GameScene
{
public:
	enum GameOverMenuInput : UINT
	{
		RESTART = 0,
		MAINMENU,
		EXIT,
		CLICKED
	};

	GameOverScreen();
	virtual ~GameOverScreen() = default;

	void SetResultText(const std::wstring& text);
	
	GameOverScreen(const GameOverScreen& other) = delete;
	GameOverScreen(GameOverScreen&& other) noexcept = delete;
	GameOverScreen& operator=(const GameOverScreen& other) = delete;
	GameOverScreen& operator=(GameOverScreen&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	SpriteFont* m_pFont = nullptr;
	std::wstring m_ResultsText = L"";

	void HandleMouseClick(const GameContext& gameContext);
};

