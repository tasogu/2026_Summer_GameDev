#pragma once
#include <chrono>

class SceneBase;
class Camera;

class SceneManager
{
public:

	//シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};

	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	//リソースの破損
	void Destroy(void);

	//状態以降
	void ChangeScen(SCENE_ID nextId);

	//シーンID取得
	SCENE_ID GetSceneID(void);

	//デルタタイムの取得
	float GetDeltaTime(void) const;

	//カメラの取得
	Camera* GetCamera(void) const;

private:

	//静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	//各種シーン
	SceneBase* scene_;

	//カメラ
	Camera* camera_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	//デフォルトコンストラクタをprivateにして
	//外部から生成できないようにする
	SceneManager(void);

	//コピーコンストラクタも同様
	SceneManager(const SceneManager& manager) = default;

	//デストラクタも同様
	~SceneManager(void) = default;

	//デルタタイムをリセットする
	void ResetDeltaTime(void);

	//シーン移行
	void DoChangeScene(SCENE_ID sceneId);

};

