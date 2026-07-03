#pragma once
#include <DxLib.h>
#include "Actor/ActorBase.h"

class WarpPortal : public ActorBase
{
public:
	WarpPortal(void);

	~WarpPortal(void);

	void Update(void) override;

	void Release(void);

private:

	//初期位置
	static constexpr VECTOR POS = { 20.0f, 50.0f, 0.0f };  

	//エフェクトハンドル
	int effectHandle_;

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

};

