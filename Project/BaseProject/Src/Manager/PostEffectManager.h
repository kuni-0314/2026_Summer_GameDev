#pragma once
#include <memory>
#include <unordered_map>
#include "../Shader/PixelMaterial.h"
#include "../Shader/PixelRenderer.h"

class PostEffectManager
{
public:
	// エフェクトタイプの定義
	enum class EffectType
	{
		NORMAL,
		MONO,
		SEPIA,
		INVERT,
		MOSAIC,
		CHROM_ABR,
		VIGNETTE,
		SCANLINE,
		POSTERIZE,
		GLITCH,
		EMBOSS,
		RETROWAVE,
		BLOOM,
		RIPPLE,
		RGB_SPLIT,
		PIXELATE,
		SWIRL,
		RADIAL_BLUR,
		HUE_SHIFT,
		WAVE,
		EDGE_DETECT,
		OLD_FILM,
		NIGHT_VISION,
		LIQUID_DIST,
		PINHOLE,
		SPEED_LINES,
		FROSTED_GLASS,
		DOT_MATRIX,
		DEPTH_FOG,
		DIGITAL_RAIN,
		STROBE,
		SNOW_STORM,
		SCREEN_SHAKE,
		CRT,
		MAX
	};

	// エフェクトパラメータ構造体
	struct EffectParams
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;
	};

	// シングルトン取得
	static PostEffectManager& GetInstance();

	// 初期化・解放
	void Init();
	void Release();

	// エフェクト適用
	void ApplyEffect(EffectType type, int srcScreen, int dstScreen, float time = 0.0f);
	void ApplyEffect(EffectType type, int srcScreen, int dstScreen, const EffectParams& customParams);

	// カスタムパラメータ設定
	void SetCustomParams(EffectType type, const EffectParams& params);
	EffectParams GetDefaultParams(EffectType type, float time = 0.0f) const;

	// スクリーン管理
	int CreatePostEffectScreen();
	void DeletePostEffectScreen(int screen);

private:
	PostEffectManager() = default;
	~PostEffectManager() = default;
	PostEffectManager(const PostEffectManager&) = delete;
	PostEffectManager& operator=(const PostEffectManager&) = delete;

	// 初期化補助
	void InitializeEffect(EffectType type, const char* shaderName);
	void SetEffectParameters(EffectType type, FLOAT4& bufs, float time) const;

	// エフェクトマップ
	std::unordered_map<EffectType, std::unique_ptr<PixelMaterial>> materials_;
	std::unordered_map<EffectType, std::unique_ptr<PixelRenderer>> renderers_;
	std::unordered_map<EffectType, EffectParams> customParams_;

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	bool isInitialized_ = false;
};

using PEManager = PostEffectManager;