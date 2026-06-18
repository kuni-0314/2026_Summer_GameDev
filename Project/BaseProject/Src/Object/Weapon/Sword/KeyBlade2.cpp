#include "KeyBlade2.h"
#include "../../../Manager/ResourceManager.h"
#include "../../Common/Transform.h"
#include "../../Collider/ColliderBase.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Application.h"

KeyBlade2::KeyBlade2(Transform& transform)
	: SwordBase(TYPE::KEY_BLADE_2, transform)
{
	scl = 0.0385f;
}

KeyBlade2::~KeyBlade2()
{
}

void KeyBlade2::InitLoad()
{
	// モデルのロード
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::KEY_BLADE_2).handleId_);
}

void KeyBlade2::InitTransform()
{
	// 武器の初期位置、回転、大きさを設定
	transform_.scl = { scl, scl, scl };
	transform_.rot = { 0.0f, 0.0f, 0.0f };
	//transform_.pos = VGet(0.0f, 0.0f, 0.0f);
}

void KeyBlade2::InitCollider()
{
}

void KeyBlade2::InitAnimation()
{
}

void KeyBlade2::InitPost()
{
	SetupNormalAttack1();
}

void KeyBlade2::Update()
{
	// Y軸周りに回転（Quaternionを使用）
	Quaternion rotY = Quaternion::AngleAxis(0.5f * DX_PI_F / 180.0f, AsoUtility::AXIS_Y);
	transform_.quaRot = transform_.quaRot.Mult(rotY);


	// 親クラスの更新処理
	SwordBase::Update();
}

void KeyBlade2::SetupNormalAttack1()
{
	// 通常攻撃1: 縦斬り
	HitboxInfo hitbox;
	hitbox.localPosStart = { 0.0f, 15.0f, 0.0f };
	hitbox.localPosEnd = { 0.0f, 70.0f, 0.0f };
	hitbox.radius = 20.0f;
	hitbox.activeStartFrame = 22;
	hitbox.activeEndFrame = 29;
	hitbox.damageMultiplier = 1.0f;
	hitbox.isActive = false;
	hitboxes_.push_back(hitbox);
}