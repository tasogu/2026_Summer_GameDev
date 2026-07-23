#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Actor/Stage.h"
#include "../Object/Actor/Character/Player.h"
#include "../Manager/Camera.h"
#include "../Manager/EnemyManager.h"
#include "../Manager/SoundManager.h"
#include "../Common/Fader.h"
#include "../Object/WarpPortal.h"
#include "GameScene.h"


GameScene::GameScene(void)
	:
	stage_(nullptr),
	player_(nullptr),
	enemy_(nullptr),
	isPause_(false),
	//collder_(nullptr),
	SceneBase(),
	gameType_(GAME_TYPE::PLAY),
	loadStep_(0)
{
	stageType_ = STAGE_TYPE::STAGE1;
}

GameScene::~GameScene(void)
{
	ColliderManager::GetInstance().ClearAll();
}

void GameScene::Init(void)
{
	SoundManager::GetInstance().PlayBGM(SoundManager::BGM_ID::GAME);

	//ステージの生成
	stage_ = std::make_unique<Stage>(stageType_);
	stage_->Init();

	//プレイヤーの生成
	player_ = std::make_unique<Player>(stageType_);
	player_->Init();

	//エネミーの生成
	enemy_ = std::make_unique<EnemyManager>(stageType_);
	enemy_->Init(); 

	//フェードの生成
	fader_ = std::make_unique<Fader>();
	fader_->Init();

	/////仮ワープステージ描画
	warpPortal_ = std::make_unique<WarpPortal>();
	warpPortal_->Init();
	warpPortal_->SetPlayer(player_.get());

	//カメラにも登録
	std::shared_ptr<Camera> camera = sceMng_.GetCamera();
	
	//カメラ追従設定
	camera->SetFollow(&player_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW);
}

void GameScene::Update(void)
{

	auto& ins = InputManager::GetInstance();


	//Qキーが押されたら画面をポーズする・ポーズ解除
	if (ins.IsNew(KEY_INPUT_ESCAPE))
	{
		isPause_ = !isPause_;
	}

	if (isPause_)
	{
		return;
	}

	switch (gameType_) {
		case GAME_TYPE::PLAY:
			//ステージの更新
			stage_->Update();

			//プレイヤーの更新
			player_->Update();

			//エネミーの更新
			enemy_->Update(player_.get());

			//フェードの更新
			fader_->Update();


			//プレイヤーが死んだら
			if (player_->IsDead()) {
				sceMng_.ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			}

			else if (enemy_->IsAllDead())
			{
				if (stageType_ == STAGE_TYPE::STAGE2) {
					sceMng_.ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
				}
				else {
					gameType_ = GAME_TYPE::WAIT_PORTAL;
				}
			}
		break;
		case GAME_TYPE::WAIT_PORTAL:
			warpPortal_->Update();

			//ステージの更新
			stage_->Update();

			//プレイヤーの更新
			player_->Update();

			//フェードの更新
			fader_->Update();

			//ワープポータルに触れたらフェードアウト
			if (warpPortal_->IsTouched() == true) {

				gameType_ = GAME_TYPE::FADEOUT;
				fader_->SetFade(Fader::STATE::FADE_OUT);
			}
			break;
		case GAME_TYPE::FADEOUT:
			//フェードの更新
			fader_->Update();

			if (fader_->IsEnd() == true) {
				gameType_ = GAME_TYPE::MEMORY_SWAP;
			}
			break;
		case GAME_TYPE::MEMORY_SWAP:
			LoadNextStage();
			break;
		case GAME_TYPE::FADEIN:
			//フェードの更新
			fader_->Update();

			// フェードイン演出が終わるのを待つ
			if (fader_->IsEnd() == true) {
				gameType_ = GAME_TYPE::PLAY;
			}
			break;
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

	//フェードの描画
	fader_->Draw();

	//ワープポータルの描画
	warpPortal_->Draw();


}

void GameScene::Release(void)
{
	//リストを削除
	ColliderManager::GetInstance().ClearAll();

	//ステージの開放
	stage_->Release();

	//プレイヤーの開放
	player_->Release();

	//エネミーの開放
	enemy_->Release();

	warpPortal_->Release();
}

void GameScene::LoadNextStage()
{
	switch (loadStep_) {
	case 0:
		//ステージを２に移行
		stageType_ = STAGE_TYPE::STAGE2;

		//ステージの初期化
		stage_ = std::make_unique<Stage>(stageType_);
		stage_->Init();

		loadStep_ = 1;
		break;
	case 1:
		//エネミーの初期化
		enemy_ = std::make_unique<EnemyManager>(stageType_);
		enemy_->Init();
		loadStep_ = 2;
		break;
	case 2:
	{
		//カメラの初期化
		std::shared_ptr<Camera> camera = sceMng_.GetCamera();
		camera->SetFollow(&player_->GetTransform());
		loadStep_ = 3;
		break;
	}
	case 3:
		/////仮ワープステージ描画
		warpPortal_->Release();
		warpPortal_ = std::make_unique<WarpPortal>();
		warpPortal_->Init();
		warpPortal_->SetPlayer(player_.get());
		gameType_ = GAME_TYPE::FADEIN;
		fader_->SetFade(Fader::STATE::FADE_IN);
		loadStep_ = 0;

		break;
	}
}
