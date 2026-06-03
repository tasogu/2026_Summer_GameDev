#include "../Object/Actor/ColliderBase.h"
#include "ColliderManager.h"

void ColliderManager::Register(const ColliderBase* col)
{
	if (!col)
	{
		return;
	}
	collders_.push_back(col);
}

void ColliderManager::Unregister(ColliderBase* col)
{
	//リストから順番に消していく
	for (size_t i = 0; i < collders_.size(); ++i) {
		//もしリストのコライダーが消したいコライダーと同じなら
		if (collders_[i] == col) {
			//
			collders_.erase(collders_.begin() + i);
			return;
		}
	}
}

void ColliderManager::Update()
{
	for (size_t i = 0; i < collders_.size(); ++i) {
		//自分より後ろの人だけとチェックする
		for (size_t j = i + 1; j < collders_.size(); ++j) {

			//i番目とj番目のコライダーに
			if (collders_[i]->CheckCollision(collders_[j])) {
			}
		}
	}
}