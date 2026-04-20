#pragma once
#include <string>

class Application
{

	public:
		//スクリーンサイズ・色
		static constexpr int SCREEN_SIZE_X = 1024;
		static constexpr int SCREEN_SIZE_Y = 640;
		static constexpr int SCREEN_COLLAR = 32;

		//データパス関連
		static const std::string PATH_IMAGE;
	
		//明示的にインスタンスを生成する
		static void CreateInstance(void);

		//静的インスタンスの取得
		static Application& GetInstance(void);

		//初期化
		void Init(void);

		//ゲームループの開始
		void Run(void);

		//リソースの破棄
		void Destroy(void);

		//初期化成功/失敗の判定
		bool IsInitFail(void) const;

		//解放成功/失敗の判定
		bool IsReleaseFail(void) const;

	private:

		//静的インスタンス
		static Application* instance_;

		//初期化失敗
		bool isInitFail_;

		//解放失敗
		bool isReleaseFail_;

		//デフォルトコンストラクタをprivateにして外部から生成できないようにする
		Application(void);
		Application(const Application& manager) = default;
		~Application(void) = default;

};

