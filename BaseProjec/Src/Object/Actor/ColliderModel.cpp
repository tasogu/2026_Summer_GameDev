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

CollisionResult ColliderModel::CheckCollision(const ColliderBase* other)const
{
	//当たっていない状態の構造体を作っておく
	CollisionResult result;
	result.isHit = false;

	if (follow_ == nullptr || follow_->modelId == -1) {
		return result;
	}

	switch (other->GetShape()) {
	case SHAPE::LINE:
		//カプセルとラインの当たり判定
		const ColliderLine* colliderLine = dynamic_cast<const ColliderLine*>(other);

		//当たり判定の実装
		if (colliderLine) {
			return CheckLineCollision(*colliderLine);
		}
		break;
	}
	return result;

}

void ColliderModel::OnCollision(const ColliderBase* hit, const CollisionResult& res)const
{
}

CollisionResult ColliderModel::CheckLineCollision( const ColliderLine& b)const
{
	CollisionResult res;
	res.isHit = false;

	MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_LineDim(follow_->modelId, -1, b.GetPosStart(), b.GetPosEnd());

	//最も高いYを持つヒット点を探す
	double bestHitY = -DBL_MAX;
	for (int i = 0; i < hits.HitNum; i++)
	{
		//除外フレーム
		if (!IsTargetFrame(hits.Dim[i].FrameIndex)) continue;

		//ヒット点のYがこれまでの最高値より高いなら、結果を更新
		if (hits.Dim[i].HitPosition.y > bestHitY)
		{
			bestHitY = hits.Dim[i].HitPosition.y;
			res.hitPos = hits.Dim[i].HitPosition;
			res.normal = hits.Dim[i].Normal; 
			res.isHit = true;
		}
	}

	// 結果構造体を返す前に、ヒット点の配列の後始末をする
	MV1CollResultPolyDimTerminate(hits);

	// 結果を返す
	return res;
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
