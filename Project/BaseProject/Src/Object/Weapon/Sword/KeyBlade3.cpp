#include "KeyBlade3.h"
#include "../../../Manager/ResourceManager.h"
#include "../../Common/Transform.h"
#include "../../Collider/ColliderBase.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Application.h"

KeyBlade3::KeyBlade3(Transform& transform)
	: SwordBase(TYPE::KEY_BLADE_3, transform)
{
	scl = 1.5f;
}

KeyBlade3::~KeyBlade3()
{
}

void KeyBlade3::InitLoad()
{
	// モデルのロード
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::KEY_BLADE_3).handleId_);
}

void KeyBlade3::InitTransform()
{
	// 武器の初期位置、回転、大きさを設定
	transform_.scl = { scl, scl, scl };
	transform_.rot = { 0.0f, 0.0f, 0.0f };
	//transform_.pos = VGet(0.0f, 0.0f, 0.0f);
}

void KeyBlade3::InitCollider()
{
}

void KeyBlade3::InitAnimation()
{
}

void KeyBlade3::InitPost()
{
	SetupNormalAttack1();
}

void KeyBlade3::Update()
{
	// Y軸周りに回転（Quaternionを使用）
	Quaternion rotY = Quaternion::AngleAxis(0.5f * DX_PI_F / 180.0f, AsoUtility::AXIS_Y);
	transform_.quaRot = transform_.quaRot.Mult(rotY);


	// 親クラスの更新処理
	SwordBase::Update();
}

void KeyBlade3::SetupNormalAttack1()
{
	// 通常攻撃1: 縦斬り
	HitboxInfo hitbox;
	hitbox.localPosStart = { 0.0f, 30.0f, 0.0f };
	hitbox.localPosEnd = { 0.0f, 120.0f, 0.0f };
	hitbox.radius = 20.0f;
	hitbox.activeStartFrame = 22;
	hitbox.activeEndFrame = 29;
	hitbox.damageMultiplier = 1.0f;
	hitbox.isActive = false;
	hitboxes_.push_back(hitbox);
}