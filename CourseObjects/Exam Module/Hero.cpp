#include "stdafx.h"
#include "Hero.h"
#include "Components.h"
#include "GameScene.h"
#include "PhysxManager.h"
#include "MeshObject.h"
#include "Wall.h"
#include "Pillar.h"
#include "SceneManager.h"
#include "SoundManager.h"

Hero::Hero(float radius, float height, float moveSpeed) :
	m_pController(nullptr),
	m_pCameraObject(nullptr),
	m_pCameraComponent(nullptr),
	m_TotalYaw(0),
	m_MoveSpeed(moveSpeed),
	m_RotationSpeed(0.7f),
	m_Radius(radius),
	m_Height(height),
	//Running
	m_MaxRunVelocity(50.0f),
	m_CurrentMaxVelocity(m_MaxRunVelocity),
	m_TerminalVelocity(20),
	m_Gravity(9.81f),
	m_RunAccelerationTime(0.3f),
	m_JumpAccelerationTime(2.5f),
	m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
	m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
	m_RunVelocity(0),
	m_JumpVelocity(0),
	//Climbing
	m_MaxClimbVelocity(5.0f),
	m_ClimbAccelerationTime(0.15f),
	m_ClimbAcceleration(m_MaxClimbVelocity / m_ClimbAccelerationTime),
	m_ClimbVelocity(0),
	//Pushing
	m_MaxPushVelocity(10.f),
	m_MaxPullVelocity(3.f),
	m_PushAccelerationTime(0.6f),
	m_PushAcceleration(m_MaxPushVelocity/m_PushAccelerationTime),
	m_PushVelocity(0),
	m_Velocity(0, 0, 0)
{}

void Hero::SetPause(bool pause)
{
	m_Paused = pause;
}

bool Hero::ShouldResetScene() const
{
	return m_ResetScene;
}

void Hero::SetResetScene(bool reset)
{
	m_ResetScene = reset;
}

void Hero::StopSounds()
{
	if (m_pRunningChannel != nullptr)
		m_pRunningChannel->setPaused(true);
	if (m_pJumpChannel != nullptr)
		m_pJumpChannel->setPaused(true);
	if (m_pClimbChannel != nullptr)
		m_pClimbChannel->setPaused(true);
}

void Hero::ResetAnimations()
{
	m_State = CharacterState::Idle;
	m_pMesh->SetAnimation(MeshObject::AnimationId::Idle);
	m_pMesh->PlayAnimation();
	GetTransform()->Translate(0, 0, 0);
}

