#include "KeyBlade1.h"
#include "../../../Manager/ResourceManager.h"
#include "../../Common/Transform.h"
#include "../../Collider/ColliderBase.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Application.h"

KeyBlade1::KeyBlade1(VECTOR startPos, VECTOR endPos, float radius, const Transform& ownerTransform)
	: SwordBase(SWORD_TYPE::KEY_BLADE_1, startPos, endPos, radius, ownerTransform)
{
	scl = 0.0385f;
}

KeyBlade1::~KeyBlade1()
{
}

void KeyBlade1::InitLoad()
{
	// モデルのロード
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::KEY_BLADE_1).handleId_);
}

void KeyBlade1::InitTransform()
{
	// 武器の初期位置、回転、大きさを設定
	transform_.scl = { scl, scl, scl };
	transform_.rot = { 0.0f, 0.0f, 0.0f };
	transform_.pos = { 0.0f, 0.0f, 0.0f};
	
	transform_.quaRotLocal = Quaternion::Identity();
}

void KeyBlade1::InitAnimation()
{
}

void KeyBlade1::InitPost()
{
}

void KeyBlade1::Update()
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

	// Transform の更新
	transform_.Update();
}