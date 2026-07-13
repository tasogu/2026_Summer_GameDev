#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene(void)
{
}

GameClearScene::~GameClearScene(void)
{
}

void GameClearScene::Init(void)
{
	SoundManager::GetInstance().PlayBGM(SoundManager::BGM_ID::CLEAR);
	SoundManager::GetInstance().PlaySE(SoundManager::SE_ID::CLEAR);

	imgGameClear = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMECLEAR)->handleId_;

}

void GameClearScene::Update(void)
{
	auto const& ins = InputManager::GetInstance();

	bool isAttackPad = ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

	// ÉVÅ[ÉìëJà⁄
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || isAttackPad)
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

}

void GameClearScene::Draw(void)
{
	DrawGraph(0, 0, imgGameClear, true);
}

void GameClearScene::Release(void)
{
}
