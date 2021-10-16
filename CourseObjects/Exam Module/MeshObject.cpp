#include "stdafx.h"
#include "MeshObject.h"
#include "Components.h"
#include "ModelAnimator.h"
#include "../../Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"

MeshObject::MeshObject()
	: m_pMesh( nullptr )
{
}

MeshObject::~MeshObject()
{
}

void MeshObject::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto* skinnedDiffuseMaterial = new SkinnedDiffuseMaterial_Shadow();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/akai_diffuse.png");
	skinnedDiffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 0);
	
	m_pMesh = new ModelComponent(L"./Resources/Meshes/Protagonist/Character.ovm");
	m_pMesh->SetMaterial(0);
	AddComponent(m_pMesh);

	m_pMesh->GetTransform()->Scale(0.04f, 0.04f, 0.04f);
	m_pMesh->GetTransform()->Translate(0.f, -4.f, 0.f);
}

void MeshObject::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void MeshObject::Update(const GameContext& gameContext)
{
	auto animator = m_pMesh->GetAnimator();
	m_TimePassedCurrentAnim += gameContext.pGameTime->GetElapsed();
	if (animator)
	{
		if (m_PlayAnimation)
		{
			if (m_animationQueue.size() > 1)
			{
				if (m_animationQueue[0]->loop)
				{
					animator->SetAnimation(m_animationQueue[1]->animationId);
					m_CurrentAnimation = m_animationQueue[1];
					m_animationQueue.pop_front();
				}
				if (!m_animationQueue[0]->loop)
				{
					if (m_TimePassedCurrentAnim >= m_animationQueue[0]->duration)
					{
						m_TimePassedCurrentAnim = 0.f;
						animator->SetAnimation(m_animationQueue[1]->animationId);
						m_CurrentAnimation = m_animationQueue[1];
						m_animationQueue.pop_front();
					}
				}
			}
			bool reverse = false;
			if (m_CurrentAnimation != nullptr && m_CurrentAnimation->playReversed)
				reverse = m_CurrentAnimation->playReversed;

			animator->SetPlayReversed(reverse);

			animator->Play();
		}
		else
		{
			animator->Pause();
		}
	}

}

void MeshObject::SetAnimation(AnimationId animationId, bool loop, bool reversed)
{
	auto animationInQueue = std::find_if(m_animationQueue.begin(), m_animationQueue.end(), [&animationId](std::shared_ptr<AnimationInfo> a)
	{
			return (UINT)animationId == a->animationId;
	});
	if (animationInQueue == m_animationQueue.end())
	{
		std::shared_ptr<AnimationInfo> newAnimation = std::make_shared<AnimationInfo>();
		newAnimation->animationId = (UINT)animationId;
		newAnimation->loop = loop;
		newAnimation->playReversed = reversed;
		m_animationQueue.push_back(newAnimation);
		if (!loop && m_pMesh->HasAnimator())
		{
			newAnimation->duration = m_pMesh->GetAnimator()->GetAnimationDuration(newAnimation->animationId);
			m_TimePassedCurrentAnim = 0.f;
		}
	}
	else
		m_animationQueue[animationInQueue - m_animationQueue.begin()]->playReversed = reversed;
}

void MeshObject::SetForcedAnimation(AnimationId animationId, bool loop, bool reversed)
{
	std::shared_ptr<AnimationInfo> newAnimation = std::make_shared<AnimationInfo>();
	newAnimation->animationId = (UINT)animationId;
	newAnimation->loop = loop;
	newAnimation->playReversed = reversed;
	if (!loop && m_pMesh->HasAnimator())
	{
		newAnimation->duration = m_pMesh->GetAnimator()->GetAnimationDuration(newAnimation->animationId);
	}
	m_animationQueue.clear();
	m_animationQueue.push_back(newAnimation);
}

void MeshObject::PauseAnimation()
{
	m_PlayAnimation = false;
}

void MeshObject::PlayAnimation()
{
	m_PlayAnimation = true;
}
