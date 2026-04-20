#include <DxLib.h>
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
	

}

void Application::Run(void)
{
}

void Application::Destroy(void)
{
}

bool Application::IsInitFail(void) const
{
	return false;
}

bool Application::IsReleaseFail(void) const
{
	return false;
}
