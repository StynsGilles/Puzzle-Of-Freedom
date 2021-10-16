#pragma once
#include <deque>

#include "GameObject.h"

class ModelComponent;

struct AnimationInfo
{
	UINT animationId;
	bool loop = true;
	bool playReversed = false;
	float duration = 1.f;
};

class MeshObject : public GameObject
{
public:
	enum class AnimationId : UINT
	{
		Idle = 0,
		CrouchIdle,
		ShimmyLeft,
		ShimmyRight,
		HangToCrouch,
		BraceToFreeHang,
		ClimbDownWall,
		ClimbUpWall,
		CrouchToStand,
		CrouchWalkBack,
		CrouchWalkForward,
		CrouchWalkLeft,
		CrouchWalkRight,
		FallToIdle,
		FallLoop,
		FreeHangToBrace,
		HangingIdle,
		JumpUp,
		LeftStrafe,
		PullObject,
		PullRope,
		PushObject,
		RightStrafe,
		RunningBackwards,
		Running,
		StandingToCrouch
	};
	
	MeshObject();
	virtual ~MeshObject();

	MeshObject(const MeshObject& other) = delete;
	MeshObject(MeshObject&& other) noexcept = delete;
	MeshObject& operator=(const MeshObject& other) = delete;
	MeshObject& operator=(MeshObject&& other) noexcept = delete;

	void SetAnimation(AnimationId animationId, bool loop = true, bool reversed = false);
	void SetForcedAnimation(AnimationId animationId, bool loop = true, bool reversed = false);
	void PauseAnimation();
	void PlayAnimation();
	
protected:
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	
	ModelComponent* m_pMesh;

	bool m_PlayAnimation = true;
	std::deque<std::shared_ptr< AnimationInfo>> m_animationQueue;
	float m_TimePassedCurrentAnim = 0.f;
	std::shared_ptr< AnimationInfo> m_CurrentAnimation;

private:
};

