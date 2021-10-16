#include "stdafx.h"
#include "BaseLevelScene.h"
#include "PhysxManager.h"
#include "SoundManager.h"
#include "Components.h"
#include "ContentManager.h"
#include "Hero.h"
#include "OverlordGame.h"
#include "PauseMenu.h"
#include "PostSepia.h"
#include "SceneManager.h"
#include "SpriteFont.h"
#include "TextRenderer.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "GameOverScreen.h"
#include "GameScene.h"

BaseLevelScene::BaseLevelScene(std::wstring sceneName)
	: GameScene(sceneName)
{
}

void BaseLevelScene::Initialize()
{
	m_TimeRemaining = m_MaxTime;

	const auto gameContext = GetGameContext();

	//GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);
	gameContext.pShadowMapper->SetLight({ 0.f,100.f,0.f }, { 0.f, -0.95f, 0.1f });

	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Immortal_16.fnt");

	//Create materials
	auto* pFloorMat = new DiffuseMaterial_Shadow();
	pFloorMat->SetDiffuseTexture(L"Resources/Textures/wall.png");
	pFloorMat->SetScale(DirectX::XMFLOAT2(40, 70));
	pFloorMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(pFloorMat, 1);

	auto* pPillarMat = new DiffuseMaterial_Shadow();
	pPillarMat->SetDiffuseTexture(L"Resources/Textures/marble.png");
	pPillarMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(pPillarMat, 2);

	auto* pTrophyMat = new DiffuseMaterial_Shadow();
	pTrophyMat->SetDiffuseTexture(L"Resources/Textures/trophy_diffuse.png");
	pTrophyMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(pTrophyMat, 3);

	auto* pRoughWallMat = new DiffuseMaterial_Shadow();
	pRoughWallMat->SetDiffuseTexture(L"Resources/Textures/RoughWall.png");
	pRoughWallMat->SetScale(DirectX::XMFLOAT2(4, 4));
	pRoughWallMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(pRoughWallMat, 4);

	FMOD_RESULT fmodResult;
	auto* pSoundSystem = SoundManager::GetInstance()->GetSystem();
	
	//Lose
	fmodResult = pSoundSystem->createSound("Resources/Audio/LevelLose.mp3", FMOD_LOOP_OFF, NULL, &m_pLoseSound);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	//Win
	fmodResult = pSoundSystem->createSound("Resources/Audio/LevelComplete.mp3", FMOD_LOOP_OFF, NULL, &m_pWinSound);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	DirectX::XMFLOAT3 initialPosition{ 0,0,0 };
	//Character
	m_pHero = new Hero();
	AddChild(m_pHero);
	m_ObjectsVector.push_back(InitValue(m_pHero, initialPosition));
	
	//Non climbable Walls
	CreateMesh(L"Resources/Meshes/Environment/staticWall", DirectX::XMFLOAT3(-40.5f, 0.f, 20), DirectX::XMFLOAT3(2.f, 2.f, 2),
		DirectX::XMFLOAT3(1, 0, 0), 90.f, 1);

	CreateMesh(L"Resources/Meshes/Environment/staticWall", DirectX::XMFLOAT3(40.5f, 0.f, 20), DirectX::XMFLOAT3(2.f, 2.f, 2),
		DirectX::XMFLOAT3(1, 0, 0), 90.f, 1);

	CreateMesh(L"Resources/Meshes/Environment/backStaticWall", DirectX::XMFLOAT3(0.f, 0.f, -10.5f), DirectX::XMFLOAT3(2, 2.f, 2),
		DirectX::XMFLOAT3(1, 0, 0), 0.f, 1, true);

	CreateMesh(L"Resources/Meshes/Environment/endStaticWall", DirectX::XMFLOAT3(0.f, 0.f, 92.6f), DirectX::XMFLOAT3(2.5f, 1.75f, 2),
		DirectX::XMFLOAT3(1, 0, 0), 0.f, 1, true);;

	//Create Floor
	CreateMesh(L"Resources/Meshes/Environment/floor", DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1),
		DirectX::XMFLOAT3(1, 0, 0), 90.f, 1);

	//Post processing
	AddPostProcessingEffect(new PostSepia(0.8f, 0.6f, 0.3f));

	//Create pause menu
	m_pPauseMenu = new PauseMenu();
	AddChild(m_pPauseMenu);

	//Level end trigger
	initialPosition = DirectX::XMFLOAT3(31.f, 5.f, 91.f);
	CreateFinishLevelTrigger(15.f, 30.f, 10.f, initialPosition);
}

