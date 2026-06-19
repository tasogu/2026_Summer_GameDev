#include ",,/../../../../Utility/AsoUtility.h"
#include "../../Common/Transform.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/ColliderManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/SoundManager.h"
#include "Enemy/NomalEnemy.h"
#include "../ColliderBase.h"
#include "../ColliderCapsule.h"
#include "CharactorBase.h"
#include "Sword.h"

Sword::Sword(void)
	:
	targetTag_(),
	swordPow_(0)
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

	transform_.Update();
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
	//複製のハンドルをもらってロード
	//transform_.SetModel(resMng_.Load(ResourceManager::SRC::SWORD)->handleId_);
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SWORD));

}

void Sword::InitTransform(void)
{
	transform_.scl = SCALE;

}

void Sword::InitCollider(void)
{
	//剣のモデル空間でのカプセルの橋と橋の相対空間を定義
	auto col = std::make_unique <ColliderCapsule>(
		ColliderBase::TAG::SWORD,
		&transform_, COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);

	//当たり判定リストに登録
	ColliderManager::GetInstance().Register(col.get());

	col->SetOwner(this);

	ownColliders_.emplace(static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE), std::move(col));

}

void Sword::InitAnimation(void)
{
}

void Sword::InitPost(void)
{
	int frameIndex = MV1SearchFrame(transform_.modelId, "Plane001");
	if (frameIndex != -1) {
		MV1SetFrameVisible(transform_.modelId, frameIndex, FALSE); // 非表示にする
	}
}

void Sword::SetWeaponProperty(ColliderBase::TAG targetTag, float pow)
{
	//受け取ったタグを箱に保存
	targetTag_ = targetTag;

	//受け取った攻撃力を箱に保存
	swordPow_ = pow;

}

void Sword::UpdatePose(MATRIX handMatrix)
{
	//プレイヤーの回転行列
	MATRIX rotMat = handMatrix;

	a += 0.1f;

	//剣の構え角度
	MATRIX rotX = MGetRotX(AsoUtility::Deg2RadF(ROTX));
	MATRIX rotY = MGetRotY(AsoUtility::Deg2RadF(ROTY));
	//MATRIX rotY = MGetRotX(AsoUtility::Deg2RadF(a));
	MATRIX rotZ = MGetRotZ(AsoUtility::Deg2RadF(ROTZ));

	MATRIX stanceMat = MMult(MMult(rotZ, rotX), rotY);

	//握り位置のズレ
	VECTOR LocalOffset = VGet(0.0f, 0.0f, 0.0f);
	MATRIX offsetMat = MGetTranslate(LocalOffset);

	//回転させてずらす
	MATRIX localMat = MMult(stanceMat, offsetMat);

	//最終的な回転行列(剣の構え×プレイヤーの向き)
	MATRIX finalMat = MMult(localMat, rotMat);

	//行列からワールド座標を抜き出す
	transform_.pos = VGet(finalMat.m[3][0], finalMat.m[3][1], finalMat.m[3][2]);

	//行列からクォータニオンを抜き出す
	transform_.quaRot = Quaternion::GetRotation(finalMat);

	//自身の行列を更新
	transform_.Update();
}

void Sword::ExecuteStrike(void)
{
	//カプセルコライダー
	int CapsuleType = static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE);
	ColliderBase* swordColliderBase = GetOwnCollider(CapsuleType);

	// カプセルコライダ情報
	const ColliderCapsule* swordCapsule =
		dynamic_cast<const ColliderCapsule*>(swordColliderBase);


	// 登録されている衝突物を全てチェック
	for (const auto& hitCol : ColliderManager::GetInstance().GetColliders()) {

		//ターゲット以外は飛ばす
		if (hitCol->GetTag() != targetTag_) continue;

		//攻撃判定
		CollisionResult res = hitCol->CheckCollision(swordCapsule);

		//攻撃が当たった
		if (!res.isHit) continue;
		ActorBase* owner = hitCol->GetOwner();

		if (owner == nullptr) continue;

		auto it = std::find(hitActors_.begin(), hitActors_.end(), owner);

		if (it != hitActors_.end()) continue;

		//NomalEnemy* enemy = dynamic_cast<NomalEnemy*>(owner);

		CharactorBase* character = dynamic_cast<CharactorBase*>(owner);

		if (character == nullptr) continue;

		//敵の死亡判定
		character->OnDamage(swordPow_);

		SoundManager::GetInstance().PlaySE(SoundManager::SE_ID::HIT);
		//printfDx("敵を倒した！\n");

		hitActors_.push_back(owner);

	}
}

void Sword::ResetStrike(void)
{
	hitActors_.clear();
}


