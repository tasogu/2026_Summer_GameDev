#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Actor/Stage.h"
#include "../Object/Actor/Character/Player.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	SceneBase()

{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	//ステージの生成
	stage_  = new Stage();
	stage_->Init();

	//プレイヤーの生成
	player_ = new Player();
	player_->Init();
}

void GameScene::Update(void)
{

	//ステージの更新
	stage_->Update();

	//プレイヤーの更新
	player_->Update();

	// シーン遷移
	auto const& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

}

void GameScene::Draw(void)
{
	//ステージの描画
	stage_->Draw();

	//プレイヤーの描画
	player_->Draw();

}

void GameScene::Release(void)
{
	//ステージの開放
	stage_->Release();
	delete stage_;

	//プレイヤーの開放
	player_->Release();
	delete player_;

}
