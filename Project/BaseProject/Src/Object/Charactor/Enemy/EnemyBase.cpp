#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const EnemyBase::EnemyData& data,Player*player)
	:
	CharactorBase(),
	player_(player),
	type_(data.type),
	hp_(data.hp),
	defaultPos_(data.defaultPos),
	movableRange_(data.movableRange),
	power_(data.power)
{
	// 初期座標の設定
	transform_.pos = data.defaultPos;
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
void EnemyBase::Damege(int damege)
{
	hp_ -= damege;
	if (hp_ <= 0)
	{
		hp_ = 0;
		// 死亡処理
	}
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



void EnemyBase::ChangeState(int state)
{
	stateBase_ = state;
	// 各状態遷移の初期処理
	stateChanges_[stateBase_]();
}




