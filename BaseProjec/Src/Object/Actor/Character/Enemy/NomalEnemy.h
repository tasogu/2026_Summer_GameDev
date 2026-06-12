#pragma once
#include "EnemyBase.h"

class NomalEnemy : public EnemyBase
{
public:
	//コンストラクタ
	NomalEnemy(const EnemyBase::EnemyData& data);

	//デストラクタ
	virtual ~NomalEnemy(void) override;

	//アニメーションタイプ
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
	};

private:
	//スケール
	static constexpr VECTOR SCALE = { 1.0f, 1.0f, 1.0f };

	//回転
	static constexpr float ROT_Y = 180.0f;

	//位置
	static constexpr VECTOR POS = { 30.0f, 20.0f, 30.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 100.0f;
	
	//ノーマルエネミーの体力
	static constexpr int NOMAL_HP = 60.0f;

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// 衝突判定の初期化
	void InitCollider(void) override;

	//// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;


	//更新系
	virtual void UpdateProcess(void) override;
	virtual void UpdateProcessPost(void) override;

	virtual void CollisionReserve(void) {};

	//エネミーの攻撃
	void ProcessAttack(void);

};

