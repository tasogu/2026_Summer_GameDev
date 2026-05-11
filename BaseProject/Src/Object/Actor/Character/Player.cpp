#include <DxLib.h>
#include "../../../Manager/ResourceManager.h"
#include "../../Common/Transform.h"
#include "Player.h"

Player::Player(void)
	:
	imgPlayer_(-1),
	CharactorBase()
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	//リソースロード
	InitLoad();

	//	// 大きさ、回転、座標の初期化
	InitTransform();

	// 衝突判定の初期化
	InitCollider();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();

}

void Player::Update(void)
{
	//移動
	Move();



}

void Player::Draw(void)
{
	//プレイヤーの描画
	MV1DrawModel(imgPlayer_);

}

void Player::Release(void)
{
	//プレイヤーの開放
	MV1DeleteModel(imgPlayer_);
}

void Player::InitLoad(void)
{
	//プレイヤーモデルのロード
//	imgPlayer_ = resMng_.Load(ResourceManager::SRC::PLAYER).handleId_;
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::PLAYER).handleId_);

}

void Player::InitTransform(void)
{

	//プレイヤーの大きさ
	//MV1SetScale(imgPlayer_, SCALE_);
	transform_.scl = SCALE_;

	//プレイヤーの回転
	//MV1SetRotationXYZ(imgPlayer_, VGet(0.0f, ROT_Y, 0.0f));
	//transform_.quaRotLocal = VGet(0.0f, ROT_Y, 0.0f);

	//プレイヤーの座標
	MV1SetPosition(imgPlayer_, POS_);
}

void Player::InitCollider(void)
{
}

void Player::InitAnimation(void)
{
}

void Player::InitPost(void)
{
}

void Player::Move(void)
{

}
