#pragma once
#include <map>
#include <Dxlib.h>
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

	//移動方向
	VECTOR moveDir_;

	//位置
	VECTOR movePos_;

	//移動速度
	float speed_;


};

