#include "../../Charactor/Enemy/EnemyBase.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../Collider/Line/ColliderLine.h"
#include "../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Charactor/Player/Player.h"
#include "HpItem.h"


HpItem::HpItem(Player* player)
	:player_(player)
{
}

HpItem::~HpItem(void)
{
}

void HpItem::Update(void)
{
	ItemBase::Update();
	//プレイヤーとの判定
	//InSearchModel()


	
}

void HpItem::Draw(void)
{


	ItemBase::Draw();
	//DrawSphere3D(transform_.pos,COL_SPHERE_RADIUS, 10, 0x0000ff, 0x0000ff, false);


}

void HpItem::InitLoad(void)
{
	//基底クラスのリソースロード
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ITEM_HP));

}

void HpItem::InitTransform(void)
{
	//大きさ、座標等の初期化
	transform_.scl = { SCALE ,SCALE ,SCALE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	jumpPow_ = AsoUtility::VECTOR_ZERO;
	transform_.Update();
}

void HpItem::InitCollider(void)
{
	// 主に地面との衝突で仕様する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::ITEM, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ITEM, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

	// モデルのコライダ
	ColliderModel* colModel =
		new ColliderModel(ColliderBase::TAG::ITEM, &transform_);
	//判定の登録
	ownColliders_.emplace(
		static_cast<int>(COLLIDER_TYPE::ITEM), colModel);
}

void HpItem::InitAnimation(void)
{
}

void HpItem::InitPost(void)
{
}

void HpItem::UpdateProcess(void)
{
    //プレイヤー座標取得
    VECTOR playerPos = player_->GetPos();

    // プレイヤー方向
    VECTOR toPlayer = VSub(playerPos, transform_.pos);

    // 距離
    float distance = VSize(toPlayer);

    // 正規化方向
    VECTOR dir = VNorm(toPlayer);

    const float SEARCH_RADIUS = 300.0f;
    const float MOVE_SPEED = 8.0f;

    // 取得前
    if (!isGet_)
    {
        // プレイヤーへ吸い寄せ
        if (distance < SEARCH_RADIUS)
        {
            transform_.pos = VAdd(
                transform_.pos,
                VScale(dir, MOVE_SPEED));
        }

     
        float playerRad = player_->GetcollRadius_();

        // 接触
        if (AsoUtility::IsHitSpheres(playerPos,playerRad,transform_.pos,COL_SPHERE_RADIUS))
        {
            isGet_ = true;
        }
    }
    else
    {
        // プレイヤーへめり込むように移動
        transform_.pos = VAdd(
            transform_.pos,
            VScale(dir, MOVE_SPEED * 1.5f));

        // 徐々に縮小
        transform_.scl.x *= SCL_REDUCTION_SPEED;
        transform_.scl.y *= SCL_REDUCTION_SPEED;
        transform_.scl.z *= SCL_REDUCTION_SPEED;

        // 小さくなったら消す
        if (transform_.scl.x < SCL_REDUCTION_MIN)
        {
            player_->HeleHp(HELE_HP);
            isAlive_ = false;
        }
    }

    transform_.Update();
}

void HpItem::UpdateProcessPost(void)
{
}
