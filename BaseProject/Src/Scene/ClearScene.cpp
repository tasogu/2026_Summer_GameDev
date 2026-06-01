#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "ClearScene.h"

ClearScene::ClearScene(void) : SceneBase(), imgClear_(-1) {}

ClearScene::~ClearScene(void) {}

void ClearScene::Init(void)
{
    // 必要なら画像をロード：imgClear_ = resMng_.Load(...);
}

void ClearScene::Update(void)
{
    // スペースキーでタイトルに戻るなどの処理
    auto const& ins = InputManager::GetInstance();
    if (ins.IsTrgDown(KEY_INPUT_SPACE))
    {
        sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
    }
}

void ClearScene::Draw(void)
{
    // 画面中央にテキストを描画
    DrawFormatString(540, 360, GetColor(255, 255, 255), "GAME CLEAR!");
    DrawFormatString(480, 400, GetColor(255, 255, 255), "Press SPACE to Title");
}

void ClearScene::Release(void)
{
    // 確保したリソースの解放処理があればここに書く
}