void BaseLevelScene::Update()
{
	if (m_LevelFinished)
	{
		m_LevelFinishedCooldown -= GetGameContext().pGameTime->GetElapsed();
		if (m_LevelFinishedCooldown <= 0.f)
		{
			m_LevelFinished = false;
			m_LevelFinishedCooldown = 1.f;
		}
	}
	
	m_Paused = m_pHero->IsPaused();
	m_pPauseMenu->GetComponent<SpriteComponent>()->SetDraw(m_Paused);

	if (!m_Paused)
	{
		float elapsedTime = GetGameContext().pGameTime->GetElapsed();

		m_TimeRemaining -= elapsedTime;
		if (m_TimeRemaining <= 0.f)
		{
			m_LevelFinished = true;
			auto* pSoundSystem = SoundManager::GetInstance()->GetSystem();
			pSoundSystem->playSound(m_pLoseSound, NULL, false, &m_pLoseChannel);

			m_pLoseChannel->setVolume(0.7f);
			m_pLoseChannel->setPaused(false);

			m_pHero->StopSounds();
			SceneManager::GetInstance()->SetActiveGameScene(L"GameOverScreen");
			GameOverScreen* gameOverScreen = dynamic_cast<GameOverScreen*>(SceneManager::GetInstance()->GetScene(L"GameOverScreen"));
			if (gameOverScreen)
				gameOverScreen->SetResultText(L"Game Over...");
		}

		const float rotationPerSecond = 10.f;
		m_CurrentTrophyRotation += rotationPerSecond * elapsedTime;

		m_pTrophyObject->GetTransform()->Rotate(-90, m_CurrentTrophyRotation, 0);
	}
	else if (m_pHero->ShouldResetScene())
	{
		m_pHero->SetResetScene(false);
		ResetScene();
	}
}

void BaseLevelScene::Draw()
{
	if (!m_Paused)
		DrawHud();
}

void BaseLevelScene::SceneActivated()
{
	ResetScene();
}

void BaseLevelScene::DrawHud()
{
	auto gameContext = GetGameContext();

	const DirectX::XMFLOAT4 textColor{ 87.f / 255.f, 28.f / 255.f, 28.f / 255.f, 1 };
	const DirectX::XMFLOAT4 brightText{ textColor.x * 3.f, textColor.y * 3.f, textColor.z * 3.f, 1 };

	const int minutesRemaining = static_cast<int>(m_TimeRemaining) / 60;
	const int secondsRemaining = static_cast<int>(m_TimeRemaining) % 60;

	std::wstring secondsString;
	if (secondsRemaining < 10)
		secondsString = L"0" + std::to_wstring(secondsRemaining);
	else
		secondsString = std::to_wstring(secondsRemaining);

	std::wstring timeRemainingString{ L"Time remaining: " + std::to_wstring(minutesRemaining) + L":" + secondsString };

	TextRenderer::GetInstance()->DrawText(m_pFont, timeRemainingString, DirectX::XMFLOAT2(20, 20), brightText);

	std::wstring fpsString{ L"FPS: " + std::to_wstring(gameContext.pGameTime->GetFPS()) };
	
	TextRenderer::GetInstance()->DrawText(m_pFont, fpsString, DirectX::XMFLOAT2(20, 50), brightText);
	
	if (m_pHero->IsTouchingPillar())
	{
		std::wstring interactText{ L"'E' to interact" };

		const auto windowSettings = OverlordGame::GetGameSettings().Window;
		const auto widthPos = (float)windowSettings.Width / 2.f - 100.f;

		TextRenderer::GetInstance()->DrawText(m_pFont, interactText, DirectX::XMFLOAT2(widthPos, (float)windowSettings.Height - 100.f), brightText);
	}
}

