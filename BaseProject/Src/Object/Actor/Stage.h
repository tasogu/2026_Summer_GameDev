#pragma once
#include "../Actor/ActorBase.h"

class Stage : public ActorBase
{
public:
	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		MODEL = 0,
		MAX,
	};

	
	//コンストラクタ
	Stage(void);
	//デストラクタ
	~Stage(void);

	//初期化
	void Init(void);

	//更新
	void Update(void) override;

	//描画
	void Draw(void) override;

	//解放
	void Release(void) override;

private:
	
	//ステージのスケール
	static constexpr VECTOR SCALE_ = { 1.0f , 1.0f, 1.0f};

	//ステージの座標
	static constexpr VECTOR POS_ = { 0.0f, 0.0f, 0.0f };



	//ステージのモデル
	int imgStage_;

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

