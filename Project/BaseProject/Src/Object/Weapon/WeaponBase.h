#pragma once
#include "../Actor/ActorBase.h"
#include <vector>
#include <map>

class AnimationController;

class WeaponBase : public ActorBase
{
public:
	// 武器の種類
	enum class TYPE
	{
		NONE,
		SWORD,      // 剣
		AXE,        // 斧
		SPEAR,      // 槍
		HAMMER,     // ハンマー
		BOW,        // 弓
		MAX
	};

	// 攻撃判定情報
	struct HitboxInfo
	{
		VECTOR localPosStart;   // 判定開始位置（武器ローカル座標）
		VECTOR localPosEnd;     // 判定終了位置（武器ローカル座標）
		float radius;           // 判定半径
		int activeStartFrame;   // 判定開始フレーム
		int activeEndFrame;     // 判定終了フレーム
		float damageMultiplier; // ダメージ倍率
		bool isActive;          // 現在有効かどうか
	};

	// コンストラクタ
	WeaponBase(TYPE type, Transform* ownerTransform);
	// デストラクタ
	virtual ~WeaponBase(void) override;

	// 更新（ActorBase の純粋仮想関数をオーバーライド）
	void Update(void) override;

	// 描画（ActorBase の仮想関数をオーバーライド）
	void Draw(void) override;

	// 攻撃開始（攻撃タイプごとに異なる判定を有効化）
	virtual void BeginAttack(int attackType);

	// 攻撃終了
	virtual void EndAttack(void);

	// 現在のアクティブな攻撃判定を取得
	const std::vector<HitboxInfo>& GetActiveHitboxes(void) const;

	// 武器の先端座標を取得（ボーンから計算）
	VECTOR GetWeaponTipPos(void) const;

	// 武器の根元座標を取得（ボーンから計算）
	VECTOR GetWeaponBasePos(void) const;

	// ゲッター
	TYPE GetType(void) const { return type_; }
	int GetBaseDamage(void) const { return baseDamage_; }
	float GetAttackRange(void) const { return attackRange_; }
	bool IsAttacking(void) const { return isAttacking_; }
	int GetCurrentAttackType(void) const { return currentAttackType_; }
	int GetCurrentFrame(void) const { return currentFrame_; }

	// セッター
	void SetBaseDamage(int damage) { baseDamage_ = damage; }
	void SetAttackRange(float range) { attackRange_ = range; }

protected:
	// ActorBase の純粋仮想関数の実装
	void InitLoad(void) override;
	void InitTransform(void) override;
	void InitCollider(void) override;
	void InitAnimation(void) override;
	void InitPost(void) override;

	// 武器固有の初期化（純粋仮想関数 - 継承先で実装）
	virtual void InitModel(void) = 0;
	virtual void InitHitboxes(void) = 0;
	virtual void InitParameters(void) = 0;

	// 武器の種類
	TYPE type_;

	// 所有者のTransform（プレイヤーなど）
	Transform* ownerTransform_;

	// 基本ダメージ
	int baseDamage_;

	// 攻撃範囲
	float attackRange_;

	// 攻撃中フラグ
	bool isAttacking_;

	// 現在の攻撃タイプ
	int currentAttackType_;

	// 現在の攻撃フレーム
	int currentFrame_;

	// 攻撃タイプごとの判定情報リスト
	std::map<int, std::vector<HitboxInfo>> hitboxInfoMap_;

	// 現在アクティブな判定情報
	std::vector<HitboxInfo> activeHitboxes_;

	// 武器のボーンフレームインデックス
	int weaponBaseFrameIndex_;   // 根元（手元）
	int weaponTipFrameIndex_;    // 先端

	// 攻撃判定の更新
	virtual void UpdateHitboxes(void);

	// 武器座標の更新（ボーンから取得）
	virtual void UpdateTransformWeapon(void);

	// デバッグ描画
	virtual void DrawDebug(void);
};

