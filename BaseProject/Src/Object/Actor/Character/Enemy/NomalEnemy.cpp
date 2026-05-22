#include "../../../../Manager/ResourceManager.h"
#include "../../../Common/Transform.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../ColliderCapsule.h"
#include "../../ColliderLine.h"
#include "NomalEnemy.h"

NomalEnemy::NomalEnemy(void)
{
}

NomalEnemy::~NomalEnemy(void)
{
}

void NomalEnemy::Init(void)
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

void NomalEnemy::InitLoad(void)
{
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::PLAYER).handleId_);
}

void NomalEnemy::InitTransform(void)
{
	//プレイヤーの大きさ
	transform_.scl = SCALE;

	//プレイヤーの回転
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(ROT_Y),0.0f });

	//移動位置を初期化
	movePos_ = POS;

	//プレイヤーの座標
	transform_.pos = movePos_;

}

void NomalEnemy::InitCollider(void)
{
	//主に地面との衝突で使用する線分コライダー	
	ColliderLine* coiLine = new ColliderLine(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), coiLine);

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

}

void NomalEnemy::InitAnimation(void)
{
}

void NomalEnemy::InitPost(void)
{
}

void NomalEnemy::Update(void)
{
}

void NomalEnemy::Draw(void)
{
}

void NomalEnemy::Release(void)
{
}


void NomalEnemy::UpdateProcess(void)
{
}

void NomalEnemy::UpdateProcessPost(void)
{
}
