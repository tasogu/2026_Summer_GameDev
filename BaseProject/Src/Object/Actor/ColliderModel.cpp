#include "../Common/Transform.h"
#include "ColliderModel.h"


ColliderModel::ColliderModel(TAG tag, const Transform* follow)
	:
	ColliderBase(SHAPE::MODEL, tag, follow)
{
}

ColliderModel::~ColliderModel(void)
{
}

void ColliderModel::AddExcludeFrameIds(const std::string& name)
{
	// フレーム数を取得
	int num = MV1GetFrameNum(follow_->modelId);

	for (int i = 0; i < num; i++)
	{
		// フレーム名称を取得
		std::string frameName = MV1GetFrameName(follow_->modelId, i);

		// 指定文字列を含むなら除外対象
		if (frameName.find(name) != std::string::npos)
		{
			excludeFrameIds_.push_back(i);
		}
	}
}

void ColliderModel::ClearExcludeFrame(void)
{
	excludeFrameIds_.clear();
}

bool ColliderModel::IsTargetFrame(int frameIdx) const
{
	// ターゲットフレームが設定されている場合はホワイトリスト優先
	if (!targetFrameIds_.empty())
	{
		return (std::find(
			targetFrameIds_.begin(),
			targetFrameIds_.end(),
			frameIdx) != targetFrameIds_.end());
	}

	// ターゲットが無い場合は除外フレームを使う（除外されていない＝対象）
	return (std::find(
		excludeFrameIds_.begin(),
		excludeFrameIds_.end(),
		frameIdx) == excludeFrameIds_.end());
	//return false;
}

bool ColliderModel::IsExcludeFrame(int frameIdx) const
{
	// ターゲット（ホワイトリスト）がある場合は、
	// それ以外は全部「除外」
	if (!targetFrameIds_.empty())
	{
		return std::find(
			targetFrameIds_.begin(),
			targetFrameIds_.end(),
			frameIdx) == targetFrameIds_.end();
	}

	// ターゲットが無い場合は exclude リストの方を使う
	return std::find(
		excludeFrameIds_.begin(),
		excludeFrameIds_.end(),
		frameIdx) != excludeFrameIds_.end();
}

bool ColliderModel::CheckCollision(ColliderBase* other)
{
	return false;
}

void ColliderModel::AddTargetFrameIds(const std::string& name)
{
	int num = MV1GetFrameNum(follow_->modelId);
	for (int i = 0; i < num; i++)
	{
		std::string frameName = MV1GetFrameName(follow_->modelId, i);
		if (frameName.find(name) != std::string::npos)
		{
			targetFrameIds_.push_back(i);
		}
	}
}

void ColliderModel::ClearTargetFrame(void)
{
	targetFrameIds_.clear();
}
