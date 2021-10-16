#pragma once
#include "BaseLevelScene.h"

class SpriteComponent;
class Hero;
class SpriteFont;

class LevelTwoScene final : public BaseLevelScene
{
public:
	LevelTwoScene();
	virtual ~LevelTwoScene() = default;

	LevelTwoScene(const LevelTwoScene& other) = delete;
	LevelTwoScene(LevelTwoScene&& other) noexcept = delete;
	LevelTwoScene& operator=(const LevelTwoScene& other) = delete;
	LevelTwoScene& operator=(LevelTwoScene&& other) noexcept = delete;

protected:
	void Initialize() override;
};
