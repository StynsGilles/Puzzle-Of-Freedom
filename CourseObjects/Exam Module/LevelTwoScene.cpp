#include "stdafx.h"
#include "LevelTwoScene.h"
#include "Components.h"
#include "ContentManager.h"
#include "Wall.h"
#include "Pillar.h"
#include "GameScene.h"

LevelTwoScene::LevelTwoScene()
	:BaseLevelScene(L"LevelTwoScene")
{
}

void LevelTwoScene::Initialize()
{	
	BaseLevelScene::Initialize();

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
	GameObject* pFirstRowPillar1 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pFirstRowPillar1->GetTransform()->Translate(initialPosition);
	AddChild(pFirstRowPillar1);
	m_ObjectsVector.push_back(InitValue(pFirstRowPillar1, initialPosition));

	initialPosition = DirectX::XMFLOAT3(-30.f, pillarUpTranslation, 0.f);
	GameObject* pFirstRowPillar2 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pFirstRowPillar2->GetTransform()->Translate(initialPosition);
	AddChild(pFirstRowPillar2);
	m_ObjectsVector.push_back(InitValue(pFirstRowPillar2, initialPosition));

	initialPosition = DirectX::XMFLOAT3(15.f, pillarUpTranslation, 0.f);
	GameObject* pFirstRowPillar3 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pFirstRowPillar3->GetTransform()->Translate(initialPosition);
	AddChild(pFirstRowPillar3);
	m_ObjectsVector.push_back(InitValue(pFirstRowPillar3, initialPosition));

	initialPosition = DirectX::XMFLOAT3(30.f, pillarUpTranslation, 0.f);
	GameObject* pFirstRowPillar4 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pFirstRowPillar4->GetTransform()->Translate(initialPosition);
	AddChild(pFirstRowPillar4);
	m_ObjectsVector.push_back(InitValue(pFirstRowPillar4, initialPosition));

	//Second row
	initialPosition = DirectX::XMFLOAT3(-30.f, pillarUpTranslation, 15.f);
	GameObject* pSecondRowPillar1 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pSecondRowPillar1->GetTransform()->Translate(initialPosition);
	AddChild(pSecondRowPillar1);
	m_ObjectsVector.push_back(InitValue(pSecondRowPillar1, initialPosition));

	initialPosition = DirectX::XMFLOAT3(-15.f, pillarUpTranslation, 15.f);
	GameObject* pSecondRowPillar2 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pSecondRowPillar2->GetTransform()->Translate(initialPosition);
	AddChild(pSecondRowPillar2);
	m_ObjectsVector.push_back(InitValue(pSecondRowPillar2, initialPosition));

	initialPosition = DirectX::XMFLOAT3(0, pillarUpTranslation, 15.f);
	GameObject* pSecondRowPillar3 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pSecondRowPillar3->GetTransform()->Translate(initialPosition);
	AddChild(pSecondRowPillar3);
	m_ObjectsVector.push_back(InitValue(pSecondRowPillar3, initialPosition));

	initialPosition = DirectX::XMFLOAT3(15.f, pillarUpTranslation, 15.f);
	GameObject* pSecondRowPillar4 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pSecondRowPillar4->GetTransform()->Translate(initialPosition);
	AddChild(pSecondRowPillar4);
	m_ObjectsVector.push_back(InitValue(pSecondRowPillar4, initialPosition));

	initialPosition = DirectX::XMFLOAT3(30.f, pillarUpTranslation, 15.f);
	GameObject* pSecondRowPillar5 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pSecondRowPillar5->GetTransform()->Translate(initialPosition);
	AddChild(pSecondRowPillar5);
	m_ObjectsVector.push_back(InitValue(pSecondRowPillar5, initialPosition));

	//Third row
	initialPosition = DirectX::XMFLOAT3(15.f, pillarUpTranslation, 30.f);
	GameObject* pThirdRowPillar1 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pThirdRowPillar1->GetTransform()->Translate(initialPosition);
	AddChild(pThirdRowPillar1);
	m_ObjectsVector.push_back(InitValue(pThirdRowPillar1, initialPosition));

	//Fifth row
	initialPosition = DirectX::XMFLOAT3(-30.f, pillarUpTranslation, 60.f);
	GameObject* pFifthRowPillar1 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pFifthRowPillar1->GetTransform()->Translate(initialPosition);
	AddChild(pFifthRowPillar1);
	m_ObjectsVector.push_back(InitValue(pFifthRowPillar1, initialPosition));

	initialPosition = DirectX::XMFLOAT3(0.f, pillarUpTranslation, 60.f);
	GameObject* pFifthRowPillar2 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pFifthRowPillar2->GetTransform()->Translate(initialPosition);
	AddChild(pFifthRowPillar2);
	m_ObjectsVector.push_back(InitValue(pFifthRowPillar2, initialPosition));

	initialPosition = DirectX::XMFLOAT3(30.f, pillarUpTranslation, 60.f);
	GameObject* pFifthRowPillar3 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pFifthRowPillar3->GetTransform()->Translate(initialPosition);
	AddChild(pFifthRowPillar3);
	m_ObjectsVector.push_back(InitValue(pFifthRowPillar3, initialPosition));

	//Sixth row
	initialPosition = DirectX::XMFLOAT3(-30.f, pillarUpTranslation, 75.f);
	GameObject* pSixthRowPillar1 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pSixthRowPillar1->GetTransform()->Translate(initialPosition);
	AddChild(pSixthRowPillar1);
	m_ObjectsVector.push_back(InitValue(pSixthRowPillar1, initialPosition));

	initialPosition = DirectX::XMFLOAT3(15.f, pillarUpTranslation, 75.f);
	GameObject* pSixthRowPillar2 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pSixthRowPillar2->GetTransform()->Translate(initialPosition);
	AddChild(pSixthRowPillar2);
	m_ObjectsVector.push_back(InitValue(pSixthRowPillar2, initialPosition));

	initialPosition = DirectX::XMFLOAT3(30.f, pillarUpTranslation, 75.f);
	GameObject* pSixthRowPillar3 = new Pillar(L"Resources/Meshes/Environment/pillarMedium", pillarWidth, pillarHeight);
	pSixthRowPillar3->GetTransform()->Translate(initialPosition);
	AddChild(pSixthRowPillar3);
	m_ObjectsVector.push_back(InitValue(pSixthRowPillar3, initialPosition));
}