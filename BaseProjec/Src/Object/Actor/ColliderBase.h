#pragma once
#include <DxLib.h>
#include <vector> 
#include <memory> 
class Transform;
class ColliderBase;
class ActorBase;

// 衝突結果
struct CollisionResult {
	bool isHit;			//当たったか
	VECTOR hitPos;		//どこに当たったか
	VECTOR normal;		//どの向きか
	int frameIndex;		//どのパーツか
	const ColliderBase* hitCollider;	//誰と当たったか
};

class ColliderBase
{
public:
	// 形状
	enum class SHAPE
	{
		NONE,
		LINE,
		SPHERE,
		CAPSULE,
		MODEL,
	};

	// 衝突種別
	enum class TAG
	{
		STAGE,
		PLAYER,
		CAMERA,
		ENEMY,
		SWORD,
		WARPPORTAL
	};



	// コンストラクタ
	ColliderBase(SHAPE shape, TAG tag, const Transform* follow);

	// デストラクタ
	virtual ~ColliderBase(void);

	// 描画
	void Draw(void);

	// 追従先の取得
	const Transform* GetFollow(void) const { return follow_; };

	// 追従先の再設定
	void SetFollow(Transform* follow);

	// 形状
	SHAPE GetShape(void) const { return shape_; }

	// 衝突種別
	TAG GetTag(void) const { return tag_; }

	// 衝突判定
	virtual CollisionResult CheckCollision(const ColliderBase* other)const = 0;

	// 衝突時の処理
	virtual void OnCollision(const ColliderBase* hit, const CollisionResult& res)const = 0;

	//持ち主を返す
	ActorBase* GetOwner(void) const { return owner_; }

	void SetOwner(ActorBase* owner) { owner_ = owner; }

protected:
	// デバッグ表示の色
	static constexpr int COLOR_VALID = 0xff0000;
	static constexpr int COLOR_INVALID = 0xaaaaaa;

	// 形状
	SHAPE shape_;

	// 衝突種別
	TAG tag_;

	// 追従先
	const Transform* follow_;

	ActorBase* owner_ = nullptr;

	// 有効フラグ
	bool isValid_;

	// ローカル座標をワールド座標に変換
	VECTOR GetRotPos(const VECTOR& localPos) const;

	// デバッグ用描画
	virtual void DrawDebug(int color) = 0;


};

