#include "../../Actor/ColliderCapsule.h"
#include "../../Actor/ColliderLine.h"
#include "../../Actor/ColliderModel.h"
#include "../../Common/AnimationController.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../../Src/Application.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/ColliderManager.h"
#include "CharactorBase.h"

CharactorBase::CharactorBase(void)
	:
	ActorBase(),
	isGround_(false),
	prevPos_(),
	movePow_(),
	moveDir_(),
	movePos_(),
	jumpPow_(),
	speed_(),
	animationController_(nullptr)


{
}

CharactorBase::~CharactorBase(void)
{
}

void CharactorBase::Update(void)
{
	OutputDebugString("DEBUG: CharactorBase::Update() is running!\n");

	//移動前座標を保存
	prevPos_ = transform_.pos;

	//各キャラクターごとの更新処理
	UpdateProcess();

	//移動方向に応じた遅延回転
	DelayRotate();

	//重力による移動量
	CalcGravityPow();

	//衝突判定
	Collision();

	//画面に反映
	transform_.Update();

	//アニメーションの更新
	animationController_->Update();

	//各キャラクターごとの更新の処理
	UpdateProcessPost();


}

void CharactorBase::Draw(void)
{

	ActorBase::Draw();
}

void CharactorBase::DelayRotate(void)
{
	// 移動方向から回転に変換する
	Quaternion goalRot = Quaternion::LookRotation(moveDir_);

	// 回転の補間
	transform_.quaRot =
		Quaternion::Slerp(transform_.quaRot, goalRot, 0.2f);
}

void CharactorBase::CalcGravityPow(void)
{

	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Application::GetInstance().GetGravityPow() *
		scnMng_.GetDeltaTime();

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// 落下速度の上限を設定
	if (jumpPow_.y < MAX_GALL_SPEED)
	{
		jumpPow_.y = MAX_GALL_SPEED;
	}
}

void CharactorBase::Collision(void)
{
	//移動処理
	transform_.pos = VAdd(transform_.pos, movePow_);

	//衝突(カプセル)
	CollisionCapsule();

	//重力移動量を足す
	transform_.pos = VAdd(transform_.pos, jumpPow_);

	//衝突判定のリセット
	isGround_ = false;

	// 衝突(重力)
	CollisionGravity();
}

void CharactorBase::CollisionGravity(void)
{

	// 線分コライダ
	int lineType = static_cast<int>(COLLIDER_TYPE::LINE);

	// 線分コライダが無ければ処理を抜ける
	if (ownColliders_.count(lineType) == 0) return;

	// 線分コライダ情報
	ColliderLine* colliderLine_ =
		dynamic_cast<ColliderLine*>(ownColliders_.at(lineType).get());

	if (colliderLine_ == nullptr) return;

	// 線分の始点と終点を取得
	VECTOR s = colliderLine_->GetPosStart();
	VECTOR e = colliderLine_->GetPosEnd();

	// 登録されている衝突物を全てチェック
	for (const auto& hitCol : ColliderManager::GetInstance().GetColliders())
	{

		//ポインタがnullならコンティニュー
		if (hitCol == nullptr) continue;

		// ステージ以外は処理を飛ばす
		if (hitCol->GetTag() != ColliderBase::TAG::STAGE) continue;

		//メソッドで判定結果を受け取る
		CollisionResult res = hitCol->CheckCollision(colliderLine_);

		//判定結果を使って位置修正
		if (res.isHit)
		{
			transform_.pos = res.hitPos;
			isGround_ = true;
		}
	}

	if (isGround_)
	{
		jumpPow_ = AsoUtility::VECTOR_ZERO;
	}
}

void CharactorBase::CollisionCapsule(void)
{
	// カプセルコライダ
	int capsuleType = static_cast<int>(COLLIDER_TYPE::CAPSULE);

	// カプセルコライダが無ければ処理を抜ける
	if (ownColliders_.count(capsuleType) == 0) return;

	// カプセルコライダ情報
	ColliderCapsule* colliderCapsule =
		dynamic_cast<ColliderCapsule*>(ownColliders_.at(capsuleType).get());
	if (colliderCapsule == nullptr) return;

	// 登録されている衝突物を全てチェック
	for (const auto& hitCol : ColliderManager::GetInstance().GetColliders())
	{
		// モデル以外は処理を飛ばす
		if (hitCol->GetShape() != ColliderBase::SHAPE::MODEL) continue;

		// 派生クラスへキャスト
		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);
		if (colliderModel == nullptr) continue;
		auto hits = MV1CollCheck_Capsule(
			colliderModel->GetFollow()->modelId, -1,
			colliderCapsule->GetPosTop(), colliderCapsule->GetPosDown(),
			colliderCapsule->GetRadius());

		// 衝突した複数のポリゴンと衝突回避するまで、
		// プレイヤーの位置を移動させる
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];

			if (!colliderModel->IsTargetFrame(hit.FrameIndex))
			{
				continue;
			}

			// 指定された回数と距離で三角形の法線方向に押し戻す
			transform_.pos =
				colliderCapsule->GetPosPushBackAlongNormal(
					hit, CNT_TRY_COLLISION, COLLISION_BACK_DIS);

			// 地面と異なり、衝突回避位置が不明なため、何度か移動させる
			// この時、移動させる方向は、移動前座標に向いた方向であったり、
			// 衝突したポリゴンの法線方向だったりする
			for (int tryCnt = 0; tryCnt < CNT_TRY_COLLISION; tryCnt++)
			{
				// 再度、モデル全体と衝突検出するには、効率が悪過ぎるので、
				// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
				int pHit = HitCheck_Capsule_Triangle(
					colliderCapsule->GetPosTop(), colliderCapsule->GetPosDown(),
					colliderCapsule->GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					// 法線の方向にちょっとだけ移動させる
					transform_.pos =
						VAdd(transform_.pos,
							VScale(hit.Normal, COLLISION_BACK_DIS));
					continue;
				}
				break;
			}
		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);
	}
}

void CharactorBase::OnDamage(int damage)
{
	hp_ -= damage;
		if (hp_ <= 0) {
			Destroy();
		}
}

