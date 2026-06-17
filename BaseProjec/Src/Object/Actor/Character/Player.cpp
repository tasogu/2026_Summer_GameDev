#include <DxLib.h>
#include "../../../Manager/ResourceManager.h"
#include "../../Common/Transform.h"
#include "../../../Manager/InputManager.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Application.h"
#include "../../Common/AnimationController.h"
#include "../../../Manager/ColliderManager.h"
#include "..//ColliderCapsule.h"
#include "..//ColliderLine.h"
#include "Enemy/EnemyBase.h"
#include "Sword.h"
#include "Player.h"

Player::Player(void)
	:
	sword_(nullptr),
	imgPlayer_(-1),
	CharactorBase()
{	
	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	state_ = STATE::NONE;
	animationController_ = nullptr;

	isDead_ = false;

	isAttack_ = false;

	imgSword_ = -1;

}

Player::~Player(void)
{

	//delete sword_;
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


void Player::UpdateProcess(void)
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

}

void Player::UpdateProcessPost(void)
{
	//更新
	transform_.Update();

	//位置を取得
	MATRIX handMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, handBoneid_);

	//剣の位置を手の位置に更新
	sword_->UpdatePose(handMatrix);

}



void Player::UpdateNone(void)
{
}

void Player::UpdatePlay(void)
{
	//アニメーションの更新
	animationController_->Update();

	//攻撃
	ProcessAttack();
	
	//攻撃中ではないなら移動
	if (isAttack_ == false) {
		//移動
		ProcessMove();

	}

	//移動方向に応じた回転
	Rotate();

	//プレイヤーの回転の更新
	transform_.quaRot = playerRotY_;


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
	//基底クラスの描画処理
	ActorBase::Draw();
	
	//剣の描画
	sword_->Draw();
}

void Player::Release(void)
{
	transform_.Release();
	
	ActorBase::Release();
}

void Player::InitLoad(void)
{
	//プレイヤーモデルのロード
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::PLAYER)->handleId_);

	//剣のモデルのロード
	//sword_ = new Sword();
	sword_ = std::make_unique<Sword>();
	sword_->Init();
	//剣の攻撃対象と攻撃力をセット
	sword_->SetWeaponProperty(ColliderBase::TAG::ENEMY, PLAYER_POW);

	//モデルの手のボーンを取得
	handBoneid_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHand");
	
	//剣のモデルを手のボーンに装着
	(imgSword_, handBoneid_, 1);
}

void Player::InitTransform(void)
{

	//プレイヤーの大きさ
	transform_.scl = SCALE;

	//プレイヤーの回転
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(ROT_Y),0.0f });

	//移動位置を初期化
	movePos_ = POS;

	//プレイヤーの座標
	transform_.pos = movePos_;

}

void Player::InitCollider(void)
{
	//主に地面との衝突で使用する線分コライダー	
	auto coiLine = std::make_unique<ColliderLine>(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	
	//線分コライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(coiLine.get());

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), std::move(coiLine));


	// 主に壁や木などの衝突で仕様するカプセルコライダ
	auto colCapsule = std::make_unique<ColliderCapsule>(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);

	//カプセルコライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(colCapsule.get());

	colCapsule->SetOwner(this);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), std::move(colCapsule));

}

void Player::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	//animationController_ = new AnimationController(transform_.modelId);
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE , 20.0f, path + "Idle.mv1");
	animationController_->Add((int)ANIM_TYPE::WALK, 20.0f, path + "Walk.mv1");
	animationController_->Add((int)ANIM_TYPE::RUN, 10.0f, path + "Run.mv1");
	animationController_->Add((int)ANIM_TYPE::ATTACK, 20.0f, path + "Attack.mv1");

	animationController_->Play((int)ANIM_TYPE::IDLE);

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
		dir = VAdd(dir,cameraRot.GetForward());
	}

	//カメラ方向に後退したい
	if (ins.IsNew(KEY_INPUT_S))
	{
		rotRad = AsoUtility:: Deg2RadD(180.0f);
		dir = VAdd(dir,cameraRot.GetBack());
	}

	//カメラ方向に右移動したい
	if (ins.IsNew(KEY_INPUT_D))
	{
		rotRad = AsoUtility::Deg2RadF(90.0f);
		dir = VAdd(dir,cameraRot.GetRight());

	}

	//カメラ方向に左移動したい
	if(ins.IsNew(KEY_INPUT_A))
	{
		rotRad = AsoUtility::Deg2RadF(270.0f);
		dir = VAdd(dir,cameraRot.GetLeft());
	}

	//移動角度がゼロ以外
	if (!AsoUtility::EqualsVZero(dir))
	{
		//移動速度を歩きに設定
		speed_ = SPEED_MOVE;

		//シフトキーを押されたら走る
		if (ins.IsNew(KEY_INPUT_RSHIFT) || ins.IsNew(KEY_INPUT_LSHIFT))
		{
			speed_ = SPEED_RUN;

			//アニメーションを走るに変更
			animationController_->Play((int)ANIM_TYPE::RUN);

		}
		else {
			//アニメーションを歩きに変更
			animationController_->Play((int)ANIM_TYPE::WALK);
		}

		//移動させたい方向(ベクトル)に変換
		moveDir_ = dir;

		//移動させたい方向に移動量をかける(=移動量)
		movePow_ = VScale(moveDir_, speed_);

		//回転処理
		SetGoalRotate(rotRad);


	}
	else if(AsoUtility::EqualsVZero(dir))
	{
		//アニメーションを待機に変更
		animationController_->Play((int)ANIM_TYPE::IDLE);
	}


}

void Player::ProcessAttack(void)
{
	auto& ins = InputManager::GetInstance();
	
	//攻撃ボタンが押されたら
	if (ins.IsNew(KEY_INPUT_K) && isAttack_ == false) {
		//攻撃中フラグを立てる
		isAttack_ = true;

		//移動量をリセット
		movePow_ = AsoUtility::VECTOR_ZERO;

		//アニメーションを攻撃に変更
		animationController_->Play((int)ANIM_TYPE::ATTACK, false);


	}
	else if (isAttack_ == true)
	{
		//アニメーションの再生時間を取得
		float nowTime = animationController_->GetTime();

		//アニメーション途中から当たり判定を開始
		if (nowTime >= 15.0f && nowTime <= 30.0f) {
			sword_->ExecuteStrike();
		}

		//攻撃アニメーションが終了したら
		if (animationController_->IsEnd()) {
			//攻撃中フラグをリセット
			isAttack_ = false;
			sword_->ResetStrike();

			//アニメーションを待機に変更
			animationController_->Play((int)ANIM_TYPE::IDLE);

		}

	}

}

void Player::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, AsoUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;

}

void Player::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	//回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

