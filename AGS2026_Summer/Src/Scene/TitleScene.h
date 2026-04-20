#pragma once
#include "SceneBase.h"
class SceneManager;

class TitleScene : public SceneBase
{
public:

	//コンストラクタ
	TitleScene(void);

	//デストラクタ
	~TitleScene(void);

	//初期化
	void Init(void) override;

	//更新
	void Update(void) override;

	//解更
	void Draw(void) override;

private:

	//画像
	int imgTitle_;

};

