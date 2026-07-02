#include "../Manager/ResourceManager.h"
#include "../Object/Common/Transform.h"
#include "Warpportal.h"

WarpPortal::WarpPortal(void)
	:
	pos_(POS)
{
}

WarpPortal::~WarpPortal(void)
{
}

void WarpPortal::Init(void)
{
	//リソースロード
	InitLoad();

	// 大きさ、回転、座標の初期化
	InitTransform();

	// 衝突判定の初期化
	InitCollider();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();
}

void WarpPortal::Update(void)
{

}

void WarpPortal::Draw(void)
{
	
}

void WarpPortal::Release(void)
{

}

void WarpPortal::InitLoad(void)
{
	//プレイヤーモデルのロード
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::PLAYER)->handleId_);

}

void WarpPortal::InitTransform(void)
{
}

void WarpPortal::InitCollider(void)
{
}

void WarpPortal::InitAnimation(void)
{
}

void WarpPortal::InitPost(void)
{
}
