#pragma once
#include <DxLib.h>
class ResourceManager;

class HpBar 
{
public:
	HpBar(void);

	~HpBar(void);

	void Init(void);

	//HPのセット
	void SetProperty(float maxHp);

	//更新
	void Update(const VECTOR& worldPos, float nowHp);

	//描画
	void Draw(void);

public:
	// シングルトン参照
	ResourceManager& resMng_;


	//HPバーの表示幅
	static constexpr int BAR_WIDTH = 80;
	//HPバーの表示高さ
	static constexpr int BAR_HEIGHT = 10;

	//最大HP
	float maxHp_;

	//HP分母
	float nowHp_;

	//HPバーの位置
	VECTOR screenPos_;

	//画面内にいるか
	bool isVisible_;

	//HP枠
	int imgFrame_;

	//残り体力
	int imgRet_;

	//減った体力
	int imgBlack_;
};

