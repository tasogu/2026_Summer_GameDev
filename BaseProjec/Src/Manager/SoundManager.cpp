#include <DxLib.h>
#include "SoundManager.h"
#include "../Application.h"

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}
	instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		SoundManager::CreateInstance();
	}
	return *instance_;
}

void SoundManager::Destroy(void)
{
	// 読み込んだサウンドハンドルをすべてメモリから解放
	for (auto& pair : instance_->seMap_)
	{
		DeleteSoundMem(pair.second);
	}
	instance_->seMap_.clear();

	delete instance_;
}

SoundManager::SoundManager(void)
	: seMap_(),
	bgmMap_(),
	nowBgm_(BGM_ID::NONE)
{
}

void SoundManager::Init(void)
{
	// 音素材のルートパス（Application::PATH_DATA などと組み合わせる）
	std::string basePath = Application::PATH_DATA + "Sound/SE/"; // 環境に合わせてフォルダ名を調整してください

	// ここで使う音を一気に登録・ロードする
	LoadSE(SE_ID::CURSOR_MOVE, basePath + "Cursor.wav");
	LoadSE(SE_ID::DECIDE, basePath + "Decide.wav");
	LoadSE(SE_ID::SWORD_SWING, basePath + "Attack.mp3");
	LoadSE(SE_ID::HIT, basePath + "Hit.mp3");
	LoadSE(SE_ID::CLEAR, basePath + "GameCleal.wav");


	SetVolume(SE_ID::CURSOR_MOVE, 120);
	SetVolume(SE_ID::DECIDE, 180);
	SetVolume(SE_ID::SWORD_SWING, 100);
	SetVolume(SE_ID::HIT, 200);
	SetVolume(SE_ID::CLEAR, 100);


	std::string bgmPath = Application::PATH_DATA + "Sound/BGM/";
	LoadBGM(BGM_ID::TITLE, bgmPath + "Title.mp3");
	LoadBGM(BGM_ID::GAME, bgmPath + "MainGame.mp3");
	LoadBGM(BGM_ID::CLEAR, bgmPath + "a.mp3");
	SetVolumeBGM(BGM_ID::TITLE, 100);
	SetVolumeBGM(BGM_ID::GAME, 100);
}

void SoundManager::LoadSE(SE_ID id, const std::string& filePath)
{
	// 二重読み込み防止
	if (seMap_.find(id) != seMap_.end()) return;

	int handle = LoadSoundMem(filePath.c_str());
	if (handle != -1)
	{
		seMap_[id] = handle;
	}
}

void SoundManager::PlaySE(SE_ID id)
{
	auto it = seMap_.find(id);
	if (it != seMap_.end())
	{
		// DX_PLAYTYPE_BACK: 裏で1回だけ再生（音が重なっても大丈夫なモード）
		// 第3引数(TRUE): すでに同じ音が鳴っていても、最初から重ねて（または上書きして）再生する
		PlaySoundMem(it->second, DX_PLAYTYPE_BACK, TRUE);
	}
}

void SoundManager::SetVolume(SE_ID id, int volume)
{
	auto it = seMap_.find(id);
	if (it != seMap_.end())
	{
		// 音量が 0未満、または 255を超えないように安全対策
		if (volume < 0) volume = 0;
		if (volume > 255) volume = 255;

		// DxLibの関数で、指定した音ハンドルの音量を変更する
		ChangeVolumeSoundMem(volume, it->second);
	}
}

void SoundManager::LoadBGM(BGM_ID id, const std::string& filePath)
{
	// 二重読み込み防止
	if (bgmMap_.find(id) != bgmMap_.end()) return;

	int handle = LoadSoundMem(filePath.c_str());

	if (handle != -1)
	{
		bgmMap_[id] = handle;
	}
}

void SoundManager::PlayBGM(BGM_ID id)
{
	// 同じ曲が既に鳴っているなら何もしない(毎フレーム呼ばれても安全に)
	if (nowBgm_ == id) return;

	// 別の曲が鳴っていたら止める
	StopBGM();

	auto it = bgmMap_.find(id);
	if (it != bgmMap_.end())
	{
		// DX_PLAYTYPE_LOOP: ループ再生
		PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, TRUE);
		nowBgm_ = id;
	}
}

void SoundManager::StopBGM(void)
{
	if (nowBgm_ == BGM_ID::NONE) return;

	auto it = bgmMap_.find(nowBgm_);
	if (it != bgmMap_.end())
	{
		StopSoundMem(it->second);
	}
	nowBgm_ = BGM_ID::NONE;
}

void SoundManager::SetVolumeBGM(BGM_ID id, int volume)
{
	auto it = bgmMap_.find(id);
	if (it != bgmMap_.end())
	{
		if (volume < 0) volume = 0;
		if (volume > 255) volume = 255;

		ChangeVolumeSoundMem(volume, it->second);
	}
}