void Hero::Initialize(const GameContext& gameContext)
{
	auto* physX = PhysxManager::GetInstance()->GetPhysics();
	auto* pMaterial = physX->createMaterial(0, 0, 1);

	//Children
	m_pCameraObject = new GameObject();
	AddChild(m_pCameraObject);
	m_pCameraComponent = new CameraComponent();
	m_pCameraObject->AddComponent(m_pCameraComponent);
	float zOffset{ -m_MaxCameraDistance * std::cosf(m_CameraAngle) };
	float yOffset{ m_MaxCameraDistance * std::sinf(m_CameraAngle) };
	m_pCameraComponent->GetTransform()->Translate(0, yOffset, zOffset);
	m_pCameraComponent->GetTransform()->Rotate(m_CameraAngle, 0, 0, false);
	
	m_pMesh = new MeshObject();
	AddChild(m_pMesh);
	
	//Components
	m_pController = new ControllerComponent(pMaterial);
	m_pController->SetCollisionGroup(CollisionGroupFlag::Group1);
	AddComponent(m_pController);

	//Audio
	FMOD_RESULT fmodResult;
	auto* pSoundSystem = SoundManager::GetInstance()->GetSystem();
	//Run
	FMOD::Sound* pRunSound;
	fmodResult = pSoundSystem->createSound("Resources/Audio/Running.mp3", FMOD_LOOP_NORMAL, NULL, &pRunSound);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	fmodResult = pSoundSystem->playSound(pRunSound, NULL, true, &m_pRunningChannel);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	m_pRunningChannel->setVolume(0.7f);

	//Climbing
	FMOD::Sound* pClimbSound;
	fmodResult = pSoundSystem->createSound("Resources/Audio/Climb.mp3", FMOD_LOOP_NORMAL, NULL, &pClimbSound);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	fmodResult = pSoundSystem->playSound(pClimbSound, NULL, true, &m_pClimbChannel);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	m_pClimbChannel->setVolume(0.7f);
	
	//Jump
	fmodResult =pSoundSystem->createSound("Resources/Audio/Jump.mp3", FMOD_LOOP_OFF, NULL, &m_pJumpSound);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);
	
	//PillarPushing
	FMOD::Sound* pPushSound;
	fmodResult = pSoundSystem->createSound("Resources/Audio/PillarPush.mp3", FMOD_LOOP_NORMAL, NULL, &pPushSound);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);
	fmodResult = pSoundSystem->playSound(pPushSound, NULL, true, &m_pPillarPushChannel);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);
	m_pPillarPushChannel->setVolume(0.5f);
	
	gameContext.pInput->RefreshControllerConnections();
	if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerOne))
		m_ActiveController = GamepadIndex::PlayerOne;
	else if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerTwo))
		m_ActiveController = GamepadIndex::PlayerTwo;
	else if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerThree))
		m_ActiveController = GamepadIndex::PlayerThree;
	else if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerFour))
		m_ActiveController = GamepadIndex::PlayerThree;
	
	//Input
	InputAction newInput = InputAction(CharacterMovement::LEFT, InputTriggerState::Down, 'A');
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::RIGHT, InputTriggerState::Down, 'D');
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::FORWARD, InputTriggerState::Down, 'W');
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::BACKWARD, InputTriggerState::Down, 'S');
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::JUMP, InputTriggerState::Released, VK_SPACE, -1, XINPUT_GAMEPAD_A, m_ActiveController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::CROUCH, InputTriggerState::Released, 'C', -1, XINPUT_GAMEPAD_B, m_ActiveController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::INTERACT, InputTriggerState::Released, 'E', -1, XINPUT_GAMEPAD_X, m_ActiveController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::PAUSE, InputTriggerState::Released, VK_ESCAPE, -1, XINPUT_GAMEPAD_START, m_ActiveController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::RESUME, InputTriggerState::Released, -1, -1, XINPUT_GAMEPAD_A, m_ActiveController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::RESTART, InputTriggerState::Released, -1, -1, XINPUT_GAMEPAD_B, m_ActiveController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::MAINMENU, InputTriggerState::Released, -1, -1, XINPUT_GAMEPAD_X, m_ActiveController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::EXIT, InputTriggerState::Released, -1, -1, XINPUT_GAMEPAD_Y, m_ActiveController);
	gameContext.pInput->AddInputAction(newInput);
	newInput = InputAction(CharacterMovement::CLICKED, InputTriggerState::Released, -1, VK_LBUTTON);
	gameContext.pInput->AddInputAction(newInput);
}

void Hero::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_pCameraComponent)
		m_pCameraComponent->SetActive();
	
	m_pMesh->GetTransform()->Rotate(0.f, 180.f, 0.f);
	GetTransform()->Translate(0, 6.f, 0.f);
}

void Hero::Update(const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(CharacterMovement::PAUSE))
		m_Paused = !m_Paused;
	
	if (m_Paused)
	{
		StopSounds();
		HandlePauseMenu(gameContext);
	}
	else
	{
		UpdateCameraSpringArm();
		CheckCollisions(gameContext);
		HandleInput(gameContext);
	}

}

void Hero::HandlePauseMenu(const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(CharacterMovement::RESUME))
		m_Paused = false;
	if (gameContext.pInput->IsActionTriggered(CharacterMovement::RESTART))
		m_ResetScene = true;
	if (gameContext.pInput->IsActionTriggered(CharacterMovement::MAINMENU))
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"MainMenuScene");
		StopSounds();
	}
	if (gameContext.pInput->IsActionTriggered(CharacterMovement::EXIT))
		PostQuitMessage(0);
	
	else if (gameContext.pInput->IsActionTriggered(CharacterMovement::CLICKED))
	{
		auto mousePos = gameContext.pInput->GetMousePosition();
		if (272 < mousePos.x && mousePos.x < 622 &&
			172 < mousePos.y && mousePos.y < 259)
			m_Paused = false;
		else if (272 < mousePos.x && mousePos.x < 622 &&
			291 < mousePos.y && mousePos.y < 391)
			m_ResetScene = true;
		else if (272 < mousePos.x && mousePos.x < 622 &&
			413 < mousePos.y && mousePos.y < 501)
		{
			SceneManager::GetInstance()->SetActiveGameScene(L"MainMenuScene");
			StopSounds();
		}
		else if (272 < mousePos.x && mousePos.x < 622 &&
			531 < mousePos.y && mousePos.y < 624)
			PostQuitMessage(0);
	}
}