void BaseLevelScene::CreateMesh(const std::wstring& filepath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& rotation, float rotationDegree, int materialId, bool isTriangleMesh)
{
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	const float rotationInRadian = rotationDegree / 180 * 3.14f;

	//Create object
	GameObject* obj = new GameObject();

	//Translate object
	obj->GetTransform()->Translate(pos);

	const physx::PxQuat physxQuaternion{ rotationInRadian,physx::PxVec3{rotation.x,rotation.y,rotation.z} };
	//Apply colliders
	RigidBodyComponent* pRigidbody = new RigidBodyComponent(true);
	pRigidbody->SetCollisionGroup(CollisionGroupFlag::Group3);
	ColliderComponent* pColliderComp = nullptr;
	auto* pMat = physX->createMaterial(0.5f, 0.5f, 0.f);
	const physx::PxMeshScale physxScale(physx::PxVec3{ scale.x,scale.y,scale.z }, physxQuaternion);
	if (isTriangleMesh)
	{
		physx::PxTriangleMesh* triangleMesh = ContentManager::Load<physx::PxTriangleMesh>(filepath + L".ovpt");
		std::shared_ptr<physx::PxGeometry> geomMesh{ new physx::PxTriangleMeshGeometry(triangleMesh,physxScale) };
		pColliderComp = new ColliderComponent(geomMesh, *pMat, physx::PxTransform(physxQuaternion));
	}
	else
	{
		physx::PxConvexMesh* convexMesh = ContentManager::Load<physx::PxConvexMesh>(filepath + L".ovpc");
		std::shared_ptr<physx::PxGeometry> geomMesh{ new physx::PxConvexMeshGeometry(convexMesh,physxScale) };
		pColliderComp = new ColliderComponent(geomMesh, *pMat, physx::PxTransform(physxQuaternion));
	}


	obj->AddComponent(pRigidbody);
	obj->AddComponent(pColliderComp);
	AddChild(obj);

	//Create Visuals
	auto* model = new GameObject();
	auto* modelComponent = new ModelComponent(filepath + L".ovm");
	model->AddComponent(modelComponent);
	modelComponent->SetMaterial(materialId);

	//Transform Visuals
	const DirectX::XMFLOAT4 XMQuat{ physxQuaternion.x,physxQuaternion.y,physxQuaternion.z,physxQuaternion.w };
	model->GetTransform()->Rotate(DirectX::XMLoadFloat4(&XMQuat));
	model->GetTransform()->Scale(scale);

	obj->AddChild(model);
}

void BaseLevelScene::ResetScene()
{
	m_pHero->ResetAnimations();
	m_pHero->SetPause(false);
	
	for (auto object : m_ObjectsVector)
		object.object->GetTransform()->Translate(object.initialPosition);

	m_TimeRemaining = m_MaxTime;
}

void BaseLevelScene::CreateFinishLevelTrigger(float width, float height, float depth, DirectX::XMFLOAT3 pos)
{
	auto* physX = PhysxManager::GetInstance()->GetPhysics();
	auto* pMat = physX->createMaterial(0.5f, 0.5f, 0.f);

	//Create object
	GameObject* obj = new GameObject();

	auto* pRigidBody = new RigidBodyComponent(true);
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group5);
	obj->AddComponent(pRigidBody);
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxBoxGeometry(width / 2, height / 2, depth / 2));
	auto* pCollider = new ColliderComponent(geom, *pMat);
	pCollider->EnableTrigger(true);

	auto colliderCallback = [this](GameObject*, GameObject* pOther, GameObject::TriggerAction)
	{
		if (!pOther) return;

		Hero* pHero = dynamic_cast<Hero*>(pOther);
		if (pHero&& !m_LevelFinished)
		{
			m_LevelFinished = true;
			this->ResetScene();

			auto* pSoundSystem = SoundManager::GetInstance()->GetSystem();
			pSoundSystem->playSound(m_pWinSound, NULL, false, &m_pWinChannel);

			m_pWinChannel->setVolume(0.7f);
			m_pWinChannel->setPaused(false);

			m_pHero->StopSounds();
			auto* pNextScene = SceneManager::GetInstance()->NextScene();
			GameOverScreen* gameOverScreen = dynamic_cast<GameOverScreen*>(pNextScene);
			if (gameOverScreen)
				gameOverScreen->SetResultText(L"Congratulations!");
		}
	};
	obj->SetOnTriggerCallBack(colliderCallback);
	obj->AddComponent(pCollider);

	//Create Trophy visual
	m_pTrophyObject = new GameObject();
	auto* modelComponent = new ModelComponent(L"Resources/Meshes/Environment/trophy.ovm");
	m_pTrophyObject->AddComponent(modelComponent);
	m_pTrophyObject->GetTransform()->Rotate(-90, 0, 0);
	modelComponent->SetMaterial(3);
	obj->AddChild(m_pTrophyObject);

	//Create particle effect
	auto* pParticleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/star.png", 30);
	pParticleEmitter->SetVelocity(DirectX::XMFLOAT3(0, 6.0f, 0));
	pParticleEmitter->SetMinSize(1.0f);
	pParticleEmitter->SetMaxSize(2.0f);
	pParticleEmitter->SetMinEnergy(7.f);
	pParticleEmitter->SetMaxEnergy(10.0f);
	pParticleEmitter->SetMinSizeGrow(1.5f);
	pParticleEmitter->SetMaxSizeGrow(3.5f);
	pParticleEmitter->SetMinEmitterRange(2.f);
	pParticleEmitter->SetMaxEmitterRange(5.f);
	pParticleEmitter->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 0.6f));
	obj->AddComponent(pParticleEmitter);

	obj->GetTransform()->Translate(pos);
	AddChild(obj);
}
