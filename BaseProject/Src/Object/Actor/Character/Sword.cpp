#include ",,/../../../../Utility/AsoUtility.h"
#include "../../Common/Transform.h"
#include "../../../Manager/ResourceManager.h"
#include "../ColliderBase.h"
#include "../ColliderCapsule.h"
#include "CharactorBase.h"
#include "Sword.h"

Sword::Sword(void)
	:
	imgSword_(-1)
{
}

Sword::~Sword(void)
{
}

void Sword::Init(void)
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

void Sword::Update(void)
{
	//モデルの情報をTransformに反映させる
	transform_.pos = MV1GetPosition(transform_.modelId);

}

void Sword::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//デバッグ用のコライダーの描画
	ActorBase::Draw();
}

void Sword::InitLoad(void)
{
	//剣のモデルのロード
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::SWORD).handleId_);

}

void Sword::InitTransform(void)
{
	transform_.scl = SCALE;

}

void Sword::InitCollider(void)
{
	//剣のモデル空間でのカプセルの橋と橋の相対空間を定義
	ColliderCapsule* col = new ColliderCapsule(
		ColliderBase::TAG::SWORD,
		&transform_, COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE), col);
}

void Sword::InitAnimation(void)
{
}

void Sword::InitPost(void)
{

}

void Sword::UpdatePose(VECTOR pos, Quaternion playerRot)
{
	//プレイヤーの回転行列
	MATRIX rotMat = playerRot.ToMatrix();

	//手から見た剣のオフセットをプレイヤーの向きに合わせて回転させる
	VECTOR LocalOffset = VGet(0.0f, 40.0f, 15.0f);
	VECTOR worldOffset = VTransform(LocalOffset, rotMat);
	VECTOR finalPos = VAdd(pos, worldOffset);

	//剣の構え用の回転行れる
	MATRIX stanceMat = MGetRotX(AsoUtility::Deg2RadF(ROT));

	//最終的な回転行列(剣の構え×プレイヤーの向き)
	MATRIX finalMat = MMult(stanceMat, rotMat);

	//適用
	//MV1SetPosition(transform_.modelId, finalPos);
	//MV1SetRotationMatrix(transform_.modelId, finalMat);
	//MV1SetScale(transform_.modelId, SCALE);

	transform_.pos = finalPos;

	transform_.quaRot = Quaternion::GetRotation(finalMat);

	transform_.Update();
}
