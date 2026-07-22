#include <EffekseerForDXLib.h>
#include "../../Common/Transform.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/ColliderManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/SoundManager.h"
#include "../../../Utility/ModelFrameUtility.h"
#include "Enemy/NomalEnemy.h"
#include "../ColliderBase.h"
#include "../ColliderCapsule.h"
#include "CharactorBase.h"
#include "Sword.h"

Sword::Sword(const Transform& followTransform, int followFrameId)
	:
	targetTag_(),
	swordPow_(0),
	followTransform_(followTransform),
	followFrameId_(followFrameId),
	localPos_(SWORD_POS),
	localRot_({ AsoUtility::Deg2RadF(SWORD_LOCAL_ROTX), AsoUtility::Deg2RadF(SWORD_LOCAL_ROTY), SWORD_LOCAL_ROTZ }),
	swordTrajectory_(-1),
	effectHandle_(-1)
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

	ModelFrameUtility::SetFrameWorldMatrix(followTransform_, followFrameId_, transform_, localPos_, localRot_);

	//モデルの情報をTransformに反映させる
	//transform_.pos = MV1GetPosition(transform_.modelId);

	transform_.quaRot = Quaternion::GetRotation(transform_.matRot);

	//大きさ・回転・座標をモデルに反映
	transform_.Update();

	//エフェクトが再生中か
	int swordTrajectoryPlay = IsEffekseer3DEffectPlaying(swordTrajectory_);

	//エフェクトが再生中なら
	if (swordTrajectoryPlay == 0) {
		SetPosPlayingEffekseer3DEffect(swordTrajectory_, transform_.pos.x, transform_.pos.y, transform_.pos.z);

		VECTOR a = Quaternion::ToEuler(transform_.quaRot);
		SetRotationPlayingEffekseer3DEffect(swordTrajectory_, a.x, a.y, a.z);

	}

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

	//ソードの軌跡ロード
	effectHandle_ = resMng_.Load(ResourceManager::SRC::SWORD_TRAJECTORY)->handleId_;
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

void Sword::StartSlashEffect(void)
{
	//エフェクトが再生中か
	int swordTrajectoryPlay = IsEffekseer3DEffectPlaying(swordTrajectory_);

	//エフェクトが再生されていないなら
	if (swordTrajectoryPlay == -1) {

		swordTrajectory_ = PlayEffekseer3DEffect(effectHandle_);

		SetScalePlayingEffekseer3DEffect(swordTrajectory_, 5.0f, 25.0f, 1.0f);
	}
	//printfDx("eff=%d traj=%d\n", effectHandle_, swordTrajectory_);
}

void Sword::EndSlashEffect(void)
{
	if (swordTrajectory_ != -1)
	{
		StopEffekseer3DEffect(swordTrajectory_);
	}
}

void Sword::SetWeaponProperty(ColliderBase::TAG targetTag, float pow)
{
	//受け取ったタグを箱に保存
	targetTag_ = targetTag;

	//受け取った攻撃力を箱に保存
	swordPow_ = pow;

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

		CharactorBase* character = dynamic_cast<CharactorBase*>(owner);

		if (character == nullptr) continue;


		SoundManager::GetInstance().PlaySE(SoundManager::SE_ID::HIT);
		//printfDx("敵を倒した！\n");

		hitActors_.push_back(owner);

		// カプセルコライダ情報
		const ColliderCapsule* hitCapsule =
			dynamic_cast<const ColliderCapsule*>(hitCol);

		//ノックバック方向
		VECTOR diff = VSub(
			hitCapsule->GetCenter(),
			followTransform_.pos);

		//敵の死亡判定
		character->OnDamage(swordPow_, diff, 30.0f);

	}
}

void Sword::ResetStrike(void)
{
	hitActors_.clear();
}


