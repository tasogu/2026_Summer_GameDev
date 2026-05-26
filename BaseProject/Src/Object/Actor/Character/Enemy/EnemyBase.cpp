#include "EnemyBase.h"

EnemyBase::EnemyBase(const EnemyBase::EnemyData& data)
	:
	CharactorBase()


{
	//‰ŠúÀ•W‚Ì‚¹‚Ä’n
	transform_.pos = data.defaultPos;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Draw(void)
{
	CharactorBase::Draw();
}
