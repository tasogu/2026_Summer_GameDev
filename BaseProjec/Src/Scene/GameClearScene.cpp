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
	// ÉVÅ[ÉìëJà⁄
	auto const& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
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
