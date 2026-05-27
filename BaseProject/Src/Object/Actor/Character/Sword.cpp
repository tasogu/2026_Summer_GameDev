#include ",,/../../../../Utility/AsoUtility.h"
#include "../../Common/Transform.h"
#include "../../../Manager/ResourceManager.h"
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
}

void Sword::Draw(void)
{
	MV1DrawModel(transform_.modelId);
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
}

void Sword::InitAnimation(void)
{
}

void Sword::InitPost(void)
{
}

void Sword::UpdatePose(VECTOR pos)
{
	// 例：剣のグリップ分だけ位置をずらす
	VECTOR offset = VGet(0.0f, -50.0f, -70.0f); 
	VECTOR finalPos = VAdd(pos, offset);

	//行列の合成: プレイあy－の世界行列 * 右手ボーンの相対位置
	MV1SetPosition(transform_.modelId, finalPos);

	//モデルによって軸は異なります。90度ずつ試してください。
	VECTOR adjustmentRot = VGet(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);

	// 手の回転(rot)に調整用回転(adjustmentRot)を加えて適用
	MV1SetRotationXYZ(transform_.modelId, VAdd(transform_.rot, adjustmentRot));

	//スケール
	MV1SetScale(transform_.modelId, SCALE);
}
