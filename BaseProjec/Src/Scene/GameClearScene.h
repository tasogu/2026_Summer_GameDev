#pragma once
#include "SceneBase.h"	

class GameClearScene :public SceneBase
{
public:
	GameClearScene(void);

	~GameClearScene(void) override;

	// ‰Šú‰»
	void Init(void) override;

	// XV
	void Update(void) override;

	// •`‰æ
	void Draw(void) override;

	// ‰ğ•ú
	void Release(void) override;

	int imgGameClear;
};

