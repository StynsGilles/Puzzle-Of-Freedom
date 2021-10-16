#include "stdafx.h"
#include "TutorialLevel.h"
#include "Hero.h"
#include "Components.h"
#include "ContentManager.h"
#include "PauseMenu.h"
#include "PhysxManager.h"
#include "PostSepia.h"
#include "SceneManager.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Wall.h"
#include "Pillar.h"
#include "SpriteFont.h"
#include "OverlordGame.h"
#include "SoundManager.h"
#include "TextRenderer.h"

TutorialLevel::TutorialLevel()
	:BaseLevelScene(L"TutorialLevel")
{
}

void TutorialLevel::Initialize()
{
	
	BaseLevelScene::Initialize();

	//Create Level
	//Climbable walls
	const float heightClimbableWalls = 50.f;

	DirectX::XMFLOAT3 initialPosition = DirectX::XMFLOAT3(-41.5f, heightClimbableWalls / 2.f, 30.f);
	GameObject* pClimbableWallZero = new Wall(true, 14.5f, heightClimbableWalls, DirectX::XMFLOAT3(2.535f, 2.535f, 2.535f));
	pClimbableWallZero->GetTransform()->Translate(initialPosition);
	AddChild(pClimbableWallZero);
	
	initialPosition = DirectX::XMFLOAT3(-27.f, heightClimbableWalls / 2.f, 30.f);
	GameObject* pFirstClimbableWall = new Wall(true, 14.5f, heightClimbableWalls, DirectX::XMFLOAT3(2.535f, 2.535f, 2.535f));
	pFirstClimbableWall->GetTransform()->Translate(initialPosition);
	AddChild(pFirstClimbableWall);
	
	initialPosition = DirectX::XMFLOAT3(-12.5f, heightClimbableWalls / 2.f, 30.f);
	GameObject* pSecondClimbableWall = new Wall(true, 14.5f, heightClimbableWalls, DirectX::XMFLOAT3(2.535f, 2.535f, 2.535f));
	pSecondClimbableWall->GetTransform()->Translate(initialPosition);
	AddChild(pSecondClimbableWall);
	
	initialPosition = DirectX::XMFLOAT3(2.f, heightClimbableWalls / 2.f, 30.f);
	GameObject* pThirdClimbableWall = new Wall(true, 14.5f, heightClimbableWalls, DirectX::XMFLOAT3(2.535f, 2.535f, 2.535f));
	pThirdClimbableWall->GetTransform()->Translate(initialPosition);
	AddChild(pThirdClimbableWall);
	
	initialPosition = DirectX::XMFLOAT3(16.5f, heightClimbableWalls / 2.f, 30.f);
	GameObject* pFourthClimbableWall = new Wall(true, 14.5f, heightClimbableWalls, DirectX::XMFLOAT3(2.535f, 2.535f, 2.535f));
	pFourthClimbableWall->GetTransform()->Translate(initialPosition);
	AddChild(pFourthClimbableWall);
	
	initialPosition = DirectX::XMFLOAT3(31.f, heightClimbableWalls / 2.f, 30.f);
	GameObject* pLastClimbableWall = new Wall(true, 14.5f, heightClimbableWalls, DirectX::XMFLOAT3(2.535f, 2.535f, 2.535f));
	pLastClimbableWall->GetTransform()->Translate(initialPosition);
	AddChild(pLastClimbableWall);

	//Pillar
	const float pillarHeight = 40.f;
	const float pillarWidth = 15.f;
	const float pillarUpTranslation = pillarHeight / 2.f;
	
	initialPosition = DirectX::XMFLOAT3(30.f, pillarUpTranslation, 75.f);
	GameObject* pPillar = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar->GetTransform()->Translate(initialPosition);
	AddChild(pPillar);
	m_ObjectsVector.push_back(InitValue(pPillar, initialPosition));
	
	//Triggers (explanation purposes)
	CreateInitialTrigger();
	CreateTopTrigger();
	CreateTreasureFoundTrigger();
}

void TutorialLevel::Update()
{
	BaseLevelScene::Update();

	m_TimeRemaining = m_MaxTime;

	if (!m_Paused)
	{
		for (auto& textAndPos : m_HUDTextVector)
		{
			textAndPos.timeLeft -= GetGameContext().pGameTime->GetElapsed();
			if (textAndPos.timeLeft <= 0.f)
				m_HUDTextVector.erase(std::remove(m_HUDTextVector.begin(), m_HUDTextVector.end(), textAndPos), m_HUDTextVector.end());
		}
	}
}

void TutorialLevel::ResetScene()
{
	BaseLevelScene::ResetScene();

	m_HUDTextVector.clear();
}

void TutorialLevel::DrawHud()
{
	BaseLevelScene::DrawHud();
	
	const DirectX::XMFLOAT4 textColor{ 87.f / 255.f, 28.f / 255.f, 28.f / 255.f, 1 };
	const DirectX::XMFLOAT4 brightText{ textColor.x * 3.f, textColor.y * 3.f, textColor.z * 3.f, 1 };

	for (auto textAndPos : m_HUDTextVector)
		TextRenderer::GetInstance()->DrawText(m_pFont, textAndPos.text, textAndPos.pos, brightText);
}

