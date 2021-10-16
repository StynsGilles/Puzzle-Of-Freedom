#pragma once
#include "Material.h"

class TextureData;

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial();
    virtual ~DiffuseMaterial();

    void SetDiffuseTexture(const std::wstring& assetFile);
    void SetScale(const DirectX::XMFLOAT2& scale);
	
    DiffuseMaterial& operator=(const DiffuseMaterial& other) = delete;
    DiffuseMaterial& operator=(DiffuseMaterial&& other) = delete;
protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
    TextureData* m_pDiffuseTexture;
    static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
    DirectX::XMFLOAT2 m_Scale = DirectX::XMFLOAT2(1.f, 1.f);
    static ID3DX11EffectVectorVariable* m_pScaleVariable;
	
	// ----------------------------
	// Disabling default copy constructor and default
	// assignment operator.
    // ----------------------------
    DiffuseMaterial(const DiffuseMaterial& obj) = delete;
    DiffuseMaterial(DiffuseMaterial&& obj) = delete;
};