#include "stdafx.h"
#include "DiffuseMaterial.h"
#include "ContentManager.h"
#include "TextureData.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectVectorVariable* DiffuseMaterial::m_pScaleVariable = nullptr;

DiffuseMaterial::DiffuseMaterial()
	: Material(L"./Resources/Effects/PosNormTex3d.fx")
	, m_pDiffuseTexture(nullptr)
{
}

DiffuseMaterial::~DiffuseMaterial()
{
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture =	ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial::SetScale(const DirectX::XMFLOAT2& scale)
{
	m_Scale = scale;
}

void DiffuseMaterial::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
		m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gDiffuseMap")->AsShaderResource();

	if (!m_pScaleVariable)
		m_pScaleVariable = GetEffect()->GetVariableByName("gScale")->AsVector();
}

void DiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	
	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());

	if (m_pScaleVariable)
		m_pScaleVariable->SetFloatVector(&m_Scale.x);
	
}