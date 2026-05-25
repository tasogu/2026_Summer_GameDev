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
	virtual ~EnemyBase(void) override;

	//更新
	virtual void Update(void) override;

	//描画
	virtual void Draw(void) override;

	//解放
	virtual void Release(void) override;

private:
	
	// リソースロード
	void InitLoad(void) override = 0;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override = 0;

	// 衝突判定の初期化
	void InitCollider(void) override = 0;

	// アニメーションの初期化
	void InitAnimation(void) override = 0;

	// 初期化後の個別処理
	void InitPost(void) override = 0;


	//更新系(純粋仮想関数)
	virtual void UpdateProcess(void) = 0;
	virtual void UpdateProcessPost(void) = 0;

};

