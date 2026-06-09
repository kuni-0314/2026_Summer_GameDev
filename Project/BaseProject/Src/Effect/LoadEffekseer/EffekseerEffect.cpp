#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>

EffekseerEffect::EffekseerEffect(const std::wstring& filePath, const VECTOR& pos)
    : EffectBase(Vector2(0, 0)),
    m_effectHandle(-1),
    m_playingHandle(-1),
    m_pos3D(pos)
{

    m_effectHandle = LoadEffekseerEffect(filePath.c_str(), 1.0f);

	// 読み込みに成功していれば再生開始
    if (m_effectHandle != -1) {
        m_playingHandle = PlayEffekseer3DEffect(m_effectHandle);
        SetPosPlayingEffekseer3DEffect(m_playingHandle, m_pos3D.x, m_pos3D.y, m_pos3D.z);
    }
    else {
        m_isDead = true;
    }
}

EffekseerEffect::~EffekseerEffect() {
    // 再生中なら停止 (0:再生中 / -1:終了)
    if (m_playingHandle != -1 && IsEffekseer3DEffectPlaying(m_playingHandle) == 0) {
        StopEffekseer3DEffect(m_playingHandle);
    }
    // リソースの解放
    if (m_effectHandle != -1) {
        DeleteEffekseerEffect(m_effectHandle);
    }
}

void EffekseerEffect::Update() {
    if (m_isDead) return;

    // 3D表示のエフェクトが再生終了（-1）しているか取得
    if (m_playingHandle == -1 || IsEffekseer3DEffectPlaying(m_playingHandle) == -1) {
        m_isDead = true; // 終了していたら消滅フラグを立ててManagerに消してもらう
    }
}

void EffekseerEffect::Draw() const {
    // Effekseer自体の描画は GameScene 側で一括して DrawEffekseer3D() を呼ぶため、ここは空でOK
}

void EffekseerEffect::SetPosition(const VECTOR& pos)
{
    m_pos3D = pos;
    if (m_playingHandle != -1 && IsEffekseer3DEffectPlaying(m_playingHandle) == 0) {
        SetPosPlayingEffekseer3DEffect(m_playingHandle, m_pos3D.x, m_pos3D.y, m_pos3D.z);
        SetScalePlayingEffekseer3DEffect(m_playingHandle,50.0f,50.0f,50.0f);
	}
}
