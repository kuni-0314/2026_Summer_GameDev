#pragma once
#include "../EffectBase.h"
#include <DxLib.h>
#include <string>

class EffekseerEffect : public EffectBase
{
private:
    int m_effectHandle;  // エフェクトのリソースハンドル
    int m_playingHandle; // 再生中のインスタンスハンドル
    VECTOR m_pos3D;      // 3D空間の位置（DxLibのVECTOR型）

public:
    // コンストラクタ：ファイルパスと発生座標を受け取る
    EffekseerEffect(const std::wstring& filePath, const VECTOR& pos);
    virtual ~EffekseerEffect();

    virtual void Update() override;
    virtual void Draw() const override;

    void SetPosition(const VECTOR& pos);

};