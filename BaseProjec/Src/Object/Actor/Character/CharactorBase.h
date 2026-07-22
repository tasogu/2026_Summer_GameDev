#pragma once
#include <map>
#include<memory>
#include <Dxlib.h>
#include "../../Actor/ActorBase.h"
#include "../KnockBack.h"
class AnimationController;

class CharactorBase : public ActorBase 
{
public:

	//状態
	enum class STATE
	{
		NONE,
		PLAY,
		KNOCKBACK,
		KNOCKBACKEND,
		DEAD
	};

	//アニメーションタイプ
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		ROLL,
		KNOCKBACK,
		DEAD
	};


	//コンストラクタ
	CharactorBase(float weight, float decayRate);

	//デストラクタ
	virtual ~CharactorBase(void) override;

	//更新
	virtual void Update(void) override;

	//描画
	virtual void Draw(void) override;

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

	//剣のモデルID
	int imgSword_;

	//移動速度
	float speed_;

	//攻撃中かの判定
	bool isAttack_;

	//死亡判定を受け取る
	bool IsDead(void) const { return isDead_; }

	//死亡させる
	void Destroy(void) { isDead_ = true; }

	//ダメージ処理
	void OnDamage(int damage, VECTOR diff, float knockBackPow) override;

	KnockBack knockBack_;

protected:

	//アニメーションコントローラーの呼び出し
	//AnimationController* animationController_;
	std::unique_ptr<AnimationController> animationController_;

	//最大落下速度
	static constexpr float MAX_GALL_SPEED = -30.0f;

	//衝突時の牡鹿英s試行回数
	static constexpr int CNT_TRY_COLLISION = 20;

	//衝突時の牡鹿絵師量
	static constexpr float COLLISION_BACK_DIS = 1.0f;

	//地面と接しているかどうか
	bool isGround_;

	//手のボーンを取得する
	int handBoneid_;

	//HP
	float hp_;

	//死亡判定
	bool isDead_;

	//現在の状態を取得
	STATE state_;

	//更新系(純粋仮想関数)
	virtual void UpdateProcess(void) = 0;
	virtual void UpdateProcessPost(void) = 0;

	//更新ステップ(NONE)
	virtual void UpdateNone(void) = 0;

	//更新ステップ(PLAY)
	virtual void UpdatePlay(void) = 0;

	//Stateの切り替え
	virtual void ChangeState(STATE state) = 0;

	//Stateの切り替え(NONE)
	virtual void ChangeStateNone(void) = 0;

	//Stateの切り替え(PLAY)
	virtual void ChangeStatePlay(void) = 0;

	//アーマーの付与
	virtual bool IsArmor(void) const { return false; }

	//移動方向に応じた遅延回転
	void DelayRotate(void);

	//重力計算
	void CalcGravityPow(void);

	// 衝突判定
	virtual void CollisionReserve(void) {};
	void Collision(void);
	void CollisionGravity(void);

	void CollisionCapsule(void);

	// ノックバック開始処理
	virtual void OnStartKnockBack(void) {}

	// ノックバック更新処理
	void UpdateKnockBack(void);

	//ノックバックの終了処理
	virtual void OnEndKnockBack(void) {};
};

