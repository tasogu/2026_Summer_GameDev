#pragma once
#include <memory>
#include "SceneBase.h"
#include "../Manager/ColliderManager.h"
class Stage;
class Player;
class EnemyManager;

class GameScene : public SceneBase
{

public:
	
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

	//ポーズシーン
	bool isPause_;
	
	////コライダー
	//ColliderManager* collder_;
};
