#pragma once
#include "../ActorBase.h"
#include ",,/../../../../Utility/AsoUtility.h"
class Sword : public ActorBase
{
public:

	Sword(const Transform& followTransform, int followFrameId);
	~Sword(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	////プレイヤーから行列を受け取って糸を反映するメソッド
	//void UpdatePose(MATRIX handMatrix);

	// 多段ヒット（チェーンソー化）を防ぐためのメモ帳
	std::vector<ActorBase*> hitActors_;

	//攻撃の当たり判定とダメージ処理
	void ExecuteStrike(void);

	//攻撃の当たり判定とダメージ処理の解放
	void ResetStrike(void);

	//標的とするタグと攻撃力をセットする処理
	void SetWeaponProperty(ColliderBase::TAG targetTag, float pow);

	//剣の軌跡エフェクト再生
	void StartSlashEffect(void);

	//剣の軌跡エフェクト終了
	void EndSlashEffect(void);
private:
	//スケール
	static constexpr VECTOR SCALE = { 0.01f, 0.01f, 0.01f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 100.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, -40.0f, 0.0f };

	//剣の持ち方の修正
	static constexpr VECTOR SWORD_POS = { 0.0f, 10.f, -4.0f };

	static constexpr float SWORD_LOCAL_ROTX =- 55.0f;
	static constexpr float SWORD_LOCAL_ROTY =  -90.0f;
	static constexpr float SWORD_LOCAL_ROTZ = 0.0f ;


	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 5.0f;

	//剣に加える回転の角度
	static constexpr float ROTX = 0.0f;
	static constexpr float ROTY = 0.0f;
	static constexpr float ROTZ = 0.0f;


	//剣の攻撃力
	int swordPow_;

	//剣の軌跡エフェクト再生ハンドル
	int swordTrajectory_;

	//剣の軌跡エフェクトリソースハンドル
	int effectHandle_;

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

	//手に対する相対位置(握り調整)
	VECTOR localPos_;

	//手に対する相対回転(剣の向き調整)
	VECTOR localRot_;

protected:
	//追従先Transform
	const Transform& followTransform_;

	//追従対象のフレームID
	int followFrameId_;
};

