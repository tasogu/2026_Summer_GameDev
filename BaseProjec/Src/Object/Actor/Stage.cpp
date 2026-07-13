#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/ColliderManager.h"
#include "../Actor/ColliderModel.h"
#include "Stage.h"

Stage::Stage(STAGE_TYPE stageType)
	:
	imgStage_(-1)
{
	stageType_ = stageType;
}

Stage::~Stage(void)
{
}

void Stage::Init()
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
	switch (stageType_) {
	case STAGE_TYPE::STAGE1:
		transform_.SetModel(resMng_.Load(ResourceManager::SRC::STAGE)->handleId_);
		break;
	case STAGE_TYPE::STAGE2:
		transform_.SetModel(resMng_.Load(ResourceManager::SRC::STAGE2)->handleId_);
		break;

	}
	//imgStage_ = resMng_.Load(ResourceManager::SRC::STAGE).handleId_;

}

void Stage::InitTransform(void)
{
	
	switch (stageType_) {
	case STAGE_TYPE::STAGE1:
		//ステージの大きさ,座標の初期化
		transform_.scl = SCALE_;
		transform_.pos = POS_;

		transform_.Update();
		break;

	case STAGE_TYPE::STAGE2:
		//ステージの大きさ,座標の初期化
		transform_.scl = SCALE_;
		transform_.pos = POS_;

		transform_.Update();

		break;
	}
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

	//ステージを当たり判定に登録
	ColliderManager::GetInstance().Register(colModel);

}

void Stage::InitAnimation(void)
{
}

void Stage::InitPost(void)
{
}
