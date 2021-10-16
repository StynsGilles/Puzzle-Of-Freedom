#pragma once
#include "GameObject.h"

class ControllerComponent;

class Pillar :  public GameObject
{
public:
	Pillar(const std::wstring& filepath, float width, float height);
	virtual ~Pillar() = default;
	
	bool Push(const GameContext& gameContext, const physx::PxVec3& translation, const DirectX::XMFLOAT3& playerRight,
		const DirectX::XMFLOAT3& playerForward, bool& isGrounded, float addedWith = 0.f);
	
	Pillar(const Pillar& other) = delete;
	Pillar(Pillar&& other) noexcept = delete;
	Pillar& operator=(const Pillar& other) = delete;
	Pillar& operator=(Pillar&& other) noexcept = delete;
protected:
	void Initialize(const GameContext & gameContext) override;
	void PostInitialize(const GameContext&) override;
	void Update(const GameContext & gameContext) override;

	float m_Width;
	float m_Height;
	std::wstring m_Filepath;
};

