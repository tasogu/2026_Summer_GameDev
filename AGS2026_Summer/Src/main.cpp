#include<DxLib.h>
#include <iostream>
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//WinMain関数
//-----------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);



	//メインループ
	while (ProcessMessage() == 0) {
		//描画スクリーンの設定
		SetDrawScreen(DX_SCREEN_BACK);

		//画面を黒でクリアする
		ClearDrawScreen();

		//画面更新
		ScreenFlip();
	}

	//DXライブラリ終了処理
	if (DxLib_End() == -1)
	{
		return -1;
	};

	return 0;
}