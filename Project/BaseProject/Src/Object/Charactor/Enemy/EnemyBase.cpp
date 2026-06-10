#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../Charactor/Player/Player.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const EnemyBase::EnemyData& data,Player*player)
	:
	CharactorBase(),
	player_(player),
	type_(data.type),
	defaultPos_(data.defaultPos),
	movableRange_(data.movableRange),
	power_(data.power)
{
	// 初期座標の設定
	transform_.pos = data.defaultPos;

	hp_ = data.hp;
}
EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Draw(void)
{
	CharactorBase::Draw();
#ifdef _DEBUG
	// 移動可能範囲のデバッグ描画
	//DrawSphere3D(defaultPos_, 500.0f, 16, 0x000099, 0x000099, false);
#endif // _DEBUG
}

bool EnemyBase::InMovableRange(void) const
{
	bool ret = false;
	// 初期位置からの距離
	float dis = static_cast<float>(
		AsoUtility::SqrMagnitude(defaultPos_, transform_.pos));
	// 指定距離判定
	if (dis < movableRange_ * movableRange_)
	{
		return true;
	}
	return ret;

}

void EnemyBase::LookPlayer(void)
{
	
	VECTOR playerPos = player_->GetPos();

	//ベクトル計算
	VECTOR diff = VSub(playerPos, transform_.pos);
	diff.y = 0.0f;

	//ベクトルの正規化で単位ベクトルの取得
	moveDir_ = VNorm(diff);

	transform_.rot.y = atan2(moveDir_.x, moveDir_.z);
	transform_.rot.y += AsoUtility::Deg2RadF(180.0f);

	//回転はY軸のみにする
	transform_.rot.x = transform_.rot.z = 0.0f;

	MV1SetRotationXYZ(transform_.modelId, transform_.rot);
}



void EnemyBase::ChangeState(int state)
{
	stateBase_ = state;
	// 各状態遷移の初期処理
	stateChanges_[stateBase_]();
}




