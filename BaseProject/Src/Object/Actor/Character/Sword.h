#pragma once
#include "../ActorBase.h"
class Sword : public ActorBase
{
public:
	Sword(void);
	~Sword(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//プレイヤーから行列を受け取って糸を反映するメソッド
	void UpdatePose(VECTOR pos);

private:
	//スケール
	static constexpr VECTOR SCALE = { 0.1f, 0.1f, 0.1f };

	//剣のモデルID
	int imgSword_;

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

