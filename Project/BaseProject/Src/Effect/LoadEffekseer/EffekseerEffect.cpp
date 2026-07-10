#include  <filesystem>
#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>
#include "../../Common/Quaternion.h"

EffekseerEffect::EffekseerEffect(const std::wstring& filePath, const VECTOR& pos)
    : EffectBase(Vector2(0, 0)),
    m_effectHandle(-1),
    m_playingHandle(-1),
    m_pos3D(pos)
{
    if (!std::filesystem::exists(filePath))
    {
        printf("ファイルがありません\n");
    }
    else
    {
        printf("ファイル発見\n");
    }
	// Effekseerのエフェクトをロード
    m_effectHandle = LoadEffekseerEffect(filePath.c_str(), 1.0f);

	// エフェクトのロードに失敗した場合は即座に消滅フラグを立てる
    if (m_effectHandle == -1)
    {
        m_isDead = true;
    }
}

EffekseerEffect::~EffekseerEffect() 
{
    // 再生中なら停止 (0:再生中 / -1:終了)
    if (m_playingHandle != -1 && IsEffekseer3DEffectPlaying(m_playingHandle) == 0) 
    {
        StopEffekseer3DEffect(m_playingHandle);
    }

    // リソースの解放
    if (m_effectHandle != -1) {
        DeleteEffekseerEffect(m_effectHandle);
    }
}

void EffekseerEffect::Update() 
{
    if (m_isDead) return;


    // 任意寿命設定
    if (m_lifeTime > 0)
    {
        m_playFrame++;

        if (m_playFrame >= m_lifeTime)
        {
            if (m_playingHandle != -1)
            {
                StopEffekseer3DEffect(m_playingHandle);
            }

            m_isDead = true;
            return;
        }
    }


    // Effekseer側の終了判定
    if (m_playingHandle != -1 &&
        IsEffekseer3DEffectPlaying(m_playingHandle) == -1)
    {
        m_isDead = true;
    }
}

void EffekseerEffect::Draw() const
{
    // Effekseer自体の描画は GameScene 側で一括して DrawEffekseer3D() を呼ぶため、ここは空でOK
}

void EffekseerEffect::Play(VECTOR pos, Quaternion rot)
{
    m_playingHandle =
        PlayEffekseer3DEffect(m_effectHandle);


    SetPosPlayingEffekseer3DEffect(
        m_playingHandle,
        pos.x,
        pos.y,
        pos.z
    );


    SetScalePlayingEffekseer3DEffect(
        m_playingHandle,
        30.0f,
        30.0f,
        30.0f
    );


    // プレイヤーの向き
    VECTOR euler = rot.ToEuler();


    SetRotationPlayingEffekseer3DEffect(
        m_playingHandle,
        0.0f,
        euler.y ,
        0.0f
    );
}

void EffekseerEffect::SetPosition(const VECTOR& pos)
{
    m_pos3D = pos;
    if (m_playingHandle != -1 && IsEffekseer3DEffectPlaying(m_playingHandle) == 0) {
        SetPosPlayingEffekseer3DEffect(m_playingHandle, m_pos3D.x, m_pos3D.y, m_pos3D.z);
        SetScalePlayingEffekseer3DEffect(m_playingHandle,50.0f,50.0f,50.0f);
	}
}

void EffekseerEffect::SetLifeTime(int frame)
{
    m_lifeTime = frame;
}

void EffekseerEffect::Stop()
{
    if (m_playingHandle != -1)
    {
        StopEffekseer3DEffect(m_playingHandle);
    }

    m_isDead = true;
}
