#pragma once
#include "../Common/Vector2.h" // 状況に応じてVector3やQuaternionに変更してください

class EffectBase {
protected:
    Vector2 m_position;  // 発生位置
    bool m_isDead;       // 消滅フラグ

public:
    EffectBase(const Vector2& pos) : m_position(pos), m_isDead(false) {}
    virtual ~EffectBase() = default;

    // 毎フレームの更新（アニメーション進行など）
    virtual void Update() = 0;

    // 描画処理
    virtual void Draw() const = 0;

    // 消滅しているかどうかの確認
    bool IsDead() const { return m_isDead; }
};