#include "../../../../Manager/ResourceManager.h"
#include "../../../../Application.h"
#include "../../../../Manager/ColliderManager.h"
#include "../../../../Manager/InputManager.h"
#include "../../../Common/AnimationController.h"
#include "../../../Common/Transform.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/SceneManager.h"
#include "../../ColliderCapsule.h"
#include "../../ColliderLine.h"
#include "../../HpBar.h"
#include "../Player.h"
#include "../Sword.h"
#include "NomalEnemy.h"

NomalEnemy::NomalEnemy(const EnemyBase::EnemyData& data)
	:
	EnemyBase(data)
{
	isAttack_ = false;
	imgSword_ = -1;
	state_ = STATE::PLAY;
	targetPlayer_ = nullptr;
	speed_ = ENEMY_RUN * scnMng_.GetDeltaTime();
	random_ = 0;
	enemyActive_ = ENEMY_ACTIVE::MOVE;
	coolTime_ = 0;
	//初期座標の設定
	transform_.pos = data.defaultPos;
	hp_ = data.hp;
}

NomalEnemy::~NomalEnemy(void)
{
	if (sword_)
	{
		sword_->ResetStrike();
	}
}

void NomalEnemy::Init(void)
{
	//リソースロード
	InitLoad();

	//大きさ、回転、座標の初期化
	InitTransform();

	// 衝突判定の初期化
	InitCollider();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();

}

void NomalEnemy::Update(Player* player)
{
	targetPlayer_ = player;
	
	CharactorBase::Update();
}

void NomalEnemy::Draw(void)
{
	sword_->Draw();

	hpBar_->Draw();

	CharactorBase::Draw();
}

void NomalEnemy::Release(void)
{
	sword_->Release();
}

void NomalEnemy::InitLoad(void)
{

	//複製のハンドルをもらってロード
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::NOMAL_ENEMY));
	
	//剣のモデルのロード
	sword_ = std::make_unique<Sword>();
	sword_->Init();

	//剣の攻撃対象と攻撃力をセット
	sword_->SetWeaponProperty(ColliderBase::TAG::PLAYER, ENEMY_POW);

	//モデルの手のボーンを取得
	handBoneid_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHand");

	//剣のモデルを手のボーンに装着
	(imgSword_, handBoneid_, 1);

	//HPバーのモデルのロード
	hpBar_ = std::make_unique<HpBar>();
	hpBar_->Init();
	hpBar_->SetProperty(hp_);
}

void NomalEnemy::InitTransform(void)
{
	//プレイヤーの大きさ
	transform_.scl = SCALE;

	//プレイヤーの回転
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(ROT_Y),0.0f });

	//プレイヤーの座標
	transform_.Update();

}

void NomalEnemy::InitCollider(void)
{
	//主に地面との衝突で使用する線分コライダー	
	auto coiLine = std::make_unique< ColliderLine>(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);

	//線分コライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(coiLine.get());

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), std::move(coiLine));

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	auto colCapsule = std::make_unique< ColliderCapsule>(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);

	//カプセルコライダーを当たり判定リストに登録
	ColliderManager::GetInstance().Register(colCapsule.get());

	colCapsule->SetOwner(this);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE),std::move( colCapsule));
}

void NomalEnemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	//animationController_ = new AnimationController(transform_.modelId);
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, 20.0f, path + "Idle.mv1");
	animationController_->Add((int)ANIM_TYPE::WALK, 20.0f, path + "Walk.mv1");
	animationController_->Add((int)ANIM_TYPE::RUN, 10.0f, path + "Run.mv1");
	animationController_->Add((int)ANIM_TYPE::ATTACK, 14.0f, path + "Attack.mv1");

	animationController_->Play((int)ANIM_TYPE::IDLE);

}

void NomalEnemy::InitPost(void)
{
	//移動量を初期化
	movePow_ = AsoUtility::VECTOR_ZERO;

	//移動方向を初期化
	moveDir_ = AsoUtility::VECTOR_ZERO;

	//stateをプレイに変更
	ChangeState(STATE::PLAY);

	//プレイヤーの更新
	transform_.Update();

}


