#include "stdafx.h"
#include "Wall.h"
#include "Components.h"
#include "ContentManager.h"
#include "CubePrefab.h"
#include "PhysxManager.h"

Wall::Wall(bool climbable, float width, float height, const DirectX::XMFLOAT3& meshScale)
	: m_Climbable(climbable)
	, m_Width(width)
	, m_Height(height)
	, m_Scale(meshScale)
{
}

void Wall::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto pRigidBody = new RigidBodyComponent(true);
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group0);
	AddComponent(pRigidBody);
	auto pMaterial = physX->createMaterial(0, 0, 0);
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxBoxGeometry(m_Width/2, m_Height/2, m_Width /2));
	AddComponent(
		new ColliderComponent(
			geom,
			*pMaterial
		)
	);
	
	auto model = new GameObject();
	auto modelComponent = new ModelComponent(L"Resources/Meshes/Environment/climbableWall.ovm");
	model->AddComponent(modelComponent);
	modelComponent->SetMaterial(4);
	
	model->GetTransform()->Scale(m_Scale);
	model->GetTransform()->Translate(0, -m_Height / 2, 0);

	AddChild(model);
}

void Wall::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

bool Wall::IsClimbable() const
{
	return m_Climbable;
}

float Wall::GetHeight() const
{
	return m_Height;
}
