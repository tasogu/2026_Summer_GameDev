#pragma once
#include <vector>
#include "../Object/Actor/Character/Enemy/EnemyBase.h"
#include "../Object/Actor/ColliderBase.h"

class EnemyManager
{
public:
	//コンストラクタ
	EnemyManager(void);;

	//デストラクタ
	~EnemyManager(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Release(void);

	//エネミー
	const std::vector<EnemyBase*>& GetEemies(void) const { return enemies_; }

	//衝突対象となる古来だを登録
	void AddHitCollider(const ColliderBase* AddHitCollider);

	//CSV～敵の情報
	void LoadData(void);

	//エネミー生成
	EnemyBase* Create(const EnemyBase::EnemyData& data);
private:
	//エネミー
	std::vector<EnemyBase*> enemies_;

	//更新系(純粋仮想関数)
	void UpdateProcess(void);
	void UpdateProcessPost(void);


};

