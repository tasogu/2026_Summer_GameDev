#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Object/Common/Transform.h"
#include "../Manager/ColliderManager.h"
#include "Actor/ColliderCapsule.h"
#include "Warpportal.h"

WarpPortal::WarpPortal(void)
{
}

WarpPortal::~WarpPortal(void)
{
}

void WarpPortal::InitLoad(void)
{
}

void WarpPortal::InitTransform(void)
{
	transform_.pos = POS;

}

void WarpPortal::InitCollider(void)
{
	// 主に壁や木などの衝突で仕様するカプセルコライダ
	auto colCapsule = std::make_unique<ColliderCapsule>(
		ColliderBase::TAG::WARPPORTAL, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);

	//カプセルコライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(colCapsule.get());

	colCapsule->SetOwner(this);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), std::move(colCapsule));

}

void WarpPortal::InitAnimation(void)
{
}

void WarpPortal::InitPost(void)
{

	int effectId = resMng_.Load(ResourceManager::SRC::WARPPORTAL)->handleId_;
	effectHandle_ = PlayEffekseer3DEffect(effectId);

	SetScalePlayingEffekseer3DEffect(effectHandle_, 30.0f, 20.0f, 30.0f);
}

void WarpPortal::Update(void)
{

	//トランスフォームの更新
	transform_.Update();

	// 毎フレーム、エフェクトを見えない基準点に追従させる！
	SetPosPlayingEffekseer3DEffect(effectHandle_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

void WarpPortal::Draw(void)
{
	ActorBase::Draw();
}

void WarpPortal::Release(void)
{
	// エフェクトの再生を止める！
	StopEffekseer3DEffect(effectHandle_);

	// 親の解放処理を呼ぶ！
	ActorBase::Release();
}

