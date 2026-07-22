#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Object/Common/Transform.h"
#include "../Manager/ColliderManager.h"
#include "Actor/Character/Player.h"
#include "Actor/ColliderCapsule.h"
#include "Warpportal.h"

WarpPortal::WarpPortal(void)
	:
	isTouched_(false),
	targetTag_(ColliderBase::TAG::PLAYER),
	player_(nullptr),
	effectHandle_(-1)
{
}

WarpPortal::~WarpPortal(void)
{
	if (effectHandle_ != -1)
	{
		StopEffekseer3DEffect(effectHandle_);
	}
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

bool WarpPortal::IsTouched(void)
{
	return isTouched_;
}

void WarpPortal::Update(void)
{
	//トランスフォームの更新
	transform_.Update();

	//プレイヤーとの当たり判定
	CheckTouctPlayer();

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

	//デストラクタでの二重ストップを防ぐ
	effectHandle_ = -1;

	// 親の解放処理を呼ぶ！
	ActorBase::Release();
}

void WarpPortal::SetPlayer(Player* player)
{
	player_ = player;
}

void WarpPortal::CheckTouctPlayer(void)
{
	if (player_ == nullptr || isTouched_ == true)return;

	int CapsuleType = static_cast<int>(COLLIDER_TYPE::CAPSULE);
	
	//自分のコライダーの取得
	ColliderBase* WarpPortalCollider = GetOwnCollider(CapsuleType);

	//カプセルであるかの判定
	const ColliderCapsule* WarpPortalCapsule =
		dynamic_cast<const ColliderCapsule*>(WarpPortalCollider);

	//プレイヤーのコライダーの取得
	ColliderBase* PlayerCollider = player_->GetOwnCollider(CapsuleType);

	//カプセルであるかの判定
	const ColliderCapsule* PlayerCapsule =
		dynamic_cast<const ColliderCapsule*>(PlayerCollider);

	if (WarpPortalCapsule == nullptr || PlayerCapsule== nullptr) return;

	CollisionResult res = PlayerCapsule->CheckCollision(WarpPortalCapsule);

	if (res.isHit == false) {
		return;
	}
	else {
		isTouched_ = true;
		//printfDx("プレイヤーに当たった");
	}
}

