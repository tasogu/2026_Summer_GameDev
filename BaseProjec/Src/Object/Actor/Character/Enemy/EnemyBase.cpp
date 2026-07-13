#include "../../../../Manager/ColliderManager.h"
#include "../../ColliderCapsule.h"
#include "../../ColliderLine.h"
#include "../Player.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const EnemyBase::EnemyData& data)
	:
	CharactorBase()


{
	isDead_ = false;

	transform_.pos = data.defaultPos;
}

EnemyBase::~EnemyBase(void)
{
}

bool EnemyBase::IsWithinCirclingRange(VECTOR targetPlayer, float goal)
{
	VECTOR myPos = transform_.pos;

	VECTOR diff = VSub(targetPlayer, myPos);

	float distance = VSize(diff);

	//ゴール地点より遠い
	if (distance >= goal) {
		return false;
	}
	//ゴール地点より近い
	else if (distance <= goal) {
		return true;
	}
}

void EnemyBase::AddPosCorrection(const VECTOR& correction)
{
	transform_.pos = VAdd(transform_.pos, correction);
}

void EnemyBase::InitCollider(void)
{

}

