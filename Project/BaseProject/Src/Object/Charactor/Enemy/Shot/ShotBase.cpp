#include "ShotBase.h"
#include "../../../Actor/ActorBase.h"
#include <DxLib.h>

ShotBase::ShotBase(TYPE type, int baseModelId)
    :ActorBase()
{
    modelId_ = MV1DuplicateModel(baseModelId);
    type_ = type;
}

ShotBase::~ShotBase(void)
{
}

void ShotBase::CreateShot(VECTOR pos, VECTOR dir)
{
    // 弾の発射位置を設定
    pos_ = pos;
    // 弾の発射方向の設定
    dir_ = dir;

    // パラメータ設定
    SetParam();

    DrawSphere3D(pos,15, 10, 0x0000ff, 0x0000ff, false);
}

void ShotBase::Update(void)
{
    if (!IsAlive())
    {
        // 生存していなければ処理中断
        return;
    }
    // 移動処理
    pos_ = VAdd(pos_, VScale(dir_, speed_));
    // 位置の設定
    MV1SetPosition(modelId_, pos_);
    // 生存カウンタの減少
    ReduceCntAlive();
}

void ShotBase::Draw(void)
{
    if (!IsAlive())
    {
        // 生存していなければ処理中断
        return;
    }
    // モデルの描画
    MV1DrawModel(modelId_);
}
void ShotBase::Release(void)
{
    MV1DeleteModel(modelId_);
}

bool ShotBase::IsAlive(void)
{
    return isAlive_;
}

VECTOR ShotBase::GetPos(void)
{
    return pos_;
}

ShotBase::TYPE ShotBase::GetType(void)
{
    return type_;
}

float ShotBase::GetCollisionRadius(void)
{
    return collisionRadius_;
}

void ShotBase::ReduceCntAlive(void)
{
    cntAlive_--;
    if (cntAlive_ < 0)
    {
        isAlive_ = false;
    }
}


