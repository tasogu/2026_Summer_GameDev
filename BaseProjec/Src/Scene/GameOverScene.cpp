#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
	SoundManager::GetInstance().PlayBGM(SoundManager::BGM_ID::CLEAR);

	imgGameOver_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMEOVER)->handleId_;

}

void GameOverScene::Update(void)
{
	// ÉVÅ[ÉìëJà⁄
	auto const& ins = InputManager::GetInstance();
	bool isAttackPad = ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

	if (ins.IsTrgDown(KEY_INPUT_SPACE) || isAttackPad)
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

}

void GameOverScene::Draw(void)
{
	DrawGraph(0, -40, imgGameOver_,true);
}

void GameOverScene::Release(void)
{
}
