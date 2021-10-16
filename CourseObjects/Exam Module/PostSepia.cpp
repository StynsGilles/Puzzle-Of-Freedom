#include "stdafx.h"
#include "PostSepia.h"
#include "RenderTarget.h"

PostSepia::PostSepia(float redScalar, float greenScalar, float blueScalar)
	: PostProcessingMaterial(L"./Resources/Effects/Post/ColorAdjustment.fx", 1)
	, m_pTextureMapVariable(nullptr)
	, m_Red(redScalar)
	, m_Green(greenScalar)
	, m_Blue(blueScalar)
{
}

void PostSepia::LoadEffectVariables()
{
	m_pTextureMapVariable = GetEffect()->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureMapVariable->IsValid())
	{
		Logger::LogWarning(L"PostGrayscale::LoadEffectVariables() > \'gTexture\' variable not found!");
		m_pTextureMapVariable = nullptr;
	}

	m_pRedScalarVariable = GetEffect()->GetVariableByName("gRedScalar")->AsScalar();
	if (!m_pRedScalarVariable->IsValid())
	{
		Logger::LogWarning(L"PostGrayscale::LoadEffectVariables() > \'gRedScalar\' variable not found!");
		m_pRedScalarVariable = nullptr;
	}
	
	m_pGreenScalarVariable = GetEffect()->GetVariableByName("gGreenScalar")->AsScalar();
	if (!m_pGreenScalarVariable->IsValid())
	{
		Logger::LogWarning(L"PostGrayscale::LoadEffectVariables() > \'gGreenScalar\' variable not found!");
		m_pGreenScalarVariable = nullptr;
	}
	
	m_pBlueScalarVariable = GetEffect()->GetVariableByName("gBlueScalar")->AsScalar();
	if (!m_pBlueScalarVariable->IsValid())
	{
		Logger::LogWarning(L"PostGrayscale::LoadEffectVariables() > \'gBlueScalar\' variable not found!");
		m_pBlueScalarVariable = nullptr;
	}
}

void PostSepia::UpdateEffectVariables(RenderTarget* pRendertarget)
{
	if (m_pTextureMapVariable)
		m_pTextureMapVariable->SetResource(pRendertarget->GetShaderResourceView());

	if (m_pRedScalarVariable)
		m_pRedScalarVariable->SetFloat(m_Red);

	if (m_pGreenScalarVariable)
		m_pGreenScalarVariable->SetFloat(m_Green);

	if (m_pBlueScalarVariable)
		m_pBlueScalarVariable->SetFloat(m_Blue);
}
