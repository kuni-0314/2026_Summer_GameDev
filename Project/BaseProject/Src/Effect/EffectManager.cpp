#include "EffectManager.h"
#include <algorithm>

// 全エフェクトの更新と不要になったエフェクトの自動削除
void EffectManager::Update() 
{
    // 1. 各エフェクトの更新処理
    for (auto& effect : m_effects)
    {
        effect->Update();
    }

    // 2. 寿命が尽きた（IsDead() == true）エフェクトをリストから削除・メモリ解放
    // std::erase_if が使えるC++20以降ならもっとスッキリ書けますが、汎用的な remove_if で記述しています
    m_effects.erase(
        std::remove_if(m_effects.begin(), m_effects.end(),
            [](const std::shared_ptr<EffectBase>& effect) 
            {
                return effect->IsDead(); // true ならリストから除外
            }),
        m_effects.end()
    );
}

// 全エフェクトの描画
void EffectManager::Draw() const 
{
    for (const auto& effect : m_effects) 
    {
        effect->Draw();
    }
}

// 全削除
void EffectManager::Clear() 
{
    m_effects.clear(); // shared_ptr なので、これだけでメモリも自動解放されます
}

// 外部から新しいエフェクトを追加
void EffectManager::RegisterEffect(const std::shared_ptr<EffectBase>& effect)
{
    if (effect != nullptr) 
    {
        m_effects.push_back(effect);
    }
}