void Hero::UpdateCameraSpringArm()
{
	const auto pos = ToPxVec3(GetTransform()->GetWorldPosition());
	const auto vecCharToCamera = ToPxVec3(m_pCameraObject->GetTransform()->GetWorldPosition()) - pos;
	const physx::PxVec3 unitDir{ vecCharToCamera.getNormalized() };
	const physx::PxReal maxDist{ m_MaxCameraDistance };
	physx::PxRaycastBuffer hit{};
	physx::PxQueryFilterData filterData;
	const physx::PxU32 CollGroup{ (UINT)CollisionGroupFlag::Group0 | (UINT)CollisionGroupFlag::Group2 | (UINT)CollisionGroupFlag::Group3 };
	filterData.data.word0 = CollGroup;

	auto* scene = m_pController->GetScene();
	if (scene->raycast(pos, unitDir, maxDist, hit, physx::PxHitFlag::eDEFAULT, filterData))
	{
		const auto lengthToHitBlock = (hit.getAnyHit(0).position - pos).magnitude() - 1.f;
		float zOffset{ -lengthToHitBlock * std::cosf(m_CameraAngle) };
		float yOffset{ lengthToHitBlock * std::sinf(m_CameraAngle) };
		m_pCameraComponent->GetTransform()->Translate(0, yOffset, zOffset);
		m_CameraInOriginalPos = false;

	}
	else if (!m_CameraInOriginalPos)
	{
		float zOffset{ -m_MaxCameraDistance * std::cosf(m_CameraAngle) };
		float yOffset{ m_MaxCameraDistance * std::sinf(m_CameraAngle) };
		m_pCameraComponent->GetTransform()->Translate(0, yOffset, zOffset);
		m_CameraInOriginalPos = true;
	}
}

void Hero::CheckCollisions(const GameContext& gameContext)
{
	auto* controller = m_pController->GetController();
	const auto controllerPos = controller->getPosition();
	const auto foot = controller->getFootPosition();
	float halfHeight = (float)controllerPos.y - (float)foot.y;
	halfHeight *= 0.5f;
	physx::PxVec3 top{ (physx::PxReal)controllerPos.x, (physx::PxReal)controllerPos.y + halfHeight, (physx::PxReal)controllerPos.z };
	const auto forwardDirection = GetTransform()->GetForward();
	const physx::PxVec3 unitDir{ ToPxVec3(forwardDirection) };
	const physx::PxReal maxDist{ m_Radius + 1.f };
	physx::PxRaycastBuffer hit{};
	physx::PxQueryFilterData filterData;
	const physx::PxU32 wallCollGroup{ (UINT)CollisionGroupFlag::Group0 };
	filterData.data.word0 = wallCollGroup;
	
	auto* scene = m_pController->GetScene();
	if (scene->raycast(top, unitDir, maxDist, hit, physx::PxHitFlag::eDEFAULT, filterData))
	{
		auto* hitWall = static_cast<Wall*>(static_cast<RigidBodyComponent*>(hit.block.actor->userData)->GetGameObject());
		if (hitWall->IsClimbable())
		{
			if (gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD) || InputManager::GetThumbstickPosition(true, m_ActiveController).y > 0)
			{
				m_pRunningChannel->setPaused(true);
				m_pClimbedWall = hitWall;
				m_State = CharacterState::WallClimb;
				m_pMesh->SetAnimation(MeshObject::AnimationId::ClimbUpWall);
				m_Velocity.y = 0.f;
				m_Velocity.x = 0.f;
				m_Velocity.z = 0.f;
				m_CurrentMaxVelocity = m_MaxRunVelocity;

				RotateToRayCast(hit);
			}
			else if (m_pController->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) &&
				(gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD) || InputManager::GetThumbstickPosition(true, m_ActiveController).y < 0))
			{
				m_pClimbChannel->setPaused(true);
				m_pMesh->PlayAnimation();
				m_State = CharacterState::Idle;
			}
		}
	}
	
	if (!m_pController->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) &&
		!scene->raycast(top, unitDir, maxDist, hit, physx::PxHitFlag::eDEFAULT, filterData))
	{
		if (m_pClimbedWall &&
			m_State == CharacterState::WallClimb)
		{
			m_pClimbChannel->setPaused(true);
			m_pMesh->SetAnimation(MeshObject::AnimationId::HangToCrouch, false);
			m_ChangingStance = true;
			m_State = CharacterState::Crouching;
			SetIdleAnimation();
			m_changeTime = 1.13333f;
			m_Velocity.x = 0.f;
			m_Velocity.y = 0.f;
			m_Velocity.z = 0.f;
			m_GettingOnWall = true;
		}
		else
		{
			if (!m_ChangingStance)
			{
				m_CurrentMaxVelocity = m_MaxRunVelocity;
				m_State = CharacterState::Jumping;
			}
		}
	}
}

