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
	void UpdatePose(VECTOR pos, Quaternion playerRot);

private:
	//スケール
	static constexpr VECTOR SCALE = { 0.05f, 0.05f, 0.05f };

	//	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 20.0f, -45.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 20.0f, 10.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 5.0f;

	//剣に加える回転の角度
	static constexpr float ROT = 135.0f;

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

