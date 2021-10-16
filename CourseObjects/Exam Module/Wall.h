#pragma once
#include "GameObject.h"

class Wall : public GameObject
{
public:
	Wall(bool climbable, float width, float height, const DirectX::XMFLOAT3& meshScale);
	virtual ~Wall() = default;

	bool IsClimbable() const;
	float GetHeight() const;
	
	Wall(const Wall& other) = delete;
	Wall(Wall&& other) noexcept = delete;
	Wall& operator=(const Wall& other) = delete;
	Wall& operator=(Wall&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

	bool m_Climbable;
	float m_Width;
	float m_Height;
	DirectX::XMFLOAT3 m_Scale;
};