void Hero::RotateToRayCast(const physx::PxRaycastBuffer& hit)
{
	auto normal = hit.getAnyHit(0).normal;
	auto normalXmfloat = ToXMFLOAT3(-normal);
	using namespace DirectX;
	auto playerForward = GetTransform()->GetForward();

	auto cross = XMVector3Cross(XMLoadFloat3(&normalXmfloat),
		XMLoadFloat3(&playerForward));
	XMFLOAT3 crossVector{};
	XMStoreFloat3(&crossVector, cross);
	
	const auto angleBetweenHitAndWorld = XMVector3AngleBetweenVectors(XMLoadFloat3(&normalXmfloat),
		XMLoadFloat3(&playerForward));

	float angleAsXm{};
	XMStoreFloat(&angleAsXm, angleBetweenHitAndWorld);

	if (crossVector.y > 0.f)
	{
		m_TotalYaw -= XMConvertToDegrees(angleAsXm);
		GetTransform()->Rotate(0, m_TotalYaw, 0);
	}
	else
	{
		m_TotalYaw += XMConvertToDegrees(angleAsXm);
		GetTransform()->Rotate(0, m_TotalYaw, 0);
	}
}

void Hero::HandleInput(const GameContext& gameContext)
{
	using namespace DirectX;
	
	switch (m_State)
	{
	case CharacterState::Idle:
		HandleMovement(gameContext);
		break;
	case CharacterState::Crouching:
		HandleMovement(gameContext);
		break;
	case CharacterState::Jumping:
		UpdateAiredVelocity(gameContext);
		break;
	case CharacterState::WallClimb:
		HandleClimbing(gameContext);
		break;
	case CharacterState::Pushing:
		HandlePushing(gameContext);
		break;
	default:
		break;
	}

	if (m_State!= CharacterState::WallClimb && !m_ChangingStance)
		m_Velocity.y += m_JumpVelocity;
	m_pController->Move(XMFLOAT3(m_Velocity.x * gameContext.pGameTime->GetElapsed(), m_Velocity.y * gameContext.pGameTime->GetElapsed(), m_Velocity.z * gameContext.pGameTime->GetElapsed()));
	
	//HANDLE CAMERA ROTATION
	if (m_pCameraComponent->IsActive() &&
		m_State != CharacterState::WallClimb &&
		m_State != CharacterState::Pushing &&
		!m_ChangingStance)
	{
		auto look = XMFLOAT2(0, 0);
		if (InputManager::IsMouseButtonDown(VK_LBUTTON))
		{
			const auto mouseMove = InputManager::GetMouseMovement();
			look.x = static_cast<float>(mouseMove.x);
		}
		
		const auto joystickMovement = InputManager::GetThumbstickPosition(false, m_ActiveController);

		if (joystickMovement.x != 0)
			look.x = joystickMovement.x * 2;

		m_TotalYaw += look.x * m_RotationSpeed;
		m_pController->GetTransform()->Rotate(0, m_TotalYaw, 0);
	}
}