void TutorialLevel::CreateInitialTrigger()
{
	auto* physX = PhysxManager::GetInstance()->GetPhysics();
	auto* pMat = physX->createMaterial(0.5f, 0.5f, 0.f);

	//Create object
	GameObject* obj = new GameObject();

	auto* pRigidBody = new RigidBodyComponent(true);
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group5);
	obj->AddComponent(pRigidBody);
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxBoxGeometry(90.f / 2, 50.f / 2, 50.f / 2));
	auto* pCollider = new ColliderComponent(geom, *pMat);
	pCollider->EnableTrigger(true);

	auto colliderCallback = [this](GameObject*, GameObject* pOther, GameObject::TriggerAction)
	{
		if (!pOther) return;

		Hero* pHero = dynamic_cast<Hero*>(pOther);
		if (pHero)
		{
			std::wstring text = L"Those walls look climbable, try running up to them...";
			auto it = std::find_if(m_HUDTextVector.begin(), m_HUDTextVector.end(), [text](TextAndPos textAndPos)
				{
					return wcscmp(textAndPos.text.c_str(), text.c_str()) == 0;
				});

			if (it == m_HUDTextVector.end())
			{
				TextAndPos textAndPos;
				textAndPos.text = text;
				textAndPos.pos = DirectX::XMFLOAT2(300.f, 600.f);
				m_HUDTextVector.push_back(textAndPos);

			}
		}
	};
	obj->SetOnTriggerCallBack(colliderCallback);
	obj->AddComponent(pCollider);
	AddChild(obj);
}

void TutorialLevel::CreateTopTrigger()
{
	auto* physX = PhysxManager::GetInstance()->GetPhysics();
	auto* pMat = physX->createMaterial(0.5f, 0.5f, 0.f);

	//Create object
	GameObject* obj = new GameObject();

	auto* pRigidBody = new RigidBodyComponent(true);
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group5);
	obj->AddComponent(pRigidBody);
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxBoxGeometry(90.f / 2, 20.f / 2, 14.5f / 2));
	auto* pCollider = new ColliderComponent(geom, *pMat);
	pCollider->EnableTrigger(true);

	auto colliderCallback = [this](GameObject*, GameObject* pOther, GameObject::TriggerAction)
	{
		if (!pOther) return;

		Hero* pHero = dynamic_cast<Hero*>(pOther);
		if (pHero)
		{
			std::wstring text = L"What's that behind that pillar? Try moving it";
			auto it = std::find_if(m_HUDTextVector.begin(), m_HUDTextVector.end(), [text](TextAndPos textAndPos)
				{
					return wcscmp(textAndPos.text.c_str(), text.c_str()) == 0;
				});

			if (it == m_HUDTextVector.end())
			{
				TextAndPos textAndPos;
				textAndPos.text = text;
				textAndPos.pos = DirectX::XMFLOAT2(300.f, 650.f);
				m_HUDTextVector.push_back(textAndPos);

			}
		}
	};
	obj->SetOnTriggerCallBack(colliderCallback);
	obj->AddComponent(pCollider);
	obj->GetTransform()->Translate(0.f, 60.f, 30.f);
	AddChild(obj);
}

void TutorialLevel::CreateTreasureFoundTrigger()
{
	auto* physX = PhysxManager::GetInstance()->GetPhysics();
	auto* pMat = physX->createMaterial(0.5f, 0.5f, 0.f);

	//Create object
	GameObject* obj = new GameObject();

	auto* pRigidBody = new RigidBodyComponent(true);
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group5);
	obj->AddComponent(pRigidBody);
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxBoxGeometry(12.5f / 2, 20.f / 2, 12.5f / 2));
	auto* pCollider = new ColliderComponent(geom, *pMat);
	pCollider->EnableTrigger(true);

	auto colliderCallback = [this](GameObject*, GameObject* pOther, GameObject::TriggerAction)
	{
		if (!pOther) return;

		Hero* pHero = dynamic_cast<Hero*>(pOther);
		if (pHero)
		{
			std::wstring text = L"You found some treasure! collect it to go on to the next level";
			auto it = std::find_if(m_HUDTextVector.begin(), m_HUDTextVector.end(), [text](TextAndPos textAndPos)
				{
					return wcscmp(textAndPos.text.c_str(), text.c_str()) == 0;
				});

			if (it == m_HUDTextVector.end())
			{
				TextAndPos textAndPos;
				textAndPos.text = text;
				textAndPos.pos = DirectX::XMFLOAT2(250.f, 600.f);
				m_HUDTextVector.push_back(textAndPos);

			}
		}
	};
	obj->SetOnTriggerCallBack(colliderCallback);
	obj->AddComponent(pCollider);
	obj->GetTransform()->Translate(30.f, 10.f, 75.f);
	AddChild(obj);
}