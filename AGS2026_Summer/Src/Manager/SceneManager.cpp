#include <DxLib.h>
#include "../Scene/SceneBase.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	//デルタタイム
	preTime_ = std::chrono::system_clock::now();

	//３D用の設定
	Init3D();

	//初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Init3D(void)
{
	//背景職設定
	SetBackgroundColor(0, 139, 139);

	//Zバッファを有効にする
	SetUseZBuffer3D(true);

	//Zバッファへのかきっこみを有効にする
	SetWriteZBuffer3D(true);

	//バック化リングを有効にする
	SetUseBackCulling(true);

	//ライトの設定
	SetUseLighting(true);
	
	//ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	//フォグ設定
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 2000.0f);
}

void SceneManager::Update(void)
{
	if (scene_ == nullptr)
	{
		return;
	}

	//デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;


}

void SceneManager::Draw(void)
{
	//描画先グラフィック領域の指定
	//(3D描画で使用するカメラの設定などがリセットされるされる)
	SetDrawScreen(DX_SCREEN_BACK);

	//描画を初期化
	ClearDrawScreen();

	//描画
	scene_->Draw();
}

//状態以降
void SceneManager::ChangeScen(SCENE_ID nextId)
{
	waitSceneId_ = nextId;

	
}

//シーンID取得
SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

//デルタタイムの取得
float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

//カメラの取得
Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;

	//デルタタイム
	deltaTime_ = 1.0f / 60.0f;
}


//デルタタイムをリセットする
void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

//シーン移行
void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	//シーンを変更する
	sceneId_ = sceneId;

	//現在のシーンを解放
	if (scene_ != nullptr)
	{
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	}

	scene_->Init();

	waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::Destroy(void)
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}
