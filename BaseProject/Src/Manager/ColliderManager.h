#pragma once
#include <vector>
class ColliderBase;

class ColliderManager
{
public:
	ColliderManager(const ColliderManager&) = delete;
	ColliderManager& operator=(const ColliderManager&) = delete;

	static ColliderManager& GetInstance()
	{
		static ColliderManager instance;
		return instance;
	}

	// コライダーを登録する
	void Register(const ColliderBase* col);

	// 削除するときに登録を外す
	void Unregister(const ColliderBase* col); 

	// 更新
	void Update();

	//全コライダーを渡す
	const std::vector<const ColliderBase*>& GetColliders(void);
private:
	ColliderManager() = default;
	~ColliderManager() = default;

	std::vector<const ColliderBase*> collders_;
};

