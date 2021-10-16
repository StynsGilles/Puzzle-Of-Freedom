#pragma once
#include "BaseLevelScene.h"

struct InitValue;
class SpriteComponent;
class Hero;
class SpriteFont;

class ExamScene final : public BaseLevelScene
{
public:
	ExamScene();
	virtual ~ExamScene() = default;

	ExamScene(const ExamScene& other) = delete;
	ExamScene(ExamScene&& other) noexcept = delete;
	ExamScene& operator=(const ExamScene& other) = delete;
	ExamScene& operator=(ExamScene&& other) noexcept = delete;

protected:
	void Initialize() override;
};


