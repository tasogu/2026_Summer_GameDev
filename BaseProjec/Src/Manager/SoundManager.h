#pragma once
#include <unordered_map>
#include <string>

class SoundManager
{
public:
	// 効果音のID（鳴らしたい音が増えたらここに追加していく）
	enum class SE_ID
	{
		CURSOR_MOVE, // カーソル移動音
		DECIDE,      // 決定音
		SWORD_SWING, // 剣を振る音
		HIT,		 //ヒット音
		CLEAR
	};

	enum class BGM_ID
	{
		NONE,        // 無音状態を表す
		TITLE,       // タイトル画面
		GAME,        // ゲーム中
		CLEAR
	};


	// インスタンスの明示的生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SoundManager& GetInstance(void);

	// リソースの破棄
	static void Destroy(void);

	// 初期化（ここで音ファイルを一括ロードする）
	void Init(void);

	// SEを再生する関数
	void PlaySE(SE_ID id);

	// SEの音量を変更する関数 (volume は 0 ～ 255)
	void SetVolume(SE_ID id, int volume);

	void LoadBGM(BGM_ID id, const std::string& filePath);
	// BGMを再生する(ループ。既に同じ曲ならなにもしない)
	void PlayBGM(BGM_ID id);
	// BGMを停止する
	void StopBGM(void);
	// BGMの音量変更
	void SetVolumeBGM(BGM_ID id, int volume);

private:
	// 静的インスタンス
	static SoundManager* instance_;

	// IDとDxLibのサウンドハンドルを紐づけるマップ
	std::unordered_map<SE_ID, int> seMap_;

	// 個別のファイル読み込み用サブ関数
	void LoadSE(SE_ID id, const std::string& filePath);

	// シングルトン用のコンストラクタ制限
	SoundManager(void);
	~SoundManager(void) = default;
	SoundManager(const SoundManager&) = default;

	// BGM用のマップ
	std::unordered_map<BGM_ID, int> bgmMap_;
	// 現在再生中のBGM
	BGM_ID nowBgm_;
};