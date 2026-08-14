#pragma once
#include "../EffectBase.h"
#include <DxLib.h>
#include <string>
#include "../../Common/Quaternion.h"

class EffekseerEffect : public EffectBase
{
private:
    int m_effectHandle;  // エフェクトのリソースハンドル
    int m_playingHandle; // 再生中のインスタンスハンドル
    VECTOR m_pos3D;      // 3D空間の位置（DxLibのVECTOR型）

    int m_lifeTime = -1;  // -1ならEffekseer任せ
    int m_playFrame = 0;

public:
    // コンストラクタ：ファイルパスと発生座標を受け取る
    EffekseerEffect(const std::wstring& filePath, const VECTOR& pos);
    virtual ~EffekseerEffect();

    virtual void Update() override;
    virtual void Draw() const override;

	void Release();

    void Play(VECTOR pos, Quaternion rot);

    void SetPosition(const VECTOR& pos);

	void SetRotation(const Quaternion& rot);

    //寿命管理
    void SetLifeTime(int frame);

	void Stop();

	void SetScale(float scale);

};