void Hero::HandleMovement(const GameContext& gameContext)
{
	using namespace DirectX;

	if (m_ChangingStance)
	{
		m_changeTime -= gameContext.pGameTime->GetElapsed();
		if (m_changeTime <= 0.f)
		{
			m_ChangingStance = false;
			if (m_GettingOnWall)
			{
				auto forward = GetTransform()->GetForward();
				auto up = GetTransform()->GetUp();
				auto translateForward = DirectX::XMLoadFloat3(&forward);
				auto translateUp = DirectX::XMLoadFloat3(&up);
				translateForward *= 5.f;
				translateUp *= 3.5f;
				const auto onWallTranslation = translateForward + translateUp;
				auto playerPos = GetTransform()->GetPosition();
				const auto playerPosVec = DirectX::XMLoadFloat3(&playerPos);
				GetTransform()->Translate(playerPosVec + onWallTranslation);
				SetIdleAnimation();
				m_GettingOnWall = false;
				return;
			}
		}
		SetIdleAnimation();
		return;
	}


	auto* controller = m_pController->GetController();
	auto controllerPos = controller->getPosition();
	physx::PxVec3 top{ (physx::PxReal)controllerPos.x, (physx::PxReal)controllerPos.y, (physx::PxReal)controllerPos.z };
	const auto forwardDirection = GetTransform()->GetForward();
	const physx::PxVec3 unitDir{ forwardDirection.x,  forwardDirection.y ,forwardDirection.z };
	const physx::PxReal maxDist{ m_Radius + 1.f };
	physx::PxRaycastBuffer hit{};
	physx::PxQueryFilterData filterData;
	physx::PxU32 pillarCollGroup{ (UINT)CollisionGroupFlag::Group2 };
	filterData.data.word0 = pillarCollGroup;

	auto* scene = m_pController->GetScene();
	m_TouchingPillar = scene->raycast(top, unitDir, maxDist, hit, physx::PxHitFlag::eDEFAULT, filterData);
	if (m_TouchingPillar && gameContext.pInput->IsActionTriggered(CharacterMovement::INTERACT))
	{
		m_pPushedPillar = static_cast<Pillar*>(static_cast<RigidBodyComponent*>(hit.block.actor->userData)->GetGameObject());
		m_State = CharacterState::Pushing;
		m_pMesh->SetAnimation(MeshObject::AnimationId::PushObject);
		RotateToRayCast(hit);
		return;
	}

	if (gameContext.pInput->IsActionTriggered(CharacterMovement::JUMP))
	{
		m_pRunningChannel->setPaused(true);
		m_JumpVelocity = 0.f;
		m_Velocity.y = -0.01f;
		m_Velocity.x = 0;
		m_Velocity.z = 0;
		m_pMesh->SetAnimation(MeshObject::AnimationId::JumpUp, false);
		m_State = CharacterState::Jumping;
		m_changeTime = 0.5f;
		m_ChangingStance = true;
		return;
	}

	if (gameContext.pInput->IsActionTriggered(CharacterMovement::CROUCH))
	{
		switch (m_State)
		{
		case CharacterState::Idle:
			m_CurrentMaxVelocity = m_MaxRunVelocity / 2.f;
			m_pMesh->SetAnimation(MeshObject::AnimationId::StandingToCrouch, false);
			m_State = CharacterState::Crouching;
			m_changeTime = 1.f;
			m_ChangingStance = true;
			break;
		case CharacterState::Crouching:
			m_CurrentMaxVelocity = m_MaxRunVelocity;
			m_pMesh->SetAnimation(MeshObject::AnimationId::CrouchToStand, false);
			m_State = CharacterState::Idle;
			m_changeTime = 1.f;
			m_ChangingStance = true;
			break;
		default:
			break;
		}
		m_Velocity.x = 0;
		m_Velocity.z = 0;
		return;
	}

	//HANDLE MOVEMENT
	auto move = DirectX::XMFLOAT2(0, 0);
	if (GetMovementDirection(gameContext, move))
	{
		SetMovementAnimation(move);
		m_pRunningChannel->setPaused(false);
	}
	else
	{
		m_pRunningChannel->setPaused(true);
		SetIdleAnimation();
		m_Velocity.x = 0;
		m_Velocity.z = 0;
	}

	m_Velocity.y = -0.01f;
}

void Hero::UpdateAiredVelocity(const GameContext& gameContext)
{	
	if (m_ChangingStance)
	{
		m_changeTime -= gameContext.pGameTime->GetElapsed();
		if (m_changeTime <= 0.f)
		{
			auto* pSoundSystem = SoundManager::GetInstance()->GetSystem();
			pSoundSystem->playSound(m_pJumpSound, NULL, false , &m_pJumpChannel);

			m_pJumpChannel->setVolume(0.7f);
			m_pJumpChannel->setPaused(false);
			m_JumpVelocity = 0.f;
			m_Velocity.y = 100.f;
			m_ChangingStance = false;

			auto move = DirectX::XMFLOAT2(0, 0);
			GetMovementDirection(gameContext, move);
		}
		return;
	}


	if (!m_pController->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		m_pMesh->SetAnimation(MeshObject::AnimationId::FallLoop);
		m_JumpVelocity -= m_JumpAcceleration * gameContext.pGameTime->GetElapsed();
	}
	else
		m_State = CharacterState::Idle;
}

