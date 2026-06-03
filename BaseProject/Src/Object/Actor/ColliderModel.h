#pragma once
#include<string>
#include "ColliderBase.h"

class ColliderModel : public ColliderBase
{
public:
	// コンストラクタ
	ColliderModel(TAG tag, const Transform* follow);

	// デストラクタ
	~ColliderModel(void) override;

	// 指定された文字を含むフレームを衝突判定から除外
	void AddExcludeFrameIds(const std::string& name);

	// 衝突判定から除外するフレームをクリアする
	void ClearExcludeFrame(void);

	// 追加：対象フレーム（ホワイトリスト）
	void AddTargetFrameIds(const std::string& name);

	void ClearTargetFrame();

	// 除外フレーム判定
	bool IsTargetFrame(int frameIdx) const;

	bool IsExcludeFrame(int frameIdx) const;
	//bool IsTargetFrameNo(int frameIdx) const;

		// 衝突判定
	bool CheckCollision(ColliderBase* other) override;

protected:
	// 衝突判定から除外するフレーム番号
	std::vector<int> excludeFrameIds_;

	std::vector<int> targetFrameIds_;   // 衝突対象

	// デバッグ用描画
	void DrawDebug(int color) override {};


};
