#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "Stage.h"

Stage::Stage(void)
	:
	imgStage_(-1)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	//リソースロード
	InitLoad();

	//	// 大きさ、回転、座標の初期化
	InitTransform();

	// 衝突判定の初期化
	InitCollider();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();



}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	MV1DrawModel(imgStage_);

}

void Stage::Release(void)
{
	MV1DeleteModel(imgStage_);
}

void Stage::InitLoad(void)
{
	imgStage_ = resMng_.Load(ResourceManager::SRC::STAGE).handleId_;

}

void Stage::InitTransform(void)
{
	//ステージの大きさ、回転、座標の初期化
	MV1SetScale(imgStage_, SCALE_);
	MV1SetPosition(imgStage_, POS_);
	

}

void Stage::InitCollider(void)
{
}

void Stage::InitAnimation(void)
{
}

void Stage::InitPost(void)
{
}
