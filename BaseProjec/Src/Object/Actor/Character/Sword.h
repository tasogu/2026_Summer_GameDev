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
	void UpdatePose(MATRIX handMatrix);

	// 多段ヒット（チェーンソー化）を防ぐためのメモ帳
	std::vector<ActorBase*> hitActors_;

	//攻撃の当たり判定とダメージ処理
	void ExecuteStrike(void);

	//攻撃の当たり判定とダメージ処理の解放
	void ResetStrike(void);

	//標的とするタグと攻撃力をセットする処理
	void SetWeaponProperty(ColliderBase::TAG targetTag, float pow);

private:
	//スケール
	static constexpr VECTOR SCALE = { 0.01f, 0.01f, 0.01f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 100.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 0.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 5.0f;

	//剣に加える回転の角度
	static constexpr float ROTX = 180.0f;
	static constexpr float ROTY = -20.0f;
	static constexpr float ROTZ = -90.0f;


	//剣の攻撃力
	int swordPow_;

	//攻撃対象のタグを持つ
	ColliderBase::TAG targetTag_;

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


	float a = 0.0f;
};

