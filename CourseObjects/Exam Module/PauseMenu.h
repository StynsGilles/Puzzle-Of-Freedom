#pragma once
#include "GameObject.h"

class PauseMenu : public GameObject
{
public:
	PauseMenu();
	virtual ~PauseMenu() = default;

	PauseMenu(const PauseMenu& other) = delete;
	PauseMenu(PauseMenu&& other) noexcept = delete;
	PauseMenu& operator=(const PauseMenu& other) = delete;
	PauseMenu& operator=(PauseMenu&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
};

