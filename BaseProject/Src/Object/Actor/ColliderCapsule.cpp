#include <DxLib.h>
#include "../Common/Transform.h"
#include "ColliderCapsule.h"

ColliderCapsule::ColliderCapsule(TAG tag, const Transform* follow,
	const VECTOR& localPosTop, const VECTOR& localPosDown, float radius)
	:
	ColliderBase(SHAPE::CAPSULE, tag, follow),
	localPosTop_(localPosTop),
	localPosDown_(localPosDown),
	radius_(radius)

{
}

ColliderCapsule::~ColliderCapsule(void)
{
}

const VECTOR& ColliderCapsule::GetLocalPosTap(void) const
{
	return localPosTop_;
}

const VECTOR& ColliderCapsule::GetLocalposDown(void) const
{
	return localPosDown_;
}

void ColliderCapsule::SetlocalPosTop(const VECTOR& pos)
{
	localPosTop_ = pos;
}

void ColliderCapsule::SetLocalPosDown(const VECTOR& pos)
{
	localPosDown_ = pos;
}

VECTOR ColliderCapsule::GetPosTop(void) const
{
	return GetRotPos(localPosTop_);
}

VECTOR ColliderCapsule::GetPosDown(void) const
{
	return GetRotPos(localPosDown_);
}

float ColliderCapsule::GetRadius(void) const
{
	return radius_;
}

void ColliderCapsule::SetRadius(float radius)
{
	radius_ = radius;
}

float ColliderCapsule::GetHeight(void) const
{
	return localPosTop_.y;
}

VECTOR ColliderCapsule::GetCenter(void) const
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();

	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}

VECTOR ColliderCapsule::GetPosPushBackAlongNormal(const MV1_COLL_RESULT_POLY& hitColPoly, int maxTryCnt, float pushDistance) const
{
	// コピー生成
	Transform tmpTransform = *follow_;
	ColliderCapsule tmpCapsule = *this;
	tmpCapsule.SetFollow(&tmpTransform);

	// 衝突補正処理
	int tryCnt = 0;
	while (tryCnt < maxTryCnt)
	{
		// カプセルと三角形の当たり判定
		if (!HitCheck_Capsule_Triangle(
			tmpCapsule.GetPosTop(), tmpCapsule.GetPosDown(),
			tmpCapsule.GetRadius(),
			hitColPoly.Position[0], hitColPoly.Position[1],
			hitColPoly.Position[2]))
		{
			break;
		}

		// 衝突していたら法線方向に押し戻し
		tmpTransform.pos =
			VAdd(tmpTransform.pos, VScale(hitColPoly.Normal, pushDistance));
		tryCnt++;
	}
	return tmpTransform.pos;

}

void ColliderCapsule::DrawDebug(int color)
{
	// 上の球体
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, 5, color, color, false);

	// 下の球体
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, 5, color, color, false);
	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// 球体を繋ぐ線(X+)
	dir = follow_->GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);

	// 球体を繋ぐ線(X-)
	dir = follow_->GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);

	// 球体を繋ぐ線(Z+)
	dir = follow_->GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);

	// 球体を繋ぐ線(Z-)
	dir = follow_->GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);

	// カプセルの中心
	DrawSphere3D(GetCenter(), 5.0f, 10, color, color, true);

}

bool ColliderCapsule::CheckCollision(const ColliderBase* other)const
{
	switch (other->GetShape()) {
	case SHAPE::LINE:
		//カプセルとラインの当たり判定
		break;
	case SHAPE::CAPSULE:
		//カプセルとカプセルの当たり判定
		const ColliderCapsule* otherCap = static_cast<const ColliderCapsule*> (other);

		//自分と相手を渡して判定を実行
		return CheckCollisionCapusle(*this, *otherCap);
	}
	return false;
}

void ColliderCapsule::OnCollision(const ColliderBase* hit)const
{
}

bool ColliderCapsule::CheckCollisionCapusle(const ColliderCapsule& a, const ColliderCapsule& b) const
{
	//カプセルa,bの上と下の取得,半径の取得
	VECTOR aTop = a.GetPosTop();
	VECTOR aDown = a.GetPosDown();
	float aRadius = a.GetRadius();
	VECTOR bTop = b.GetPosTop();
	VECTOR bDown = b.GetPosDown();
	float bRadius = b.GetRadius();

	//a,bの中心線を求める
	VECTOR aCenter = VScale(VAdd(aTop, aDown), 0.5f);
	VECTOR bCanter = VScale(VAdd(bTop, bDown), 0.5f);

	//半径の合計を計算
	float radiusSum = aRadius + bRadius;

	//距離を求める
	float dist = VSize(VSub(aCenter, bCanter));

	//距離が半径の合計より小さいなら衝突している
	return (dist < radiusSum);
}
