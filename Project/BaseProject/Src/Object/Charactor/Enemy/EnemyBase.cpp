#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../Charactor/Player/Player.h"
#include "../Enemy/Shot/ShotBase.h"
#include "../Enemy/Shot/ShotStraight.h"
#include "EnemyBase.h"


EnemyBase::EnemyBase(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:
CharactorBase(),
player_(player),
type_(data.type),
defaultPos_(data.defaultPos),
movableRange_(data.movableRange),
power_(data.power),
attackModle_(attackModel)
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

void EnemyBase::Release(void)
{
	CharactorBase::Release();
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


	auto it = stateChanges_.find(stateBase_);

	if (it != stateChanges_.end())
	{
		// 登録された初期処理を実行（EnemyRat::ChangeStateXXX が stateUpdate_ を設定する）
		it->second();
	}
	else
	{
		// マッピングがない場合は明示的に空にする（保険）
		stateUpdate_ = std::function<void(void)>{};
	}

}

