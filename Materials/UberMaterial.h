#pragma once
#include "Material.h"

class TextureData;

class UberMaterial : public Material
{
public:
	UberMaterial();
	~UberMaterial();

	//LIGHT
	//*****
	void SetLightDirection(DirectX::XMFLOAT3 direction);

	//DIFFUSE
	//*******
	void EnableDiffuseTexture(bool enable);
	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetDiffuseColor(DirectX::XMFLOAT4 color);

	//SPECULAR
	//********
	void SetSpecularColor(DirectX::XMFLOAT4 color);
	void EnableSpecularLevelTexture(bool enable);
	void SetSpecularLevelTexture(const std::wstring& assetFile);
	void SetShininess(int shininess);

	//AMBIENT
	//*******
	void SetAmbientColor(DirectX::XMFLOAT4 color);
	void SetAmbientIntensity(float intensity);

	//NORMAL MAPPING
	//**************
	void FlipNormalGreenChannel(bool flip);
	void EnableNormalMapping(bool enable);
	void SetNormalMapTexture(const std::wstring& assetFile);

	//ENVIRONMENT MAPPING
	//*******************
	void EnableEnvironmentMapping(bool enable);
	void SetEnvironmentCube(const std::wstring& assetFile);
	void SetReflectionStrength(float strength);
	void SetRefractionStrength(float strength);
	void SetRefractionIndex(float index);

	//OPACITY
	//*******
	void SetOpacity(float opacity);
	void EnableOpacityMap(bool enable);
	void SetOpacityTexture(const std::wstring& assetFile);

	//SPECULAR MODELS
	//***************
	void EnableSpecularBlinn(bool enable);
	void EnableSpecularPhong(bool enable);

	//FRESNEL FALLOFF
	//***************
	void EnableFresnelFaloff(bool enable);
	void SetFresnelColor(DirectX::XMFLOAT4 color);
	void SetFresnelPower(float power);
	void SetFresnelMultiplier(float multiplier);
	void SetFresnelHardness(float hardness);

protected:
	virtual void LoadEffectVariables() override;
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
	void LoadLightVariables() const;
	void LoadDiffuseVariables()const;
	void LoadSpecularVariables()const;
	void LoadAmbientVariables()const;
	void LoadNormalVariables()const;
	void LoadEnvironmentMappingVariables()const;
	void LoadOpacityVariables()const;
	void LoadSpecularModelVariables()const;
	void LoadFresnelFalloffVariables()const;

	void UpdateLightVariables() const;
	void UpdateDiffuseVariables() const;
	void UpdateSpecularVariables()const;
	void UpdateAmbientVariables()const;
	void UpdateNormalVariables()const;
	void UpdateEnvironmentMappingVariables()const;
	void UpdateOpacityVariables()const;
	void UpdateSpecularModelVariables()const;
	void UpdateFresnelFalloffVariables()const;
	
	//LIGHT
	//*****
	DirectX::XMFLOAT3 m_LightDirection = DirectX::XMFLOAT3(0.577f, 0.577f, 0.577f);
	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;

	//DIFFUSE
	//*******
	bool m_bDiffuseTexture = false;
	static ID3DX11EffectScalarVariable* m_pUseDiffuseTextureVariable;
	TextureData* m_pDiffuseTexture = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	DirectX::XMFLOAT4 m_ColorDiffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	static ID3DX11EffectVectorVariable* m_pDiffuseColorVariable;

	//SPECULAR
	//********
	DirectX::XMFLOAT4 m_ColorSpecular = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	static ID3DX11EffectVectorVariable* m_pSpecularColorVariable;
	bool m_bSpecularLevelTexture = false;
	static ID3DX11EffectScalarVariable* m_pUseSpecularLevelTextureVariable;
	TextureData* m_pSpecularLevelTexture = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pSpecularLevelSRVvariable;
	int m_Shininess = 1;
	static ID3DX11EffectScalarVariable* m_pShininessVariable;

	//AMBIENT
	//*******
	DirectX::XMFLOAT4 m_ColorAmbient = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	static ID3DX11EffectVectorVariable* m_pAmbientColorVariable;
	float m_AmbientIntensity = 0.f;
	static ID3DX11EffectScalarVariable* m_pAmbientIntensityVariable;

	//NORMAL MAPPING
	//**************
	bool m_bFlipGreenChannel = false;
	static ID3DX11EffectScalarVariable* m_pFlipGreenChannelVariable;
	bool m_bNormalMapping = false;
	static ID3DX11EffectScalarVariable* m_pUseNormalMappingVariable;
	TextureData* m_pNormalMappingTexture = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pNormalMappingSRVvariable;

	//ENVIRONMENT MAPPING
	//*******************
	bool m_bEnvironmentMapping = false;
	static ID3DX11EffectScalarVariable* m_pUseEnvironmentMappingVariable;
	TextureData* m_pEnvironmentCube = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pEnvironmentSRVvariable;
	float m_ReflectionStrength = 0.f;
	static ID3DX11EffectScalarVariable* m_pReflectionStrengthVariable;
	float m_RefractionStrength = 0.f;
	static ID3DX11EffectScalarVariable* m_pRefractionStrengthVariable;
	float m_RefractionIndex = 0.f;
	static ID3DX11EffectScalarVariable* m_pRefractionIndexVariable;

	//OPACITY
	//***************
	float m_Opacity = 1.f;
	static ID3DX11EffectScalarVariable* m_pOpacityVariable;
	bool m_bOpacityMap = false;
	static ID3DX11EffectScalarVariable* m_pUseOpacityMapVariable;
	TextureData* m_pOpacityMap = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pOpacitySRVvariable;

	//SPECULAR MODELS
	//***************
	bool m_bSpecularBlinn = false;
	static ID3DX11EffectScalarVariable* m_pUseBlinnVariable;
	bool m_bSpecularPhong = false;
	static ID3DX11EffectScalarVariable* m_pUsePhongVariable;

	//FRESNEL FALLOFF
	//***************
	bool m_bFresnelFaloff = false;
	static ID3DX11EffectScalarVariable* m_pUseFresnelFalloffVariable;
	DirectX::XMFLOAT4 m_ColorFresnel = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	static ID3DX11EffectVectorVariable* m_pFresnelColorVariable;
	float m_FresnelPower = 1.f;
	static ID3DX11EffectScalarVariable* m_pFresnelPowerVariable;
	float m_FresnelMultiplier = 1.f;
	static ID3DX11EffectScalarVariable* m_pFresnelMultiplierVariable;
	float m_FresnelHardness = 0.f;
	static ID3DX11EffectScalarVariable* m_pFresnelHardnessVariable;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	UberMaterial(const UberMaterial& obj);
	UberMaterial& operator=(const UberMaterial& obj);
};

