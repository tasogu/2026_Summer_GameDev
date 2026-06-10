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
}

NomalEnemy::~NomalEnemy(void)
{
}

void NomalEnemy::InitLoad(void)
{

	transform_.SetModel(resMng_.Load(ResourceManager::SRC::NOMAL_ENEMY).handleId_);

	
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
	ColliderLine* coiLine = new ColliderLine(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), coiLine);

	//線分コライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(coiLine);

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

	//カプセルコライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(colCapsule);

	colCapsule->SetOwner(this);
}

void NomalEnemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(transform_.modelId);

}

void NomalEnemy::InitPost(void)
{
}


void NomalEnemy::UpdateProcess(void)
{
}

void NomalEnemy::UpdateProcessPost(void)
{
	if (hp_ == 0) {
	}
}

void NomalEnemy::OnDamege(int damege)
{
	Destroy();
}
