#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "SceneBase.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitle_ = -1;
}

TitleScene::~TitleScene(void)
{
	
}

void TitleScene::Init(void)
{
	//‰æ‘œ“Ç‚İ‚İ
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
}

void TitleScene::Update(void)
{
}

void TitleScene::Draw(void)
{
}
