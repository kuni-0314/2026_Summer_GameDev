#pragma once
#include <vector>
#include <memory>
#include "EffectBase.h"

class EffectManager {
private:
    // 生存しているエフェクトを一元管理するリスト
    std::vector<std::shared_ptr<EffectBase>> m_effects;

    // シングルトン、もしくは静的メンバとしてどこからでも呼べるように設計すると便利です
    EffectManager() = default;
    ~EffectManager() = default;

public:
    // インスタンスの取得（シングルトンパターンの場合）
    static EffectManager& GetInstance() {
        static EffectManager instance;
        return instance;
    }

    // コピーと代入を禁止
    EffectManager(const EffectManager&) = delete;
    EffectManager& operator=(const EffectManager&) = delete;

    // エフェクトの更新（全エフェクトの更新 ＆ 寿命が尽きたものの削除）
    void Update();

    // エフェクトの描画（全エフェクトの描画）
    void Draw() const;

    // 全エフェクトの強制削除（ステージ切り替え時などに使用）
    void Clear();

    // エフェクトの登録（外部から呼び出す用）
    void RegisterEffect(const std::shared_ptr<EffectBase>& effect);
};