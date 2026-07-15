#pragma once
#include <DxLib.h>
#include <cmath>
class KnockBack
{
public:

	KnockBack(float weifht,float decayRate_);
		
	void Init(VECTOR knockBackVec, float knockBackPow);

	VECTOR GetMovePow(void)const;

	void Decay(void);

	bool IsEnd(void) ;

private:
	//重さ
	float weight_;

	//減衰率
	float decayRate_;

	//ノックバック方向
	VECTOR dir_;

	//ノックバック力
	float pow_;

};

