#pragma once
#include <map>
#include <Dxlib.h>
#include "../../Actor/ActorBase.h"

class CharactorBase : public ActorBase
{
public:
	//衝突判定種別
	enum class COLLIDER_TYPE
	{
		LINE,
		CAPSULE,
		MAX,
	};

	//コンストラクタ
	CharactorBase(void);

	//デストラクタ
	virtual ~CharactorBase(void) override;

	//更新
	virtual void Update(void) override;

	//移動前座標を保存する
	VECTOR prevPos_;

	//移動量
	VECTOR movePow_;

	//移動方向
	VECTOR moveDir_;

	//位置
	VECTOR movePos_;

	//ジャンプ後
	VECTOR jumpPow_;

	//移動速度
	float speed_;

protected:
	//最大落下速度
	static constexpr float MAX_GALL_SPEED = -30.0f;

	//衝突時の牡鹿英s試行回数
	static constexpr int CNT_TRY_COLLISION = 20;

	//衝突時の牡鹿絵師量
	static constexpr float COLLISION_BACK_DIS = 1.0f;

	//更新系(純粋仮想関数)
	virtual void UpdateProcess(void) = 0;
	virtual void UpdateProcessPost(void) = 0;

	//重力計算
	void CalcGravityPow(void);

	// 衝突判定
	virtual void CollisionReserve(void) {};
	void Collision(void);
	void CollisionGravity(void);

	void CollisionCapsule(void);

};

