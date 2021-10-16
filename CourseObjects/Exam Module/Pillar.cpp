#include "stdafx.h"
#include "Pillar.h"
#include "Components.h"
#include "ContentManager.h"
#include "CubePrefab.h"
#include "PhysxManager.h"
#include "SoundManager.h"

Pillar::Pillar(const std::wstring& filepath, float width, float height)
	: m_Filepath(filepath)
	, m_Width(width)
	, m_Height(height)
{
}

void Pillar::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	float rotationInRadian = 90.f / 180.f * DirectX::XM_PI;
	const DirectX::XMFLOAT3 rotation{ 1,0,0 };
	
	physx::PxQuat physxQuaternion{ rotationInRadian,physx::PxVec3{rotation.x,rotation.y,rotation.z} };
	//Apply colliders

	auto* pRigidBody = new RigidBodyComponent();
	pRigidBody->SetKinematic(true);
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group2);
	pRigidBody->SetConstraint(RigidBodyConstraintFlag::RotX, true);
	pRigidBody->SetConstraint(RigidBodyConstraintFlag::RotY, true);
	pRigidBody->SetConstraint(RigidBodyConstraintFlag::RotZ, true);
	pRigidBody->SetConstraint(RigidBodyConstraintFlag::TransY, true);
	AddComponent(pRigidBody);

	auto* pMaterial = physX->createMaterial(1.f, 1.f, 0);
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxBoxGeometry(m_Width / 2, m_Height / 2, m_Width / 2));
	AddComponent(
		new ColliderComponent(
			geom,
			*pMaterial
		)
	);

	//Create Visuals
	auto model = new GameObject();
	auto modelComponent = new ModelComponent(m_Filepath + L".ovm");
	model->AddComponent(modelComponent);
	modelComponent->SetMaterial(2);

	//Transform Visuals
	DirectX::XMFLOAT4 XMQuat{ physxQuaternion.x,physxQuaternion.y,physxQuaternion.z,physxQuaternion.w };
	model->GetTransform()->Rotate(DirectX::XMLoadFloat4(&XMQuat));
	model->GetTransform()->Translate(0, -m_Height / 2, 0);
	
	AddChild(model);

}

void Pillar::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//GetTransform()->Rotate(0, 20, 0);
	//GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setMass(100.f);
}

void Pillar::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	
}

bool Pillar::Push(const GameContext& gameContext, const physx::PxVec3& translation, const DirectX::XMFLOAT3& playerRight,
	const DirectX::XMFLOAT3& playerForward, bool& isGrounded, float addedWith)
{
	//Check if the pillar is still grounded
	auto pos = GetTransform()->GetPosition();
	physx::PxVec3 physXPos{ ToPxVec3(pos) };
	physx::PxVec3 unitDir{ 0,-1,0 };
	physXPos += unitDir * (m_Height / 2 + 0.01f);
	const physx::PxReal maxDist{ 0.1f };
	physx::PxRaycastBuffer hit{};
	physx::PxQueryFilterData filterData;
	const physx::PxU32 floorCollGroup{ (UINT)CollisionGroupFlag::Group0 | (UINT)CollisionGroupFlag::Group2 | (UINT)CollisionGroupFlag::Group3 };
	filterData.data.word0 = floorCollGroup;

	const physx::PxVec3 playerForwardVec = ToPxVec3(playerForward);
	const auto playerForwardNorm = playerForwardVec * (m_Width / 2);
	physXPos -= playerForwardNorm;
	
	auto* scene = GetComponent<RigidBodyComponent>()->GetPxRigidBody()->getScene();
	//Check if the pillar is still grounded
	isGrounded = scene->raycast(physXPos, unitDir, maxDist, hit, physx::PxHitFlag::eDEFAULT, filterData);
	if (!isGrounded || translation.magnitudeSquared() == 0.f)
		return false;
	
	unitDir = translation.getNormalized();
	physXPos = ToPxVec3(pos) + unitDir * (m_Width / 2 + 0.01f) + addedWith * unitDir;
	
	//Check front
	if (scene->raycast(physXPos, unitDir, maxDist, hit, physx::PxHitFlag::eDEFAULT, filterData))
		return false;

	//Check edges
	unitDir = translation.getNormalized();
	physXPos = ToPxVec3(pos) + unitDir * (m_Width / 2 + 0.01f);
	const physx::PxVec3 playerRightVec = ToPxVec3(playerRight);
	const auto playerRightNorm = playerRightVec * (m_Width / 2 - 0.5f);

	////Right edge
	physXPos += playerRightNorm;
	if (scene->raycast(physXPos, unitDir, maxDist, hit, physx::PxHitFlag::eDEFAULT, filterData))
		return false;

	//Left edge
	physXPos -= (playerRightNorm * 2.f);
	if (scene->raycast(physXPos, unitDir, maxDist, hit, physx::PxHitFlag::eDEFAULT, filterData))
		return false;
	
	auto displacement = DirectX::XMFLOAT3(pos.x + translation.x * gameContext.pGameTime->GetElapsed(), pos.y + translation.y * gameContext.pGameTime->GetElapsed(), pos.z + translation.z * gameContext.pGameTime->GetElapsed());
	GetTransform()->Translate(displacement);
	return true;
}
