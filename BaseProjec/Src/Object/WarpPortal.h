#pragma once
#include <DxLib.h>
#include "Actor/ActorBase.h"
class Player;

class WarpPortal : public ActorBase
{
public:
	WarpPortal(void);

	~WarpPortal(void);

	void Update(void) override;

	void Draw(void);

	void Release(void);

	void SetPlayer(Player* player);

	void CheckTouctPlayer(void);
private:

	Player* player_;

	//初期位置
	static constexpr VECTOR POS = { 20.0f, 50.0f, 0.0f };  

	//衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0, 110.0f, 0.0f };

	//衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0, 50.0f, 0.0f };

	//衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 40.0f;

	//エフェクトハンドル
	int effectHandle_;

	//当たっているか
	bool isTouched_;

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// 衝突判定の初期化
	void InitCollider(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

	ColliderBase::TAG targetTag_;

};

