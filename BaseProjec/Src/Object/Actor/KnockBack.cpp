#include "KnockBack.h"

KnockBack::KnockBack(float weifht, float decayRate_)
	:
	weight_(weifht),
	dir_({ 0.0f,0.0f,0.0f }),
	pow_(0.0f),
	decayRate_(decayRate_)
{

}

void KnockBack::Init(VECTOR knockBackVec, float knockBackPow)
{
	//ノックバック力
	pow_ = knockBackPow / weight_;

	//XZ平面のノックバック方向を求める
	VECTOR dirXZ = knockBackVec;
	dirXZ.y = 0.0f;
	dirXZ = VNorm(dirXZ);

	//上にはねる角度
	float angle = 40.0f * DX_PI_F / 180.0f;

	//上にはねる角度と、XZ平面方向に保ったまま、
	//最終的なノックバック方向を求める
	//（Y成分はsinで方向変換し、XZ平面成分をcosで弱める）
	VECTOR nkockBackDir = VECTOR();
	nkockBackDir.x = dirXZ.x * cosf(angle);
	nkockBackDir.z = dirXZ.z * cosf(angle);
	nkockBackDir.y = sinf(angle);

	//正規化
	dir_ = VNorm(nkockBackDir);

}

VECTOR KnockBack::GetMovePow(void) const
{
	return VScale(dir_, pow_);
}

void KnockBack::Decay(void)
{
	pow_ = pow_ * (1.0f - decayRate_);
}

bool KnockBack::IsEnd(void)
{
	return pow_ <= 0.1f;
}

