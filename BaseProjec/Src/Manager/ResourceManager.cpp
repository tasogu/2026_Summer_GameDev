#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	dummy_ = std::make_shared<Resource>();

	// 推奨しませんが、どうしても使いたい方は
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	std::shared_ptr<Resource> res;

	//タイトル画像
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Title/Title.png"); 
	resourcesMap_.emplace(SRC::TITLE, res);

	//タイトル文字
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "Title/TitleLetter.png");
	resourcesMap_.emplace(SRC::TITLELETTER, res);

	//ステージモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Stage/MainStage/MainStage.mv1");
	resourcesMap_.emplace(SRC::STAGE, res);

	//ステージモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Stage/MainStage/MainStage.mv1");
	resourcesMap_.emplace(SRC::STAGE2, res);

	//プレイヤーモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);

	//剣のモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Sword/sword1.mv1");
	resourcesMap_.emplace(SRC::SWORD, res);

	//剣の軌跡effect
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "Sword/SwordTrajectory.efkefc");
	resourcesMap_.emplace(SRC::SWORD_TRAJECTORY, res);

	//ノーマルエネミーモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + "Enemy/Enemy.mv1");
	resourcesMap_.emplace(SRC::NOMAL_ENEMY, res);

	//ワープポイントのエフェクト
	res = std::make_shared<RES>(RES_T::EFFEKSEER, PATH_EFF + "Warp/WalpStar.efkefc");
	resourcesMap_.emplace(SRC::WARPPORTAL, res);

	//HPバーの赤画像
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "HP/Ret.png");
	resourcesMap_.emplace(SRC::HP_RET, res);

	//HPバーの黒エフェクト
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "HP/Black.png");
	resourcesMap_.emplace(SRC::HP_BLACK, res);

	//HPバーの枠エフェクト
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "HP/Frame.png");
	resourcesMap_.emplace(SRC::HP_FRAME, res);

	//GAMEOVER
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER, res);

	//GAMECLEAR
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + "GameClear.png");
	resourcesMap_.emplace(SRC::GAMECLEAR, res);

}

void ResourceManager::Release(void)
{
	loadedMap_.clear();

}

void ResourceManager::Destroy(void)
{
	Release();
	//for (auto& res : resourcesMap_)
	//{
	//	res.second->Release();
	//	delete res.second;
	//}
	//resourcesMap_.clear();
	//delete instance_;
	delete instance_;

	instance_ = nullptr;

}

std::shared_ptr<const Resource> ResourceManager::Load(SRC src)
{
	std::shared_ptr<const Resource> res = _Load(src);
	if (!res || res->type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	std::shared_ptr<Resource> res = _Load(src);
	if (!res ||res->type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

std::shared_ptr<Resource> ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	auto it = loadedMap_.find(src);
	if (it != loadedMap_.end())
	{
		return it->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	rPair->second->Load();

	// ロード処理
	auto res = rPair->second;

	// 念のためコピーコンストラクタ
	loadedMap_.try_emplace(src, res);

	return res;

}
