#pragma once
#include "SceneBase.h"

class ClearScene : public SceneBase
{
public:
    ClearScene(void);
    ~ClearScene(void) override;

    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;

private:
    // 必要であれば画像やフォントのハンドルを追加
    int imgClear_;
};