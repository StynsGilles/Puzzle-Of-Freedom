#pragma once
#include "GameScene.h"

class MainMenuScene : public GameScene
{
public:
	enum MainMenuInput : UINT
	{
		START = 0,
		EXIT,
		CLICKED
	};
	
	MainMenuScene();
	virtual ~MainMenuScene() = default;

	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	
	void HandleMouseClick(POINT mousePos);
};

