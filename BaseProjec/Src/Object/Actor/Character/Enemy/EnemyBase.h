#pragma once
#include "../CharactorBase.h"

class EnemyBase : public CharactorBase
{
public:
	enum class ENEMY_TYPE
	{
		NOMAL,
		BOSS,
	};

	//エネミーデータ
	struct EnemyData
	{
		int id;
		EnemyBase::ENEMY_TYPE type;
		int hp;
		VECTOR defaultPos;
		float movebleRange;
	};

	//コンストラクタ
	EnemyBase(const EnemyBase::EnemyData& data);

	//デストラクタ
	~EnemyBase(void) override;

	//描画
	virtual void Draw(void) = 0;

	////死亡判定を受け取る
	//bool IsDead(void) const { return isDead_; }

	////死亡させる
	//void Destroy(void) { isDead_ = true; }

private:
	
	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 80.0f;

	// リソースロード
	virtual void InitLoad(void) = 0;

	// 大きさ、回転、座標の初期化
	virtual void InitTransform(void) = 0;

	// 衝突判定の初期化
	void InitCollider(void) override;

	// アニメーションの初期化
	virtual void InitAnimation(void) = 0;

	// 初期化後の個別処理
	virtual void InitPost(void) = 0;

	//更新系(純粋仮想関数)
	virtual void UpdateProcess(void) = 0;
	virtual void UpdateProcessPost(void) = 0;

protected:
	//bool isDead_;

};

