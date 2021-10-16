//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "DiffuseMaterial_Shadow.h"
#include "GeneralStructs.h"
#include "Logger.h"
#include "ContentManager.h"
#include "TextureData.h"
#include "Components.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial_Shadow::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* DiffuseMaterial_Shadow::m_pShadowSRVvariable = nullptr;
ID3DX11EffectVectorVariable* DiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* DiffuseMaterial_Shadow::m_pLightWVPvariable = nullptr;
ID3DX11EffectVectorVariable* DiffuseMaterial_Shadow::m_pScaleVariable = nullptr;

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow() : Material(L"./Resources/Effects/Shadow/PosNormTex3D_Shadow.fx"),
	m_pDiffuseTexture(nullptr)
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial_Shadow::SetLightDirection(DirectX::XMFLOAT3 dir)
{
	m_LightDirection = dir;
}

void DiffuseMaterial_Shadow::SetScale(const DirectX::XMFLOAT2& scale)
{
	m_Scale = scale;
}

void DiffuseMaterial_Shadow::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	} 

	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = GetEffect()->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pLightDirectionVariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'gLightDirection\' variable not found!");
			m_pLightDirectionVariable = nullptr;
		}
	}

	if (!m_pShadowSRVvariable)
	{
		m_pShadowSRVvariable = GetEffect()->GetVariableByName("gShadowMap")->AsShaderResource();
		if (!m_pShadowSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'gShadowMap\' variable not found!");
			m_pShadowSRVvariable = nullptr;
		}
	}

	if (!m_pLightWVPvariable)
	{
		m_pLightWVPvariable = GetEffect()->GetVariableByName("gWorldViewProj_Light")->AsMatrix();
		if (!m_pLightWVPvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'gWorldViewProj_Light\' variable not found!");
			m_pLightWVPvariable = nullptr;
		}
	}


	if (!m_pScaleVariable)
		m_pScaleVariable = GetEffect()->GetVariableByName("gScale")->AsVector();
}

void DiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());

	if (m_pLightDirectionVariable)
		m_pLightDirectionVariable->SetFloatVector(&m_LightDirection.x);

	if (m_pShadowSRVvariable)
		m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());

	if (pModelComponent && m_pLightWVPvariable)
	{
		auto lightVP = gameContext.pShadowMapper->GetLightVP();
		const auto lightVPMatrix = DirectX::XMLoadFloat4x4(&lightVP);
		const auto& world = pModelComponent->GetTransform()->GetWorld();
		const auto worldMatrix = DirectX::XMLoadFloat4x4(&world);
		const auto lightWorldViewProjection = worldMatrix * lightVPMatrix;
		DirectX::XMFLOAT4X4 WVP;
		XMStoreFloat4x4(&WVP, lightWorldViewProjection);
		m_pLightWVPvariable->SetMatrix(&WVP._11);
	}

	if (m_pScaleVariable)
		m_pScaleVariable->SetFloatVector(&m_Scale.x);
}