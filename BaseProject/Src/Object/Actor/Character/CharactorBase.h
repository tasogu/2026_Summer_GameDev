#pragma once
#include "../../Actor/ActorBase.h"

class CharactorBase : public ActorBase
{
public:

	//コンストラクタ
	CharactorBase(void);

	//デストラクタ
	virtual ~CharactorBase(void) override;


	//移動量
	VECTOR movePow_;
};

