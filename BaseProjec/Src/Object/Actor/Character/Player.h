#pragma once
#include "CharactorBase.h"
//class AnimationController;
#include "../../../Scene/StageCommon.h"

class Sword;

class Player : public CharactorBase
{
public:
	//コンストラクタ
	Player(STAGE_TYPE stageType);

	//デストラクタ
	~Player(void);

	//初期化
	void Init(void);

	////更新
	//void Update(void);

	//描画
	void Draw(void) override;

	//解放
	void Release(void) override;

	//HP取得
	bool IsDead(void);
private:
	//ステージ(１～２)
	STAGE_TYPE stageType_;

	////アニメーションコントローラーの呼び出し
	//AnimationController* animationController_;

	//スケール
	static constexpr VECTOR SCALE = { 1.0f, 1.0f, 1.0f };

	//回転
	static constexpr float ROT_Y =180.0f;

	//座標
	static constexpr VECTOR POS = { 0.0f, 1000.0f, 0.0f };

	//移動速度(歩き)
	static constexpr float SPEED_MOVE = 180.0f;

	//移動速度(走り)
	static constexpr float SPEED_RUN = 360.0f;

	//回転時間
	static constexpr float TIME_ROT = 1.0f;

	//衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	//衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	//衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };

	//衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	//衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;

	//剣のプレイヤーでの攻撃力
	static constexpr float PLAYER_POW = 10.0f;

	//プレイヤー体力
	static constexpr float PLAYER_HP = 100.0f;

	//ソードのポインタ
	//Sword* sword_;
	std::unique_ptr<Sword> sword_;

	//モデル描画
	int imgPlayer_;

	//ジャストパリー
	float parryTimer_;

	//回転する方向最初
	Quaternion playerRotY_;

	//回転する方向最後
	Quaternion goalQuaRot_;

	//回転時間
	float stepRotTime_;

	//----------------------------------------
	//回避中か
	bool isEvasion_;
	//回避の残り時間
	float evasionTime_;
	//回避の再使用待ち時間
	float evasionCoolTime_;
	//回避方向
	VECTOR evasionDir_;

	//回避の持続秒数
	static constexpr float EVASION_TIME = 0.45f;
	//回避の移動速度
	static constexpr float EVASION_SPEED = 800.0f;
	//回避の再使用待ち秒数
	static constexpr float EVASION_COOL = 0.8f;

	void OnDamage(int damage) override;


	//HPバー用画像ハンドル
	int imgHpFrame_;
	int imgHpRed_;
	int imgHpBlack_;
	//----------------------------------------------

	//更新系(純粋仮想関数継承)
	void UpdateProcess(void) override;
	void UpdateProcessPost(void) override;

	//更新ステップ(NONE)
	void UpdateNone(void) override;

	//更新ステップ(PLAY)
	void UpdatePlay(void)override;

	//Stateの切り替え
	void ChangeState(STATE state) override;

	//Stateの切り替え(NONE)
	void ChangeStateNone(void) override;

	//Stateの切り替え(PLAY)
	void ChangeStatePlay(void) override;

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

	//プレイヤーの移動
	void ProcessMove(void);

	//プレイヤーの攻撃
	void ProcessAttack(void);

	//プレイヤー回避
	void ProcessEvasion(void);

	//回転したい角度を設定
	void SetGoalRotate(double rotRad);

	//移動方向への回転
	void Rotate(void);


};