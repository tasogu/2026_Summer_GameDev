#pragma once
#include <memory>
#include "SceneBase.h"
#include "StageCommon.h"
#include "../Manager/ColliderManager.h"
class Stage;
class Player;
class EnemyManager;
class Fader;
class WarpPortal;

class GameScene : public SceneBase
{

public:
	//ステージチェンジ
	enum class GAME_TYPE
	{
		PLAY,
		WAIT_PORTAL,
		FADEOUT,
		MEMORY_SWAP,
		FADEIN
	};


	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

private:

	//ステージ
	//Stage* stage_;
	std::unique_ptr<Stage> stage_;

	//プレイヤー
	//Player* player_;
	std::unique_ptr<Player> player_;

	//エネミー
	//EnemyManager* enemy_;
	std::unique_ptr<EnemyManager> enemy_;

	//フェード
	std::unique_ptr<Fader> fader_;

	//ワープポータル
	std::unique_ptr<WarpPortal> warpPortal_;

	//ロードフレーム
	int loadStep_;

	//ポーズシーン
	bool isPause_;

	//ゲームのタイプ
	GAME_TYPE gameType_;

	//ステージの種類
	STAGE_TYPE stageType_;

	////コライダー
	//ColliderManager* collder_;

	//再初期化用
	void LoadNextStage();

};
