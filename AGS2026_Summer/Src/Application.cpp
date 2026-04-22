#include <DxLib.h>
#include<chrono>
#include <thread>
#include "Manager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"


Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	//アプリケーションの初期設定
	SetWindowText("aaa");

	//ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, SCREEN_COLLAR);
	ChangeWindowMode(true);

	//DXライブラリ初期化処理
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	//キー制御初期化
	SetUseDirectInputFlag(true);

	//リソース管理初期化
	ResourceManager::Createinstance();

	//シーン管理初期化
	SceneManager::CreateInstance();
}

void Application::Run(void)
{
	//1フレーム当たりの理想的な時間(1000ms / 60fps = 16.666....ms)
	const std::chrono::microseconds frameTime(1000000 / 60);

	auto& sceneManager = SceneManager::GetInstance();
	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//①開始時間を記録
		auto startTime = std::chrono::steady_clock::now();

		sceneManager.Update();

		sceneManager.Draw();

		ScreenFlip();

		//②かかった時間を計算し、余っていれば待機
		auto endTime = std::chrono::steady_clock::now();
		auto processTime = endTime - startTime;
		auto sleepTime = frameTime - processTime;

		if (sleepTime > std::chrono::microseconds(0)) {
			std::this_thread::sleep_for(sleepTime);
		}
	}

}

void Application::Destroy(void)
{
	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;

}


bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

}
