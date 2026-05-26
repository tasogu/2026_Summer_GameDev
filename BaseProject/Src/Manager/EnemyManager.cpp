#include "../Object/Actor/Character/Enemy/NomalEnemy.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager(void)
{
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{ 

	//エネミーのデータ読み込み
	LoadData();
}

void EnemyManager::Update(void)
{
	for (auto& enemy : enemies_)
	{
		enemy->Update();
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
		delete enemy;
	}
	enemies_.clear();
}

void EnemyManager::AddHitCollider( const ColliderBase* AddHitCollider)
{
	for (auto& enemy : enemies_)
	{
		enemy->AddHitCollider(AddHitCollider);
	}

}

void EnemyManager::LoadData(void)
{
	EnemyBase::EnemyData data = EnemyBase::EnemyData();

	//ID
	data.id = 0;

	//種別
	data.type = EnemyBase::ENEMY_TYPE::NOMAL;

	//HP
	data.hp = 10;

	//初期座標
	data.defaultPos = { 200.0f, 200.0f, 200.0f };

	//エネミー生成
	Create(data);
}

EnemyBase* EnemyManager::Create(const EnemyBase::EnemyData& data)
{
	EnemyBase* enemy = nullptr;

	//種別ごとに生成
	switch (data.type)
	{
	case EnemyBase::ENEMY_TYPE::NOMAL:
		enemy = new NomalEnemy(data);
		break;
	}

	//生成後Init
	enemy->Init();

	//配列に追加
	enemies_.push_back(enemy);

	return enemy;
}
