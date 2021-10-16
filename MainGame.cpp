
#include "stdafx.h"

#include "MainGame.h"
#include "GeneralStructs.h"
#include "SceneManager.h"
#include "DebugRenderer.h"

#define EXAM

#ifdef W3
	#include "CourseObjects/Week 3/MinionScene.h"
	#include "CourseObjects/Week 3/CubeTestScene.h"
	#include "CourseObjects/Week 3/PhysicsTestScene.h"
	#include "CourseObjects/Week 3/PongScene.h"
	#include "CourseObjects/Week 3/TestScene.h"
#endif
#ifdef W4_A
	#include "CourseObjects/Week 4/SpriteTestScene.h"
#endif
#ifdef W4_B
	#include "CourseObjects/Week 4/ModelTestScene.h"
	#include "CourseObjects/Week 4/SpikeyScene.h"
	#include "CourseObjects/Week 4/UberScene.h"
#endif
#ifdef W5_A
	#include "CourseObjects/Week 5/FontTestScene.h"
#endif
#ifdef W5_B
	#include "CourseObjects/Week 5/PickScene.h"
	#include "CourseObjects/Week 5/CharacterScene.h"
#endif
#ifdef W6
	#include "CourseObjects/Week 6/SoftwareSkinningScene_1.h"
	#include "CourseObjects/Week 6/SoftwareSkinningScene_2.h"
	#include "CourseObjects/Week 6/SoftwareSkinningScene_3.h"
#endif
#ifdef W7
	#include "CourseObjects/Week 7/HardwareSkinningScene.h"
#endif
#ifdef W8
	#include "CourseObjects/Week 8/ShadowMappingScene.h"
#endif
#ifdef W9
	#include "CourseObjects/Week 9/ParticleScene.h"
#endif
#ifdef W10
	#include "CourseObjects/Week 10/PostProcessingScene.h"
#endif
#ifdef EXAM
	#include "CourseObjects/Exam Module/MainMenuScene.h"
	#include "CourseObjects/Exam Module/ControlsMenu.h"
	#include "CourseObjects/Exam Module/ExamScene.h"
	#include "CourseObjects/Exam Module/LevelTwoScene.h"
	#include "CourseObjects/Exam Module/GameOverScreen.h"
	#include "CourseObjects/Exam Module/TutorialLevel.h"
#endif
#ifdef Milestone3
	#include "CourseObjects/Week 9/ParticleScene.h"
	#include "CourseObjects/Week 10/PostProcessingScene.h"
	#include "CourseObjects/Exam Module/ExamScene.h"
#endif

MainGame::MainGame(void)
{}

MainGame::~MainGame(void)
{}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
	//Nothing to do atm.
}

void MainGame::Initialize()
{
#ifdef W3
	SceneManager::GetInstance()->AddGameScene(new MinionScene());
	SceneManager::GetInstance()->AddGameScene(new CubeTestScene());
	SceneManager::GetInstance()->AddGameScene(new PhysicsTestScene());
	SceneManager::GetInstance()->AddGameScene(new PongScene());
	SceneManager::GetInstance()->AddGameScene(new TestScene());

	SceneManager::GetInstance()->SetActiveGameScene(L"MinionScene");
#endif
#ifdef W4_A
	SceneManager::GetInstance()->AddGameScene(new SpriteTestScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"SpriteTestScene");
#endif
#ifdef W4_B
	SceneManager::GetInstance()->AddGameScene(new ModelTestScene());
	SceneManager::GetInstance()->AddGameScene(new SpikeyScene());
	SceneManager::GetInstance()->AddGameScene(new UberScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"ModelTestScene");
#endif
#ifdef W5_A
	SceneManager::GetInstance()->AddGameScene(new FontTestScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"FontTestScene");
#endif
#ifdef W5_B
	SceneManager::GetInstance()->AddGameScene(new PickScene());
	SceneManager::GetInstance()->AddGameScene(new CharacterScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"PickScene");
#endif
#ifdef W6
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_1());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_2());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_3());
	SceneManager::GetInstance()->SetActiveGameScene(L"SoftwareSkinningScene_1");
#endif
#ifdef W7
	SceneManager::GetInstance()->AddGameScene(new HardwareSkinningScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"HardwareSkinningScene");
#endif
#ifdef W8
	SceneManager::GetInstance()->AddGameScene(new ShadowMappingScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"ShadowMappingScene");
#endif
#ifdef W9
	SceneManager::GetInstance()->AddGameScene(new ParticleScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"ParticleScene");
#endif
#ifdef W10
	SceneManager::GetInstance()->AddGameScene(new PostProcessingScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"PostProcessingScene");
#endif
#ifdef EXAM
	SceneManager::GetInstance()->AddGameScene(new MainMenuScene());
	SceneManager::GetInstance()->AddGameScene(new ControlsMenu());
	SceneManager::GetInstance()->AddGameScene(new TutorialLevel());
	SceneManager::GetInstance()->AddGameScene(new ExamScene());
	SceneManager::GetInstance()->AddGameScene(new LevelTwoScene());
	SceneManager::GetInstance()->AddGameScene(new GameOverScreen());
	SceneManager::GetInstance()->SetActiveGameScene(L"MainMenuScene");
	DebugRenderer::ToggleDebugRenderer();
#endif 
#ifdef Milestone3
	SceneManager::GetInstance()->AddGameScene(new ExamScene());
	SceneManager::GetInstance()->AddGameScene(new ParticleScene());
	SceneManager::GetInstance()->AddGameScene(new PostProcessingScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"ParticleScene");
#endif	

}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	//switch (message)
	//{
	//	case WM_KEYUP:
	//	{
	//		if ((lParam & 0x80000000) != 0x80000000)
	//			return -1;

	//		//NextScene
	//		if (wParam == VK_F3)
	//		{
	//			SceneManager::GetInstance()->NextScene();
	//			return 0;
	//		}
	//		//PreviousScene
	//		else if (wParam == VK_F2)
	//		{
	//			SceneManager::GetInstance()->PreviousScene();
	//			return 0;
	//		}
	//		else if (wParam == VK_F4)
	//		{
	//			DebugRenderer::ToggleDebugRenderer();
	//			return 0;
	//		}
	//		else if (wParam == VK_F6)
	//		{
	//			auto activeScene = SceneManager::GetInstance()->GetActiveScene();
	//			activeScene->GetPhysxProxy()->NextPhysXFrame();
	//		}
	//	}
	//}

	return -1;
}
