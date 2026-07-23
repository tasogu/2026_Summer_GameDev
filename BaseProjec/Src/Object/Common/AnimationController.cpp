#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
	:
	modelId_(modelId),
	playType_(-1),
	playAnim_(),
	isLoop_(true)
{
}

AnimationController::~AnimationController(void)
{
	Release();
}

void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;

	Add(type, speed, animation);
}

void AnimationController::AddInFbx(int type, float speed, int animIndex)
{
	Animation animation;
	animation.model = -1;
	animation.animIndex = animIndex;

	Add(type, speed, animation);
}

void AnimationController::Play(int type, bool isLoop, float blendTime)
{

	if (priorityType_ == type)
	{
		// 同じアニメーションだったら再生を継続する
		return;
	}

	if (priorityType_ != -1)
	{
		auto& oldAnimation = animations_[priorityType_];
		oldAnimation.isPriority = false;
		oldAnimation.weight = oldAnimation.blendRate;
	}

	auto& newAnimation = animations_[type];
	if (newAnimation.attachNo = -1) {
		if (newAnimation.model == -1) {
			newAnimation.attachNo = MV1AttachAnim(modelId_, 0, newAnimation.animIndex);
		}
		else {
			newAnimation.attachNo = MV1AttachAnim(modelId_, 0, newAnimation.model);
		}
	}

	newAnimation.step = 0;
	newAnimation.blendStep = 0;
	newAnimation.blendRate = 0;
	newAnimation.isPriority = true;
	newAnimation.isLoop = isLoop;

	priorityType_ = type;
	blendTime_ = blendTime;
	isBlending_ = true;

	//// アニメーション種別を変更
	//playType_ = type;
	//playAnim_ = animations_[type];

	//// 初期化
	//playAnim_.step = 0.0f;

	//// モデルにアニメーションを付ける
	//if (playAnim_.model == -1)
	//{
	//	// モデルと同じファイルからアニメーションをアタッチする
	//	playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	//}
	//else
	//{
	//	// 別のモデルファイルからアニメーションをアタッチする
	//	// DxModelViewerを確認すること(大体0か1)
	//	int animIdx = 0;
	//	playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);
	//}

	//// アニメーション総時間の取得
	//playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	//// アニメーションループ
	//isLoop_ = isLoop;

}

void AnimationController::Update(void)
{

	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	if (isBlending_ == true && priorityType_ != -1) {
		auto& precedeAnimation = animations_[priorityType_];
		precedeAnimation.blendStep += deltaTime;
		precedeAnimation.blendRate = precedeAnimation.blendStep / blendTime_;
		if (precedeAnimation.blendRate >= 1.0) {
			precedeAnimation.blendRate = 1.0;
			isBlending_ = false;
		}
	}

	for (auto& pair : animations_) {
		auto& anim = pair.second;
		if (anim.attachNo == -1)continue;

	}
	//// 再生
	//playAnim_.step += (deltaTime * playAnim_.speed);

	//// アニメーションが終了したら
	//if (playAnim_.step > playAnim_.totalTime)
	//{
	//	if (isLoop_)
	//	{
	//		// ループ再生
	//		playAnim_.step = 0.0f;
	//	}
	//	else
	//	{
	//		// ループしない
	//		playAnim_.step = playAnim_.totalTime;
	//	}
	//}

	//// アニメーション設定
	//MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);

}

void AnimationController::Release(void)
{

	// 外部FBXのモデル(アニメーション)解放
	for (const std::pair<int, Animation>& pair : animations_)
	{
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	}
	
	// 可変長配列をクリアする
	animations_.clear();
	
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

bool AnimationController::IsEnd(int animType) const
{

	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}

const AnimationController::Animation& AnimationController::GetPlayAnim(void) const
{
	return playAnim_;
}

float AnimationController::GetTime(void) const
{
	return playAnim_.step;
}

void AnimationController::Add(int type, float speed, Animation& animation)
{
	animation.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 追加
		animations_.emplace(type, animation);
	}
}
