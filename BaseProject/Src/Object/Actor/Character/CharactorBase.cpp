#include "../../Actor/ColliderCapsule.h"
#include "../../Actor/ColliderLine.h"
#include "../../Actor/ColliderModel.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../../Src/Application.h"
#include "../../../Manager/SceneManager.h"
#include "CharactorBase.h"

CharactorBase::CharactorBase(void)
	:
	ActorBase()
{
}

CharactorBase::~CharactorBase(void)
{
}

void CharactorBase::Update(void)
{
	//重力による移動量
	CalcGravityPow();

	//衝突判定
	Collision();
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

	transform_.pos = VAdd(transform_.pos, jumpPow_);
}

void CharactorBase::Collision(void)
{

	CollisionCapsule();

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
		dynamic_cast<ColliderLine*>(ownColliders_.at(lineType));

	if (colliderLine_ == nullptr) return;

	// 線分の始点と終点を取得
	VECTOR s = colliderLine_->GetPosStart();
	VECTOR e = colliderLine_->GetPosEnd();
	//VECTOR s = VAdd(transform_.pos, VGet(0, 0, 0));
	//VECTOR e = VAdd(transform_.pos, VGet(0, -50.0f, 0));

	// 登録されている衝突物を全てチェック
	for (const auto& hitCol : hitColliders_)
	{
		// ステージ以外は処理を飛ばす
		if (hitCol->GetTag() != ColliderBase::TAG::STAGE) continue;

		// 派生クラスへキャスト
		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);

		if (colliderModel == nullptr) continue;
		const float descendThreshold = 0.9f;

		// ステージモデル(地面)との衝突（全ポリゴンを取得）
		auto hits = MV1CollCheck_LineDim(
			colliderModel->GetFollow()->modelId, -1, s, e);

		bool found = false;
		VECTOR bestHitPos = AsoUtility::VECTOR_ZERO;
		double bestHitY = -DBL_MAX;

		// 全ヒットから「除外フレームを無視」しつつ最も Y が大きいものを選ぶ
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];

			// 除外フレームは無視する（IsTargetFrame が除外リストを示す実装になっている前提）
			if (!colliderModel->IsTargetFrame(hit.FrameIndex))
			{
				continue;
			}

			// Y がより大きい（上にある）ヒットを採用
			if (hit.HitPosition.y > bestHitY)
			{
				bestHitY = hit.HitPosition.y;
				bestHitPos = hit.HitPosition;
				found = true;
			}
		}

		// 最適なヒットが見つかったら一度だけ位置補正を行う
		if (found)
		{
			// 衝突地点より少し上に移動（押し出す）
			if (transform_.pos.y < bestHitPos.y)
			{
				transform_.pos = VAdd(bestHitPos, VScale(AsoUtility::DIR_U, 2.0f));
			}

		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);
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
		dynamic_cast<ColliderCapsule*>(ownColliders_.at(capsuleType));
	if (colliderCapsule == nullptr) return;

	// 登録されている衝突物を全てチェック
	for (const auto& hitCol : hitColliders_)
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
