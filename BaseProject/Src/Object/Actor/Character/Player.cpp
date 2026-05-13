#include <DxLib.h>
#include "../../../Manager/ResourceManager.h"
#include "../../Common/Transform.h"
#include "../../../Manager/InputManager.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/Camera.h"
#include "../../Common/AnimationController.h"
#include "Player.h"

Player::Player(void)
	:
	imgPlayer_(-1),
	CharactorBase()
{
	state_ = STATE::NONE;

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
	//更新ステップ
	switch (state_)
	{
	case Player::STATE::NONE:
		UpdateNone();
		break;
	case Player::STATE::PLAY:
		UpdatePlay();
		break;
	}


	//プレイヤーの更新
	transform_.Update();
}

void Player::UpdateNone(void)
{
}

void Player::UpdatePlay(void)
{
	//移動
	ProcessMove();

}

void Player::ChangeState(STATE state)
{
	
	state_ = state;

	switch (state_)
	{
	case Player::STATE::NONE:
		ChangeStateNone();
		break;
	case Player::STATE::PLAY:
		ChangeStatePlay();
		break;
	}
}

void Player::ChangeStateNone(void)
{
}

void Player::ChangeStatePlay(void)
{
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
	transform_.scl = SCALE;

	//プレイヤーの回転
	//MV1SetRotationXYZ(imgPlayer_, VGet(0.0f, ROT_Y, 0.0f));
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(ROT_Y),0.0f });

	//移動位置を初期化
	movePos_ = POS;

	//プレイヤーの座標
	transform_.pos = movePos_;
//	MV1SetPosition(imgPlayer_, POS_);

}

void Player::InitCollider(void)
{
}

void Player::InitAnimation(void)
{
	
}

void Player::InitPost(void)
{
	//移動量を初期化
	movePow_ = AsoUtility::VECTOR_ZERO;

	//移動方向を初期化
	moveDir_ = AsoUtility::VECTOR_ZERO;

	//stateをプレイに変更
	ChangeState(Player::STATE::PLAY);

	//プレイヤーの更新
	transform_.Update();

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

	//移動角度がゼロより上
	if (!AsoUtility::EqualsVZero(dir))
	{
		//移動処理
		speed_ = SPEED_MOVE;
		if (ins.IsNew(KEY_INPUT_RSHIFT))
		{
			speed_ = SPEED_RUN;
		}
		//移動させたい方向(ベクトル)に変換
		moveDir_ = dir;

		//移動させたい方向に移動量をかける(=移動量)
		movePow_ = VScale(moveDir_, speed_);

		//プレイヤーの座標に移動量を加算
		movePos_ = VAdd(transform_.pos, movePow_);

		//プレイヤーに移動を適用
		transform_.pos = movePos_;
	}


}