void NomalEnemy::UpdateProcess(void)
{
	//更新ステップ
	switch (state_)
	{
	case STATE::NONE:
		UpdateNone();
		break;
	case STATE::PLAY:
		UpdatePlay();
		break;
	}

}

void NomalEnemy::UpdateProcessPost(void)
{
	//更新
	transform_.Update();

	//位置を取得
	MATRIX handMatrix = MV1GetFrameLocalWorldMatrix(transform_.modelId, handBoneid_);

	//剣の位置を手の位置に更新
	sword_->UpdatePose(handMatrix);

}

void NomalEnemy::UpdateNone(void)
{
}

void NomalEnemy::UpdatePlay(void)
{
	//アニメーションの更新
	animationController_->Update();

	//HPバーの更新
	VECTOR barPos = VAdd(transform_.pos, HP_BAR_OFFSET);
	//hpBar_->Update(transform_.pos, hp_);
	//hpBar_->Update(transform_.pos, hp_);
	hpBar_->Update(barPos, hp_);

	switch (enemyActive_)
	{
	case ENEMY_ACTIVE::MOVE:
		//移動
		ProcessMove();
		break;
	case ENEMY_ACTIVE::TURN:
		//向きなおす
		TurnMove();
		break;
	case ENEMY_ACTIVE::ATTACK:
		//攻撃
		ProcessAttack();
		break;
	case ENEMY_ACTIVE::COOLDOWN:
		//硬直
		Cooldown();
		break;
	}

	
	////攻撃中ではないなら移動
	//if (isAttack_ == false) {
	//	//移動
	//	ProcessMove();

	//}


}

void NomalEnemy::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case STATE::NONE:
		ChangeStateNone();
		break;
	case STATE::PLAY:
		ChangeStatePlay();
		break;
	}

}

void NomalEnemy::ChangeStateNone(void)
{
}

void NomalEnemy::ChangeStatePlay(void)
{
}

void NomalEnemy::ProcessAttack(void) 
{
	auto& ins = InputManager::GetInstance();

	VECTOR targetPos = targetPlayer_->GetTransform().pos;

	if (isAttack_ == true)
	{
		//アニメーションを攻撃に変更
		animationController_->Play((int)ANIM_TYPE::ATTACK, false);

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

			//硬直秒数をセット
			coolTime_ = COOL_TIME;

			//行動を移動に変更
			enemyActive_ = ENEMY_ACTIVE::COOLDOWN;
		}

	}

}

void NomalEnemy::ProcessMove(void)
{
	//記録しているターゲットの座標をゲット
	VECTOR targetPos = targetPlayer_->GetTransform().pos;
	VECTOR myPos = transform_.pos;

	VECTOR diff = VSub(targetPos, myPos);

	float distance = VSize(diff);

	//近づくまで移動
	//if(!IsWithinCirclingRange(targetPos, 200.0f))
	if (distance >= ENEMY_RANGE)
	{
		animationController_->Play((int)ANIM_TYPE::RUN);

		moveDir_ = VNorm(diff);
		movePow_ = VScale(moveDir_, speed_);

	}
	else {

		// 近づきすぎないように止まる
		movePow_ = AsoUtility::VECTOR_ZERO;

		//攻撃に交代
		enemyActive_ = ENEMY_ACTIVE::TURN;

		//攻撃を開始する
		isAttack_ = true;

	}

	//if (distance <= 300.0f && distance >= 200.0f) {
	//	isAttack_ = true;

	//	moveDir_ = VNorm(diff);
	//	
	//	// 近づきすぎないように止まる
	//	movePow_ = AsoUtility::VECTOR_ZERO;
	//}

}

void NomalEnemy::TurnMove(void)
{
	//記録しているターゲットの座標をゲット
	VECTOR targetPos = targetPlayer_->GetTransform().pos;
	VECTOR myPos = transform_.pos;

	VECTOR diff = VSub(targetPos, myPos);

	moveDir_ = VNorm(diff);


	enemyActive_ = ENEMY_ACTIVE::ATTACK;
}

void NomalEnemy::Cooldown(void)
{
	//硬直秒数を減らす
	coolTime_ -= scnMng_.GetDeltaTime();

	//硬直がゼロなら移動へ
	if (coolTime_ <= 0.0f)
	{
		enemyActive_ = ENEMY_ACTIVE::MOVE;
	}
}


