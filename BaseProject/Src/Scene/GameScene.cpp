#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Actor/Stage.h"
#include "../Object/Actor/Character/Player.h"
#include "../Manager/Camera.h"
#include "../Manager/EnemyManager.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	stage_(nullptr),
	player_(nullptr),
	enemy_(nullptr),
	SceneBase()
{
}

GameScene::~GameScene(void)
{
	delete stage_;
	delete player_;
	delete enemy_;
	delete collder_;
}

void GameScene::Init(void)
{
	//ステージの生成
	stage_  = new Stage();
	stage_->Init();

	//プレイヤーの生成
	player_ = new Player();
	player_->Init();

	//エネミーの生成
	enemy_ = new EnemyManager();
	enemy_->Init();;

	//コライダーの生成
	collder_ = new ColliderManager();
	
	//プレイヤーのコライダーを登録
	collder_->Register(player_->GetOwnCollider(0));

	// ステージのコライダを取得
	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));

	//collder_->Register(stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL)));

	//プレイヤーに登録
	player_->AddHitCollider(stageCollider);

	//エネミーに追加
	enemy_->AddHitCollider(stageCollider);

	//カメラにも登録
	Camera* camera = sceMng_.GetCamera();
	
	//カメラ追従設定
	camera->SetFollow(&player_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW);
}

void GameScene::Update(void)
{

	//ステージの更新
	stage_->Update();

	//プレイヤーの更新
	player_->Update();

	//エネミーの更新
	enemy_->Update();

	//コライダーの更新
	collder_->Update();

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

	//エネミーの描画
	enemy_->Draw();
}

void GameScene::Release(void)
{
	//ステージの開放
	stage_->Release();
	delete stage_;

	//プレイヤーの開放
	player_->Release();
	delete player_;

	//エネミーの開放
	enemy_->Release();
	delete enemy_;
}
