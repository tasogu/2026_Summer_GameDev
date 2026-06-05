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

bool ColliderModel::CheckCollision(const ColliderBase* other)const
{
	switch (other->GetShape()) {
	case SHAPE::LINE:
		//カプセルとラインの当たり判定
		const ColliderLine* colliderLine = dynamic_cast<const ColliderLine*>(other);

		//当たり判定の実装
		if (colliderLine) {
			return CheckLineCollision(*this, *colliderLine);
		}
		break;
	}
	return false;

}

void ColliderModel::OnCollision(const ColliderBase* hit)const
{
}

bool ColliderModel::CheckLineCollision(const ColliderModel& a, const ColliderLine& b)const
{
	//モデルのハンドルID
	int Mv1Id = a.follow_->modelId;

	//線分の視点と終点を取得
	VECTOR LineStart = b.GetPosStart();
	VECTOR LineEnd = b.GetPosEnd();

	//衝突判定を実行
	MV1_COLL_RESULT_POLY_DIM hitRes = MV1CollCheck_LineDim(Mv1Id, -1, LineStart, LineEnd);

	// HitNum が 0 より大きければ衝突している
	if (hitRes.HitNum > 0)
	{
		return true;
	}

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
