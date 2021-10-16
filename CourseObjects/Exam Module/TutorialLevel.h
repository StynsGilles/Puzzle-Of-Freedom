#pragma once
#include "BaseLevelScene.h"

struct TextAndPos
{
	std::wstring text;
	DirectX::XMFLOAT2 pos;
	float timeLeft = 5.f;

	friend bool operator==(const TextAndPos& lValue, const TextAndPos& rValue)
	{
		return wcscmp(lValue.text.c_str(), rValue.text.c_str()) == 0 &&
			lValue.pos.x == rValue.pos.x &&
			lValue.pos.y == rValue.pos.y;
	}
};

class TutorialLevel : public BaseLevelScene
{
public:
	TutorialLevel();
	virtual ~TutorialLevel() = default;

	TutorialLevel(const TutorialLevel& other) = delete;
	TutorialLevel(TutorialLevel&& other) noexcept = delete;
	TutorialLevel& operator=(const TutorialLevel& other) = delete;
	TutorialLevel& operator=(TutorialLevel&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;

private:
	std::vector<TextAndPos> m_HUDTextVector;

	void ResetScene() override;
	void DrawHud() override;
	void CreateInitialTrigger();
	void CreateTopTrigger();
	void CreateTreasureFoundTrigger();
};

