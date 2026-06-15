#include "../../../../Manager/ColliderManager.h"
#include "../../ColliderCapsule.h"
#include "../../ColliderLine.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const EnemyBase::EnemyData& data)
	:
	CharactorBase()


{
	isDead_ = false;
	//‰ŠúÀ•W‚Ì‚¹‚Ä’n
	transform_.pos = data.defaultPos;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::InitCollider(void)
{

}

