#pragma once
#include <vector>
class ColliderBase;

class ColliderManager
{
public:
	// コライダーを登録する
	void Register(const ColliderBase* col);

	// 削除するときに登録を外す
	void Unregister(ColliderBase* col); 

	// 更新
	void Update();
private:
	std::vector<const ColliderBase*> collders_;
};

