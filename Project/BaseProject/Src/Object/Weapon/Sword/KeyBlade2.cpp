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
	// フレーム22のワールドマトリクスを取得
	MATRIX mat = MV1GetFrameLocalWorldMatrix(ownerTransform_.modelId, 22);

	// 位置補正（プレイヤーの向きに合わせて微調整）
	MATRIX offset = MMult(MGetTranslate(VGet(0.0f, 0.0f, -3.0f)), mat);

	// 位置を適用
	transform_.pos = VGet(offset.m[3][0], offset.m[3][1], offset.m[3][2]);

	// 回転をQuaternionに変換
	Quaternion rot = Quaternion::GetRotation(mat);

	// 回転補正
	rot = rot.Mult(Quaternion::Euler(0.0f * DX_PI_F / 180.0f, 8.0f * DX_PI_F / 180.0f, -60.0f * DX_PI_F / 180.0f));
	transform_.quaRot = rot;

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