void Hero::HandleClimbing(const GameContext& gameContext)
{
	float movement;
	if (GetWallMovement(gameContext, movement))
	{
		m_pClimbChannel->setPaused(false);
		m_pMesh->PlayAnimation();
		SetAnimationWallClimb(movement);
	}
	else
	{
		m_pClimbChannel->setPaused(true);
		m_Velocity.x = 0;
		m_Velocity.y = 0;
		m_Velocity.z = 0;
		m_pMesh->PauseAnimation();
	}
	
}

void Hero::HandlePushing(const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(CharacterMovement::INTERACT))
	{
		m_pPillarPushChannel->setPaused(true);
		m_pMesh->PlayAnimation();
		m_State = CharacterState::Idle;
		SetIdleAnimation();
		return; 
	}
	
	float movement;
	bool pillarGrounded = true;
	if (GetPushMovement(gameContext, movement, pillarGrounded))
	{
		m_pPillarPushChannel->setPaused(false);
		m_pRunningChannel->setPaused(false);
		m_pMesh->PlayAnimation();
		SetAnimationPush(movement);
	}
	else
	{
		m_pPillarPushChannel->setPaused(true);
		m_pRunningChannel->setPaused(true);
		m_Velocity.x = 0;
		m_Velocity.y = 0;
		m_Velocity.z = 0;
		m_pMesh->PauseAnimation();
	}
	if (!pillarGrounded)
	{
		m_State = CharacterState::Idle;
		SetIdleAnimation();
		m_pMesh->PlayAnimation();
	}
}

bool Hero::GetMovementDirection(const GameContext& gameContext, DirectX::XMFLOAT2& move)
{
	using namespace DirectX;

	move.y = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD) ? 1.0f : 0.0f;
	if (move.y == 0) move.y = -(gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD) ? 1.0f : 0.0f);
	if (move.y == 0) move.y = InputManager::GetThumbstickPosition(true, m_ActiveController).y;

	move.x = gameContext.pInput->IsActionTriggered(CharacterMovement::RIGHT) ? 1.0f : 0.0f;
	if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(CharacterMovement::LEFT) ? 1.0f : 0.0f);
	if (move.x == 0) move.x = InputManager::GetThumbstickPosition(true, m_ActiveController).x;

	if (move.x != 0 || move.y != 0)
	{
		const XMVECTOR forward = DirectX::XMLoadFloat3(&GetTransform()->GetForward());
		XMVECTOR forwardMovement = forward * move.y;
		const XMVECTOR right = DirectX::XMLoadFloat3(&GetTransform()->GetRight());
		XMVECTOR rightMovement = right * move.x;

		auto movementDirectionVector = forwardMovement + rightMovement;
		movementDirectionVector = XMVector3Normalize(movementDirectionVector);

		m_RunVelocity += m_RunAcceleration * gameContext.pGameTime->GetElapsed();
		Clamp(m_RunVelocity, m_CurrentMaxVelocity, 0.f);

		XMVECTOR velocityVector = movementDirectionVector * m_RunVelocity;

		float velocityY = m_Velocity.y;
		DirectX::XMStoreFloat3(&m_Velocity, velocityVector);
		m_Velocity.y = velocityY;

		return true;
	}

	return false;
}

bool Hero::GetWallMovement(const GameContext& gameContext, float& move)
{
	using namespace DirectX;

	move = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD) ? 1.0f : 0.0f;
	if (move == 0) move = -(gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD) ? 1.0f : 0.0f);
	if (move == 0) move = InputManager::GetThumbstickPosition(true, m_ActiveController).y;

	if (move != 0)
	{
		const XMVECTOR up = DirectX::XMLoadFloat3(&GetTransform()->GetUp());
		XMVECTOR upMovement = up * move;

		auto movementDirectionVector = upMovement;
		movementDirectionVector = XMVector3Normalize(movementDirectionVector);

		m_ClimbVelocity += m_ClimbAcceleration * gameContext.pGameTime->GetElapsed();
		Clamp(m_ClimbVelocity, m_MaxClimbVelocity, 0.f);

		XMVECTOR velocityVector = movementDirectionVector * m_ClimbVelocity;
		
		DirectX::XMStoreFloat3(&m_Velocity, velocityVector);
		return true;
	}

	return false;
}

