#pragma once
#include <memory>
#include <unordered_map>
#include "../Shader/PixelMaterial.h"
#include "../Shader/PixelRenderer.h"

class PostEffectManager
{
public:
	// エフェクトタイプの定義
	enum class EFFECT_TYPE : std::uint8_t
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
	void ApplyEffect(const EFFECT_TYPE type, const int srcScreen, const int dstScreen, const float time = 0.0f);
	void ApplyEffect(EFFECT_TYPE type, int srcScreen, int dstScreen, const EffectParams& customParams);

	// カスタムパラメータ設定
	void SetCustomParams(const EFFECT_TYPE type, const EffectParams& params);
	EffectParams GetDefaultParams(const EFFECT_TYPE type, float time = 0.0f) const;

	// スクリーン管理
	int CreatePostEffectScreen();
	void DeletePostEffectScreen(int screen);
private:
	// 初期化補助
	void InitializeEffect(EFFECT_TYPE type, const char* shaderName);
	void SetEffectParameters(EFFECT_TYPE type, FLOAT4& bufs, float time) const;

	// エフェクトマップ
	std::unordered_map<EFFECT_TYPE, std::unique_ptr<PixelMaterial>> materials_;
	std::unordered_map<EFFECT_TYPE, std::unique_ptr<PixelRenderer>> renderers_;
	std::unordered_map<EFFECT_TYPE, EffectParams> customParams_;

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	bool isInitialized_ = false;

	// コンストラクタ・デストラクタは非公開
public:
	PostEffectManager(const PostEffectManager&) = delete;
	PostEffectManager& operator=(const PostEffectManager&) = delete;
private:
	PostEffectManager() = default;
	~PostEffectManager() = default;
};

using PstEfcManager = PostEffectManager;