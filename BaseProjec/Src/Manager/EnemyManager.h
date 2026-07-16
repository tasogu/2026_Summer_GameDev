#pragma once
#include <vector>
#include "../Object/Actor/Character/Enemy/EnemyBase.h"
#include "../Object/Actor/ColliderBase.h"
#include "../Scene/StageCommon.h"

class Player;

class EnemyManager
{
public:
	//コンストラクタ
	EnemyManager(STAGE_TYPE stageType);

	//デストラクタ
	~EnemyManager(void);

	//初期化
	void Init();

	//更新
	void Update(Player* player);

	//描画
	void Draw(void);

	//解放
	void Release(void);

	//エネミー死亡
	bool IsAllDead(void);

	//エネミー
	const std::vector<std::shared_ptr<EnemyBase>>& GetEemies(void) const { return enemies_; }

	//CSV～敵の情報
	void LoadData(void);

	//エネミー生成
	std::shared_ptr<EnemyBase> Create(const EnemyBase::EnemyData& data);
private:

	//ステージの種類
	STAGE_TYPE stageType_;

	//エネミー
	std::vector<std::shared_ptr<EnemyBase>> enemies_;

	//更新系(純粋仮想関数)
	void UpdateProcess(void);
	void UpdateProcessPost(void);

	//押し出し
	void ProcessPushOut(Player* player);
};

