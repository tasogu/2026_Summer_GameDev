#pragma once
#include "CharactorBase.h"

class Player : public CharactorBase
{
public:

	//状態
	enum class STATE
	{
		NONE,
		PLAY,
	};

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
	static constexpr VECTOR SCALE = { 1.0f, 1.0f, 1.0f };

	//回転
	static constexpr float ROT_Y =180.0f;

	//座標
	static constexpr VECTOR POS = { 0.0f, 40.0f, 0.0f };

	//移動速度(歩き)
	static constexpr float SPEED_MOVE = 3.0f;

	//移動速度(走り)
	static constexpr float SPEED_RUN = 6.0f;

	//モデル描画
	int imgPlayer_;

	//現在の状態を取得
	STATE state_;

	//更新ステップ(NONE)
	void UpdateNone(void);

	//更新ステップ(PLAY)
	void UpdatePlay(void);

	//Stateの切り替え
	void ChangeState(STATE state);

	//Stateの切り替え(NONE)
	void ChangeStateNone(void);

	//Stateの切り替え(PLAY)
	void ChangeStatePlay(void);

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
	void ProcessMove(void);

};