bool Hero::GetPushMovement(const GameContext& gameContext, float& move, bool& pillarGrounded)
{
	using namespace DirectX;

	auto forwardPlayer = GetTransform()->GetForward();
	
	move = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD) ? 1.0f : 0.0f;
	if (move == 0) move = -(gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD) ? 1.0f : 0.0f);
	if (move == 0) move = InputManager::GetThumbstickPosition(true, m_ActiveController).y;

	if (move != 0)
	{
		const XMVECTOR forward = DirectX::XMLoadFloat3(&forwardPlayer);
		XMVECTOR forwardMovement = forward * move;

		auto movementDirectionVector = forwardMovement;
		movementDirectionVector = XMVector3Normalize(movementDirectionVector);

		m_PushVelocity += m_PushAcceleration * gameContext.pGameTime->GetElapsed();
		if (move>0)
			Clamp(m_PushVelocity, m_MaxPushVelocity, 0.f);
		else
			Clamp(m_PushVelocity, m_MaxPullVelocity, 0.f);

		
		XMVECTOR velocityVector = movementDirectionVector * m_PushVelocity;

		DirectX::XMStoreFloat3(&m_Velocity, velocityVector);
		const physx::PxVec3 pillarVelocity{ m_Velocity.x, m_Velocity.y, m_Velocity.z };
		float addedWidth = 0.f;
		if (move < 0)
			addedWidth = m_Radius * 2.f;
		if (m_pPushedPillar->Push(gameContext, pillarVelocity, GetTransform()->GetRight(), forwardPlayer, pillarGrounded, addedWidth))
			return true;
		

		m_Velocity.x = 0;
		m_Velocity.y = 0;
		m_Velocity.z = 0;
		return false;
	}
	return m_pPushedPillar->Push(gameContext, physx::PxVec3(0, 0, 0), GetTransform()->GetRight(), forwardPlayer, pillarGrounded);
}

void Hero::SetMovementAnimation(const DirectX::XMFLOAT2& move)
{
	switch (m_State)
	{
	case CharacterState::Idle:
		SetAnimationStanding(move);
		break;
	case CharacterState::Crouching:
		SetAnimationCrouched(move);
		break;
	default:
		break;
	}
}

void Hero::SetAnimationStanding(const DirectX::XMFLOAT2& move)
{
	if (move.y > 0)
	{
		m_pMesh->SetAnimation(MeshObject::AnimationId::Running);
		return;
	}
	if (move.y < 0)
	{
		m_pMesh->SetAnimation(MeshObject::AnimationId::RunningBackwards);
		return;
	}

	if (move.x > 0)
		m_pMesh->SetAnimation(MeshObject::AnimationId::RightStrafe);
	else if (move.x < 0)
		m_pMesh->SetAnimation(MeshObject::AnimationId::LeftStrafe);
}

void Hero::SetAnimationCrouched(const DirectX::XMFLOAT2& move)
{
	if (move.y > 0)
	{
		m_pMesh->SetAnimation(MeshObject::AnimationId::CrouchWalkForward);
		return;
	}
	if (move.y < 0)
	{
		m_pMesh->SetAnimation(MeshObject::AnimationId::CrouchWalkBack);
		return;
	}

	if (move.x > 0)
		m_pMesh->SetAnimation(MeshObject::AnimationId::CrouchWalkRight);
	else if (move.x < 0)
		m_pMesh->SetAnimation(MeshObject::AnimationId::CrouchWalkLeft);
}

void Hero::SetAnimationWallClimb(float movement) const
{
	m_pMesh->SetAnimation(MeshObject::AnimationId::ClimbUpWall, true, movement < 0.f);
}

void Hero::SetAnimationPush(float movement) const
{
	if (movement > 0)
		m_pMesh->SetAnimation(MeshObject::AnimationId::PushObject);
	else if (movement < 0)
		m_pMesh->SetAnimation(MeshObject::AnimationId::PullObject);
	//m_pMesh->SetAnimation(MeshObject::AnimationId::PushObject, true, movement < 0.f);
}

void Hero::SetIdleAnimation() const
{
	switch (m_State)
	{
	case CharacterState::Idle:
		m_pMesh->SetAnimation(MeshObject::AnimationId::Idle);
		break;
	case CharacterState::Crouching:
		m_pMesh->SetAnimation(MeshObject::AnimationId::CrouchIdle);
		break;
	default:
		break;
	}
}
