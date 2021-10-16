#pragma once
#include "GameScene.h"

class ControlsMenu : public GameScene
{
public:
	enum ControlsMenuInput : UINT
	{
		START = 0,
		EXIT,
		CLICKED
	};

	ControlsMenu();
	virtual ~ControlsMenu() = default;

	ControlsMenu(const ControlsMenu& other) = delete;
	ControlsMenu(ControlsMenu&& other) noexcept = delete;
	ControlsMenu& operator=(const ControlsMenu& other) = delete;
	ControlsMenu& operator=(ControlsMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	void HandleMouseClick(POINT mousePos);
};

