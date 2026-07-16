#include "../Object/Actor/Character/Enemy/NomalEnemy.h"
#include "../Object/Actor/Character/Player.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager(STAGE_TYPE stageType )
{
	//ステージの種類を保存
	stageType_ = stageType;

}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init()
{ 
	

	//エネミーのデータ読み込み
	LoadData();
}

void EnemyManager::Update(Player* player)
{
	for (auto& enemy : enemies_)
	{
		enemy->Update(player);
	}

	//押し出し
	ProcessPushOut(player);

	//死んだ敵だけを消す
	for (auto it = enemies_.begin(); it != enemies_.end(); )
	{
		if ((*it)->IsDead())
		{		
			(*it)->Release();
			it = enemies_.erase(it);
		}
		else
		{		
			++it;
		}
	}
}

void EnemyManager::Draw(void)
{
	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}

}

void EnemyManager::Release(void)
{
	for (auto& enemy : enemies_)
	{
		enemy->Release();
	}
	enemies_.clear();
}

bool EnemyManager::IsAllDead(void)
{
	return enemies_.empty();
}

void EnemyManager::LoadData(void)
{
	EnemyBase::EnemyData data = EnemyBase::EnemyData();


	switch (stageType_) {
	case STAGE_TYPE::STAGE1:
		//一体目
		data.id = 0;
		//種別
		data.type = EnemyBase::ENEMY_TYPE::NOMAL;
		//HP
		data.hp = 60;
		//初期座標
		data.defaultPos = { 200.0f, 200.0f, 200.0f };
		//エネミー生成
		Create(data);

		//二体目
		data.id = 1;
		//初期座標
		data.defaultPos = { 250.0f, 0.0f, 150.0f };
		//生成
		Create(data);
		break;
	case STAGE_TYPE::STAGE2:
		//一体目
		data.id = 0;
		//種別
		data.type = EnemyBase::ENEMY_TYPE::NOMAL;
		//HP
		data.hp = 60;
		//初期座標
		data.defaultPos = { 200.0f, 100.0f, 200.0f };
		//エネミー生成
		Create(data);

		//二体目
		data.id = 1;
		//初期座標
		data.defaultPos = { 450.0f, 100.0f, 150.0f };
		//生成
		Create(data);

		//四体目
		data.id = 1;
		//初期座標
		data.defaultPos = { 560.0f, 100.0f, 250.0f };
		//生成
		Create(data);

		break;

	}
}

std::shared_ptr<EnemyBase> EnemyManager::Create(const EnemyBase::EnemyData& data)
{
	std::shared_ptr<EnemyBase> enemy = nullptr;

	//種別ごとに生成
	switch (data.type)
	{
	case EnemyBase::ENEMY_TYPE::NOMAL:
		enemy = std::make_shared<NomalEnemy>(data);
		break;
	}

	//生成後Init
	enemy->Init();

	//配列に追加
	enemies_.push_back(enemy);

	return enemy;
}

void EnemyManager::ProcessPushOut(Player* player)
{
	//敵 vs プレイヤー
	for (auto& enemy : enemies_)
	{
		if (enemy->IsPushable() == false)continue;
		VECTOR diff = VSub(enemy->GetTransform().pos, player->GetTransform().pos);
		diff.y = 0.0f;                             // XZ平面のみ
		float distance = VSize(diff);
		float limit = 40.0f;                       // 半径合計(仮。後述)

		if (distance >= limit) continue;           // 重なってない

		VECTOR pushDir;
		if (distance <= 0.0f) {
			pushDir = { 1.0f, 0.0f, 0.0f };        // 同座標ガード
		}
		else {
			pushDir = VNorm(diff);
		}
		float overlap = limit - distance;          // めり込み量
		enemy->AddPosCorrection(VScale(pushDir, overlap));
	}

	//敵 vs 敵(互いに半分ずつ退く)
	for (size_t i = 0; i < enemies_.size(); ++i)
	{
		for (size_t j = i + 1; j < enemies_.size(); ++j)
		{
			if (enemies_[i]->IsPushable() == false) continue;
			if (enemies_[j]->IsPushable() == false) continue;

			VECTOR diff = VSub(enemies_[i]->GetTransform().pos,
				enemies_[j]->GetTransform().pos);
			diff.y = 0.0f;
			float distance = VSize(diff);
			float limit = 40.0f;               // 敵半径20+敵半径20

			if (distance >= limit) continue;

			VECTOR pushDir;
			if (distance <= 0.0f) {
				pushDir = { 1.0f, 0.0f, 0.0f };
			}
			else {
				pushDir = VNorm(diff);
			}

			float half = (limit - distance) * 0.5f;   // めり込みの半分
			enemies_[i]->AddPosCorrection(VScale(pushDir, half));
			enemies_[j]->AddPosCorrection(VScale(pushDir, -half));   // 逆方向
		}
	}
}
