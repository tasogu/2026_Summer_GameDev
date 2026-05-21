#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../Actor/ColliderModel.h"
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
	//MV1DrawModel(imgStage_);
	MV1DrawModel(transform_.modelId);

}

void Stage::Release(void)
{
//	MV1DeleteModel(imgStage_);
	transform_.Release();
}

void Stage::InitLoad(void)
{
	//imgStage_ = resMng_.Load(ResourceManager::SRC::STAGE).handleId_;
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::STAGE).handleId_);

}

void Stage::InitTransform(void)
{
	//ステージの大きさ、回転、座標の初期化
	//MV1SetScale(imgStage_, SCALE_);
	//MV1SetPosition(imgStage_, POS_);
	
	//ステージの大きさ,座標の初期化
	transform_.scl = SCALE_;
	transform_.pos = POS_;

	transform_.Update();
}

void Stage::InitCollider(void)
{
	//DxLib側の衝突情報セットアップ
	MV1SetupCollInfo(transform_.modelId);

	//モデルのコライダー
	ColliderModel* colModel =
		new ColliderModel(ColliderBase::TAG::STAGE, &transform_);

	//モデルのコライダーを登録
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);

}

void Stage::InitAnimation(void)
{
}

void Stage::InitPost(void)
{
}
