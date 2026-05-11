#pragma once
#include "CharactorBase.h"

class Player : public CharactorBase
{
public:

	//コンストラクタ
	Player(void);

	//デストラクタ
	~Player(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Release(void);

private:

	//スケール
	static constexpr VECTOR SCALE_ = { 1.0f, 1.0f, 1.0f };

	//回転
	static constexpr float ROT_Y = 180.0f * DX_PI_F / 180.0f;

	//座標
	static constexpr VECTOR POS_ = { 0.0f, 40.0f, 0.0f };

	//モデル描画
	int imgPlayer_;

		// リソースロード
	void InitLoad(void);

	// 大きさ、回転、座標の初期化
	void InitTransform(void);

	// 衝突判定の初期化
	void InitCollider(void);

	// アニメーションの初期化
	void InitAnimation(void);

	// 初期化後の個別処理
	void InitPost(void);

	//プレイヤーの移動
	void Move(void);
};