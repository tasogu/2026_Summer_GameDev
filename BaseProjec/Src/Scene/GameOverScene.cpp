#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
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
	DrawFormatString(0, 0, GetColor(255, 255, 255), "GAMEOVER");

}

void GameOverScene::Release(void)
{
}
