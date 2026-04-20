#pragma once
#include "SceneBase.h"

class GameScene : public SceneBase
{
public:
	//コンストラクタ
	GameScene(void);

	//デストラクタ
	~GameScene(void);

	//初期化
	void Init(void) override;

	//更新
	void Update(void) override;

	//解更
	void Draw(void) override;

private:

};

