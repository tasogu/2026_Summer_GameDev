#include "../../../../Manager/ResourceManager.h"
#include "../../../../Application.h"
#include "../../../../Manager/ColliderManager.h"
#include "../../../Common/AnimationController.h"
#include "../../../Common/Transform.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../ColliderCapsule.h"
#include "../../ColliderLine.h"
#include "NomalEnemy.h"

NomalEnemy::NomalEnemy(const EnemyBase::EnemyData& data)
	:
	EnemyBase(data)
{
	hp_ = NOMAL_HP;
}

NomalEnemy::~NomalEnemy(void)
{
}

void NomalEnemy::Draw(void)
{

	CharactorBase::Draw();
}

void NomalEnemy::InitLoad(void)
{

	//transform_.SetModel(resMng_.Load(ResourceManager::SRC::NOMAL_ENEMY)->handleId_);
	//正しい書き方（必ず分身のハンドルをもらってセットする）
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::NOMAL_ENEMY));
	
}

void NomalEnemy::InitTransform(void)
{
	//プレイヤーの大きさ
	transform_.scl = SCALE;

	//プレイヤーの回転
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(ROT_Y),0.0f });

	//プレイヤーの座標
	transform_.Update();

}

void NomalEnemy::InitCollider(void)
{
	//主に地面との衝突で使用する線分コライダー	
	auto coiLine = std::make_unique< ColliderLine>(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);

	//線分コライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(coiLine.get());

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), std::move(coiLine));

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	auto colCapsule = std::make_unique< ColliderCapsule>(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);

	//カプセルコライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(colCapsule.get());

	colCapsule->SetOwner(this);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE),std::move( colCapsule));
}

void NomalEnemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	//animationController_ = new AnimationController(transform_.modelId);
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

}

void NomalEnemy::InitPost(void)
{
}


void NomalEnemy::UpdateProcess(void)
{
}

void NomalEnemy::UpdateProcessPost(void)
{
}

void NomalEnemy::ProcessAttack(void) 
{

}

void NomalEnemy::ProcessMove(void)
{

}