#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "GemeOver.h"

GemeOver::GemeOver(void)
{
}

GemeOver::~GemeOver(void)
{
}

void GemeOver::Init(void)
{
    // 必要なら画像をロード：imgClear_ = resMng_.Load(...);
}

void GemeOver::Update(void)
{
    // スペースキーでタイトルに戻るなどの処理
    auto const& ins = InputManager::GetInstance();
    if (ins.IsTrgDown(KEY_INPUT_SPACE))
    {
        sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
    }
}

void GemeOver::Draw(void)
{
    // 画面中央にテキストを描画
    DrawFormatString(540, 360, GetColor(255, 255, 255), "GAME OVER!");
}

void GemeOver::Release(void)
{
    // 確保したリソースの解放処理があればここに書く
}