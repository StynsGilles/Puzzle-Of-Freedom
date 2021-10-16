#pragma once
#include "GameObject.h"

class Pillar;
class Wall;
class MeshObject;
class ControllerComponent;
class CameraComponent;

class Hero : public GameObject
{
public:
	enum class CharacterState
	{
		Idle,
		Crouching,
		Jumping,
		WallClimb,
		Pushing
	};
	
	enum CharacterMovement : UINT
	{
		LEFT = 0,
		RIGHT,
		FORWARD,
		BACKWARD,
		JUMP,
		CROUCH,
		INTERACT,
		PAUSE,
		RESUME,
		RESTART,
		MAINMENU,
		EXIT,
		CLICKED
	};

	Hero(float radius = 2, float height = 5, float moveSpeed = 100);
	virtual ~Hero() = default;

	Hero(const Hero& other) = delete;
	Hero(Hero&& other) noexcept = delete;
	Hero& operator=(const Hero& other) = delete;
	Hero& operator=(Hero&& other) noexcept = delete;

	GameObject* GetCamera() const { return m_pCameraObject; }
	bool IsTouchingPillar() const { return m_TouchingPillar; }
	bool IsPaused() const { return m_Paused; }
	void SetPause(bool pause);
	bool ShouldResetScene() const;
	void SetResetScene(bool reset);
	void StopSounds();
	void ResetAnimations();

protected:
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	//Input controller
	GamepadIndex m_ActiveController = GamepadIndex::PlayerOne;
	bool m_Paused = false;
	bool m_ResetScene = false;
	
	//Components
	MeshObject* m_pMesh;
	ControllerComponent* m_pController;

	//Camera
	CameraComponent* m_pCameraComponent;
	GameObject* m_pCameraObject;
	const float m_MaxCameraDistance = 60.f;
	float m_CameraAngle = 0.610865f;
	bool m_CameraInOriginalPos = true;

	float m_TotalYaw;
	float m_MoveSpeed, m_RotationSpeed;
	float m_Radius, m_Height;

	CharacterState m_State = CharacterState::Idle;

	//Changing Stance
	float m_changeTime = 0.f;
	bool m_ChangingStance = false;

	//Running
	const float m_MaxRunVelocity;
	float m_CurrentMaxVelocity,
		m_TerminalVelocity,
		m_Gravity,
		m_RunAccelerationTime,
		m_JumpAccelerationTime,
		m_RunAcceleration,
		m_JumpAcceleration,
		m_RunVelocity,
		m_JumpVelocity;

	//Climbing
	float m_MaxClimbVelocity,
		m_ClimbAccelerationTime,
		m_ClimbAcceleration,
		m_ClimbVelocity;
	Wall* m_pClimbedWall = nullptr;
	bool m_GettingOnWall = false;

	//Pushing
	bool m_TouchingPillar = false;
	Pillar* m_pPushedPillar = nullptr;
	float m_MaxPushVelocity,
		m_MaxPullVelocity,
		m_PushAccelerationTime,
		m_PushAcceleration,
		m_PushVelocity;

	DirectX::XMFLOAT3 m_Velocity;

	//Audio
	FMOD::Channel* m_pRunningChannel;
	FMOD::Channel* m_pJumpChannel;
	FMOD::Channel* m_pClimbChannel;
	FMOD::Sound* m_pJumpSound;
	FMOD::Channel* m_pPillarPushChannel;

	void HandlePauseMenu(const GameContext& gameContext);
	void UpdateCameraSpringArm();
	void CheckCollisions(const GameContext& gameContext);
	void RotateToRayCast(const physx::PxRaycastBuffer& hit);
	void HandleInput(const GameContext& gameContext);
	void HandleMovement(const GameContext& gameContext);
	void UpdateAiredVelocity(const GameContext& gameContext);
	void HandleClimbing(const GameContext& gameContext);
	void HandlePushing(const GameContext& gameContext);
	bool GetMovementDirection(const GameContext& gameContext, DirectX::XMFLOAT2& move);
	bool GetWallMovement(const GameContext& gameContext, float& move);
	bool GetPushMovement(const GameContext& gameContext, float& move, bool& pillarGrounded);
	void SetMovementAnimation(const DirectX::XMFLOAT2& move);
	void SetAnimationStanding(const DirectX::XMFLOAT2& move);
	void SetAnimationCrouched(const DirectX::XMFLOAT2& move);
	void SetAnimationWallClimb(float movement) const;
	void SetAnimationPush(float movement) const;
	void SetIdleAnimation() const;
};