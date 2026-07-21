#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "HpBar.h"

HpBar::HpBar(void)
	:
	maxHp_(0),
	nowHp_(0),
	screenPos_(AsoUtility::VECTOR_ZERO),
	isVisible_(false),
	imgFrame_(0),
	imgRet_(0),
	imgBlack_(0),
	resMng_(ResourceManager::GetInstance())
{
}

HpBar::~HpBar(void)
{
}
void HpBar::Init(void)
{
	//HP画像(フレーム、体力)
	imgFrame_ = resMng_.Load(ResourceManager::SRC::HP_FRAME)->handleId_;
	imgRet_  = resMng_.Load(ResourceManager::SRC::HP_RET)->handleId_;
	imgBlack_ = resMng_.Load(ResourceManager::SRC::HP_BLACK)->handleId_;

}

void HpBar::SetProperty(float maxHp)
{
	//HPMAXの設定
	maxHp_ = maxHp;
}

void HpBar::Update(const VECTOR& worldPos, float nowHp)
{
	//HPの保存
	nowHp_ = nowHp;

	//3Dを画像ピクセルに変換
	screenPos_ = ConvWorldPosToScreenPos(worldPos);

	//カメラ正面
	isVisible_ = (screenPos_.z > 0.0f && screenPos_.z < 1.0f);
}

void HpBar::Draw(void)
{
	//カメラに映るなら描画
	if (isVisible_ == false) return;

	int barX = static_cast<int>(screenPos_.x) - BAR_WIDTH / 2;
	int barY = static_cast<int>(screenPos_.y);

	//HPの割合
	float rate = nowHp_ / maxHp_;
	if (rate <= 0.0f) {
		rate = 0.0f;
	}
	else if (rate >= 1.0f) {
		rate = 1.0f;
	}


	//黒背景(全幅)
	DrawExtendGraph(barX, barY,
		barX + BAR_WIDTH, barY + BAR_HEIGHT,
		imgBlack_, TRUE);

	//赤(割合の幅だけ)
	DrawRectExtendGraph(
		barX, barY,
		barX + static_cast<int>(BAR_WIDTH * rate), barY + BAR_HEIGHT,
		0, 0,
		static_cast<int>(1080 * rate), 153,
		imgRet_, TRUE);

	//枠(一番上)
	DrawExtendGraph(barX, barY,
		barX + BAR_WIDTH, barY + BAR_HEIGHT,
		imgFrame_, TRUE);
}

