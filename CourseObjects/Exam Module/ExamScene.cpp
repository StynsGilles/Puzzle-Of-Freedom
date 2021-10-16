#include "stdafx.h"
#include "ExamScene.h"
#include "SoundManager.h"
#include "Components.h"
#include "ContentManager.h"
#include "Wall.h"
#include "Pillar.h"
#include "SceneManager.h"

ExamScene::ExamScene()
	: BaseLevelScene(L"ExamScene")
{
}

void ExamScene::Initialize()
{
	BaseLevelScene::Initialize();

	//Audio
	//Background Sound
	FMOD_RESULT fmodResult;
	auto* pSoundSystem = SoundManager::GetInstance()->GetSystem();
	FMOD::Sound* pSound;
	FMOD::Channel* pChannel;
	fmodResult = pSoundSystem->createSound("Resources/Audio/AncientArabianMusic.mp3", FMOD_LOOP_NORMAL, NULL, &pSound);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	fmodResult = pSoundSystem->playSound(pSound, NULL, false, &pChannel);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	pChannel->setVolume(0.5f);

	//Create Level
	//Climbable walls
	const float heightClimbableWalls = 50.f;

	DirectX::XMFLOAT3 initialPosition = DirectX::XMFLOAT3(-23.5f, heightClimbableWalls / 2.f, 37.5f);
	GameObject* pFirstClimbableWall = new Wall(true, 29.5f, heightClimbableWalls, DirectX::XMFLOAT3(5.07f, 2.535f, 5.07f));
	pFirstClimbableWall->GetTransform()->Translate(initialPosition);
	AddChild(pFirstClimbableWall);

	initialPosition = DirectX::XMFLOAT3(31.f, heightClimbableWalls / 2.f, 45.f);
	GameObject* pSecondClimbableWall = new Wall(true, 14.5f, heightClimbableWalls, DirectX::XMFLOAT3(2.535f, 2.535f, 2.535f));
	pSecondClimbableWall->GetTransform()->Translate(initialPosition);
	AddChild(pSecondClimbableWall);

	//Pillars
	const float pillarHeight = 40.f;
	const float pillarWidth = 15.f;
	const float pillarUpTranslation = pillarHeight / 2.f;
	
	//First row
	initialPosition = DirectX::XMFLOAT3(-15.f, pillarUpTranslation, 0.f);
	GameObject* pPillar1 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar1->GetTransform()->Translate(initialPosition);
	AddChild(pPillar1);
	m_ObjectsVector.push_back(InitValue(pPillar1, initialPosition));

	initialPosition = DirectX::XMFLOAT3(-30.f, pillarUpTranslation, 0.f);
	GameObject* pPillar2 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar2->GetTransform()->Translate(initialPosition);
	AddChild(pPillar2);
	m_ObjectsVector.push_back(InitValue(pPillar2, initialPosition));

	initialPosition = DirectX::XMFLOAT3(15.f, pillarUpTranslation, 0.f);
	GameObject* pPillar3 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar3->GetTransform()->Translate(initialPosition);
	AddChild(pPillar3);
	m_ObjectsVector.push_back(InitValue(pPillar3, initialPosition));

	//Second row
	initialPosition = DirectX::XMFLOAT3(-30.f, pillarUpTranslation, 15.f);
	GameObject* pPillar4 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar4->GetTransform()->Translate(initialPosition);
	AddChild(pPillar4);
	m_ObjectsVector.push_back(InitValue(pPillar4, initialPosition));

	initialPosition = DirectX::XMFLOAT3(-15.f, pillarUpTranslation, 15.f);
	GameObject* pPillar5 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar5->GetTransform()->Translate(initialPosition);
	AddChild(pPillar5);
	m_ObjectsVector.push_back(InitValue(pPillar5, initialPosition));

	initialPosition = DirectX::XMFLOAT3(0, pillarUpTranslation, 15.f);
	GameObject* pPillar6 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar6->GetTransform()->Translate(initialPosition);
	AddChild(pPillar6);
	m_ObjectsVector.push_back(InitValue(pPillar6, initialPosition));

	initialPosition = DirectX::XMFLOAT3(15.f, pillarUpTranslation, 15.f);
	GameObject* pPillar7 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar7->GetTransform()->Translate(initialPosition);
	AddChild(pPillar7);
	m_ObjectsVector.push_back(InitValue(pPillar7, initialPosition));

	initialPosition = DirectX::XMFLOAT3(30.f, pillarUpTranslation, 15.f);
	GameObject* pPillar8 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar8->GetTransform()->Translate(initialPosition);
	AddChild(pPillar8);
	m_ObjectsVector.push_back(InitValue(pPillar8, initialPosition));

	//Third row
	initialPosition = DirectX::XMFLOAT3(30.f, pillarUpTranslation, 30.f);
	GameObject* pPillar9 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar9->GetTransform()->Translate(initialPosition);
	AddChild(pPillar9);
	m_ObjectsVector.push_back(InitValue(pPillar9, initialPosition));

	//Fourth row
	initialPosition = DirectX::XMFLOAT3(0.f, pillarUpTranslation, 45.f);
	GameObject* pPillar10 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar10->GetTransform()->Translate(initialPosition);
	AddChild(pPillar10);
	m_ObjectsVector.push_back(InitValue(pPillar10, initialPosition));

	//Fifth row
	initialPosition = DirectX::XMFLOAT3(-30.f, pillarUpTranslation, 60.f);
	GameObject* pPillar11 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar11->GetTransform()->Translate(initialPosition);
	AddChild(pPillar11);
	m_ObjectsVector.push_back(InitValue(pPillar11, initialPosition));

	initialPosition = DirectX::XMFLOAT3(-15.f, pillarUpTranslation, 60.f);
	GameObject* pPillar12 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar12->GetTransform()->Translate(initialPosition);
	AddChild(pPillar12);
	m_ObjectsVector.push_back(InitValue(pPillar12, initialPosition));

	initialPosition = DirectX::XMFLOAT3(15.f, pillarUpTranslation, 60.f);
	GameObject* pPillar13 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar13->GetTransform()->Translate(initialPosition);
	AddChild(pPillar13);
	m_ObjectsVector.push_back(InitValue(pPillar13, initialPosition));

	//Sixth row
	initialPosition = DirectX::XMFLOAT3(-30.f, pillarUpTranslation, 75.f);
	GameObject* pPillar14 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar14->GetTransform()->Translate(initialPosition);
	AddChild(pPillar14);
	m_ObjectsVector.push_back(InitValue(pPillar14, initialPosition));

	initialPosition = DirectX::XMFLOAT3(0.f, pillarUpTranslation, 75.f);
	GameObject* pPillar15 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar15->GetTransform()->Translate(initialPosition);
	AddChild(pPillar15);
	m_ObjectsVector.push_back(InitValue(pPillar15, initialPosition));

	initialPosition = DirectX::XMFLOAT3(15.f, pillarUpTranslation, 75.f);
	GameObject* pPillar16 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar16->GetTransform()->Translate(initialPosition);
	AddChild(pPillar16);
	m_ObjectsVector.push_back(InitValue(pPillar16, initialPosition));

	initialPosition = DirectX::XMFLOAT3(30.f, pillarUpTranslation, 75.f);
	GameObject* pPillar17 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pPillar17->GetTransform()->Translate(initialPosition);
	AddChild(pPillar17);
	m_ObjectsVector.push_back(InitValue(pPillar17, initialPosition));
}