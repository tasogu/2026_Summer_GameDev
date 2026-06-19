#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene(void)
{
}

GameClearScene::~GameClearScene(void)
{
}

void GameClearScene::Init(void)
{
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
	DrawFormatString(0,0,GetColor(255,255,255),"GAMECLEAR");
}

void GameClearScene::Release(void)
{
}
