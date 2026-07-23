#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Application.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
	:
	SceneBase(),
	imgTitle_(-1),
	isStickHeld_(false),
	imgTitleLetter_(-1)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	SoundManager::GetInstance().PlayBGM(SoundManager::BGM_ID::TITLE);

	//画像読み込み
	imgTitle_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE)->handleId_;
	imgTitleLetter_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLELETTER)->handleId_;

	// 定点カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//カーソル位置
	cursorIndex_ = static_cast<int>(MENU_ITEM::START);
}

void TitleScene::Update(void)
{

	//auto const& ins = InputManager::GetInstance();
	//bool isAttackPad = ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

	//if (ins.IsTrgDown(KEY_INPUT_SPACE)|| isAttackPad)
	//{
	//	sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	//}

	auto const& ins = InputManager::GetInstance();

	// -------------------------------------------------------------
	// ① 入力の受付（キーボード ＋ コントローラーのスティック）
	// -------------------------------------------------------------
	bool isUpPressed = ins.IsTrgDown(KEY_INPUT_W) || ins.IsTrgDown(KEY_INPUT_UP);
	bool isDownPressed = ins.IsTrgDown(KEY_INPUT_S) || ins.IsTrgDown(KEY_INPUT_DOWN);

	// コントローラーの左スティックのベクトルを取得（Zが前後/上下）
	VECTOR stickInput = ins.GetLeftStickDir(InputManager::JOYPAD_NO::PAD1);

	// スティックがしっかりと上下に倒されているかチェック
	// （※InputManager内で前進が Z = +1.0f に正規化されているため、Zがプラスなら上、マイナスなら下になります）
	if (isStickHeld_ == false)
	{
		if (stickInput.z > 0.5f)      // スティックを上に強く倒した
		{
			isUpPressed = true;
			isStickHeld_ = true;     // 倒しっぱなしフラグをオンにして連打を防ぐ
		}
		else if (stickInput.z < -0.5f) // スティックを下に強く倒した
		{
			isDownPressed = true;
			isStickHeld_ = true;     // 倒しっぱなしフラグをオンにして連打を防ぐ
		}
	}
	else
	{
		// スティックが中央付近（デッドゾーン以下）に戻ったら、フラグを解除して次の入力を待つ
		if (stickInput.z < 0.2f && stickInput.z > -0.2f)
		{
			isStickHeld_ = false;
		}
	}

	// -------------------------------------------------------------
	// ② カーソル位置の更新（実際の移動処理）
	// -------------------------------------------------------------
	if (isUpPressed)
	{
		cursorIndex_--;
		SoundManager::GetInstance().PlaySE(SoundManager::SE_ID::CURSOR_MOVE);
		if (cursorIndex_ < 0) {
			cursorIndex_ = static_cast<int>(MENU_ITEM::MAX) - 1;
		}
	}
	else if (isDownPressed)
	{
		cursorIndex_++;
		SoundManager::GetInstance().PlaySE(SoundManager::SE_ID::CURSOR_MOVE);
		if (cursorIndex_ >= static_cast<int>(MENU_ITEM::MAX)) {
			cursorIndex_ = 0;
		}
	}

	// -------------------------------------------------------------
	// ③ 決定ボタンの処理
	// -------------------------------------------------------------
	// キーボードのSpace、またはコントローラーの「右ボタン（XBoxのB、PSの〇）」が押された瞬間
	bool isDecide = ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);

	if (isDecide)
	{
		SoundManager::GetInstance().PlaySE(SoundManager::SE_ID::DECIDE);

		if (cursorIndex_ == static_cast<int>(MENU_ITEM::START))
		{
			sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		}
		else if (cursorIndex_ == static_cast<int>(MENU_ITEM::EXIT))
		{
			Application::GetInstance().Quit(); // 前のステップで作った終了処理
		}
	}
}

void TitleScene::Draw(void)
{
	DrawGraph(0, 0, imgTitle_, true);


	int baseX = 500;
	int baseY = 400;
	int spaceY = 60; 

	// メニューの文字を描画
	//DrawString(450, 200, "リコールエッジ", GetColor(255, 255, 255));
	DrawGraph(200, -120, imgTitleLetter_, true);

	DrawString(baseX, baseY, "GAME START", GetColor(255, 255, 255));
	DrawString(baseX, baseY + spaceY, "EXIT", GetColor(255, 255, 255));

	// カーソル（矢印）の描画
	int cursorY = baseY + (cursorIndex_ * spaceY);
	DrawString(baseX - 40, cursorY, "->", GetColor(255, 255, 0)); // 黄色い矢印を描画
}
void TitleScene::Release(void)
{
	
}
