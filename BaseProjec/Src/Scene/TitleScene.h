#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

private:
	//メニュー項目
	enum class MENU_ITEM
	{
		START,
		EXIT,
		MAX
	};

	//減じあのカーソル位置
	int cursorIndex_;

	//タイトル画像
	int imgTitle_;

	//タイトル文字
	int imgTitleLetter_;

	//スティックの連打防止用フラグ
	bool isStickHeld_;
};
