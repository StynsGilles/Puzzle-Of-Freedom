#pragma once
#include "GameScene.h"

class Hero;
class SpriteFont;

struct InitValue
{
	GameObject* object;
	DirectX::XMFLOAT3 initialPosition;

	InitValue(GameObject* obj, DirectX::XMFLOAT3 pos)
		:object(obj)
		, initialPosition(pos)
	{}
};

class BaseLevelScene : public GameScene
{
public:
	BaseLevelScene(std::wstring sceneName);
	virtual ~BaseLevelScene() = default;

	BaseLevelScene(const BaseLevelScene& other) = delete;
	BaseLevelScene(BaseLevelScene&& other) noexcept = delete;
	BaseLevelScene& operator=(const BaseLevelScene& other) = delete;
	BaseLevelScene& operator=(BaseLevelScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneActivated() override;
	virtual void DrawHud();
	void CreateMesh(const std::wstring& filepath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& rotation, float rotationDegree, int materialId, bool isTriangleMesh = false);
	virtual void ResetScene();
	
	std::vector<InitValue> m_ObjectsVector;
	const float m_MaxTime = 120.f;
	float m_TimeRemaining; //in seconds

	Hero* m_pHero = nullptr;

	bool m_LevelFinished = false;
	float m_LevelFinishedCooldown = 1.f;
	GameObject* m_pTrophyObject = nullptr;
	float m_CurrentTrophyRotation = 0.f;

	SpriteFont* m_pFont = nullptr;

	GameObject* m_pPauseMenu = nullptr;

	//Sound effects
	FMOD::Channel* m_pLoseChannel;
	FMOD::Sound* m_pLoseSound;
	FMOD::Channel* m_pWinChannel;
	FMOD::Sound* m_pWinSound;

private:
	void CreateFinishLevelTrigger(float width = 2, float height = 5, float depth = 2, DirectX::XMFLOAT3 pos = { 0,0,0 });
};

