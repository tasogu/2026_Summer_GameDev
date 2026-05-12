#include <DxLib.h>
#include "../../../Manager/ResourceManager.h"
#include "../../Common/Transform.h"
#include "../../../Manager/InputManager.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/Camera.h"
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
	ProcessMove();



}

void Player::Draw(void)
{
	//プレイヤーの描画
	MV1DrawModel(transform_.modelId);
	

}

void Player::Release(void)
{
	transform_.Release();
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
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(ROT_Y),0.0f });

	//プレイヤーの座標
	transform_.pos = POS_;
//	MV1SetPosition(imgPlayer_, POS_);
	transform_.Update();

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

void Player::ProcessMove(void)
{

	auto& ins = InputManager::GetInstance();

	//移動量をリセット
	movePow_ = AsoUtility::VECTOR_ZERO;

	//X回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotY();

	//回転したい角度
	double rotRad = 0;

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	//カメラ方向に前進したい
	if (ins.IsNew(KEY_INPUT_W))
	{
		rotRad = AsoUtility::Deg2RadD(0.0f);
		dir = cameraRot.GetForward();
	}

	//カメラ方向に後退したい
	if (ins.IsNew(KEY_INPUT_S))
	{
		rotRad = AsoUtility:: Deg2RadD(180.0f);
		dir = cameraRot.GetBack();
	}

	//カメラ方向に右移動したい
	if (ins.IsNew(KEY_INPUT_D))
	{
		rotRad = AsoUtility::Deg2RadF(90.0f);
		dir = cameraRot.GetRight();

	}

	//カメラ方向に左移動したい
	if(ins.IsNew(KEY_INPUT_A))
	{
		rotRad = AsoUtility::Deg2RadF(270.0f);
		dir = cameraRot.GetLeft();
	}

}
