#include <DxLib.h>
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Collider/Sphere/ColliderSphere.h"
#include "../../../Collider/Line/ColliderLine.h"
#include "../../../Collider/Model/ColliderModel.h"
#include "../../../Common/AnimationController.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Object/Item/HP/HpItem.h"
#include "../../../../Object/Item/ItemManger.h"
#include "../../../../Sound/AudioManager.h"
#include "./../../../../Application.h"
#include "../../Player/Player.h"
#include "Dragon.h"

Dragon::Dragon(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:EnemyBase(data, attackModel, player),
	state_(STATE::NONE),
	step_(0.0f)
{
}

Dragon::~Dragon()
{
}

void Dragon::Draw(void)
{
}

void Dragon::Release(void)
{
}

void Dragon::InitLoad()
{
	//規定クラスのロード
	CharactorBase::InitLoad();
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_DRAGON));

}

void Dragon::InitTransform()
{
}

void Dragon::InitCollider()
{
}

void Dragon::InitAnimation()
{
}

void Dragon::InitPost()
{
}

void Dragon::UpdateProcess()
{
}

void Dragon::UpdateProcessPost()
{
}
