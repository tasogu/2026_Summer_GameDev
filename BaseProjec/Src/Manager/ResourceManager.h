#pragma once
#include <map>
#include <memory>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE,
		STAGE,
		STAGE2,
		PLAYER,
		SWORD,
		NOMAL_ENEMY,
		IDLE,
		WALK,
		RUN,
		WARPPORTAL,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	std::shared_ptr<const Resource> Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, std::shared_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, std::shared_ptr<Resource>> loadedMap_;

	std::shared_ptr<Resource> dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	std::shared_ptr<Resource> _Load(SRC src);

};
