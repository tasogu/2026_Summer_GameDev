#pragma once

class EnemyManager
{
public:
	//コンストラクタ
	EnemyManager(void);;

	//デストラクタ
	~EnemyManager(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Release(void);

private:
	//更新系(純粋仮想関数)
	void UpdateProcess(void);
	void UpdateProcessPost(void);


};

