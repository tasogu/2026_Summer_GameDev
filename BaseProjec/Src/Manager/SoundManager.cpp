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
	: seMap_()
{
}

void SoundManager::Init(void)
{
	// 音素材のルートパス（Application::PATH_DATA などと組み合わせる）
	std::string basePath = Application::PATH_DATA + "Sound/SE/"; // 環境に合わせてフォルダ名を調整してください

	// ここで使う音を一気に登録・ロードする
	LoadSE(SE_ID::CURSOR_MOVE, basePath + "Cursor.wav");
	LoadSE(SE_ID::DECIDE, basePath + "Decide.wav");
	LoadSE(SE_ID::SWORD_SWING, basePath + "Swing.mp3");
	LoadSE(SE_ID::HIT, basePath + "Hit.mp3");

	SetVolume(SE_ID::CURSOR_MOVE, 120);
	SetVolume(SE_ID::DECIDE, 180);
	SetVolume(SE_ID::SWORD_SWING, 50);
	SetVolume(SE_ID::HIT, 80);
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
