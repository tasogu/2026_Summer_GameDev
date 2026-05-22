#pragma once
#include "../CharactorBase.h"

class EnemyBase : public CharactorBase
{
public:
	//コンストラクタ
	EnemyBase(void);;

	//デストラクタ
	virtual ~EnemyBase(void) override;

	//初期化
	void Init(void);

	//更新
	virtual void Update(void) override;

	//描画
	virtual void Draw(void) override;

	//解放
	virtual void Release(void) override;

private:
	
	//更新系(純粋仮想関数)
	virtual void UpdateProcess(void) = 0;
	virtual void UpdateProcessPost(void) = 0;

};

