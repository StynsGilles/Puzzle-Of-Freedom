#pragma once
#include "PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostSepia : public PostProcessingMaterial
{
public:
	PostSepia(float redScalar = 1.f, float greenScalar = 1.f, float blueScalar = 1.f);
	PostSepia(const PostSepia& other) = delete;
	PostSepia(PostSepia&& other) noexcept = delete;
	PostSepia& operator=(const PostSepia& other) = delete;
	PostSepia& operator=(PostSepia&& other) noexcept = delete;
	virtual ~PostSepia() = default;

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(RenderTarget* pRendertarget) override;

private:
	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariable;
	ID3DX11EffectScalarVariable* m_pRedScalarVariable;
	ID3DX11EffectScalarVariable* m_pGreenScalarVariable;
	ID3DX11EffectScalarVariable* m_pBlueScalarVariable;

	float m_Red, m_Green, m_Blue;
};

