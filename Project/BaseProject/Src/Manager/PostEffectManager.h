#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
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

	// エフェクトパラメータ構造体(可変長対応)
	struct EffectParams
	{
		std::vector<FLOAT4> params;

		EffectParams() = default;
		
		// 1つのFLOAT4を設定
		EffectParams(float x, float y = 0.0f, float z = 0.0f, float w = 0.0f)
		{
			params.push_back({ x, y, z, w });
		}

		// 複数のFLOAT4を設定
		EffectParams(const std::vector<FLOAT4>& p) : params(p) {}

		// インデックスアクセス
		FLOAT4& operator[](size_t index) { return params[index]; }
		const FLOAT4& operator[](size_t index) const { return params[index]; }

		// サイズ取得
		size_t Size() const { return params.size(); }
	};

	// シングルトン取得
	static PostEffectManager& GetInstance();

	// 初期化・解放
	void Init();
	void Release();

	// 単一エフェクト適用
	void ApplyEffect(const EFFECT_TYPE type, const int srcScreen, const int dstScreen, const float time = 0.0f);
	void ApplyEffect(EFFECT_TYPE type, int srcScreen, int dstScreen, const EffectParams& customParams);

	// 複数エフェクトを順番に適用
	void ApplyEffects(const std::vector<EFFECT_TYPE>& effectTypes, 
	                  const int srcScreen, 
	                  const int dstScreen, 
	                  const float time = 0.0f);

	// カスタムパラメータ設定
	void SetCustomParams(const EFFECT_TYPE type, const EffectParams& params);
	EffectParams GetDefaultParams(const EFFECT_TYPE type, float time = 0.0f) const;

	// スクリーン管理
	int CreatePostEffectScreen();
	void DeletePostEffectScreen(int screen);

private:
	// 初期化補助
	void InitializeEffect(EFFECT_TYPE type, const char* shaderName, int paramCount = 1);
	void SetEffectParameters(EFFECT_TYPE type, EffectParams& params, float time) const;
	
	// 一時スクリーンの確保
	void EnsureTempScreens(int count);

	// エフェクトマップ
	std::unordered_map<EFFECT_TYPE, std::unique_ptr<PixelMaterial>> materials_;
	std::unordered_map<EFFECT_TYPE, std::unique_ptr<PixelRenderer>> renderers_;
	std::unordered_map<EFFECT_TYPE, EffectParams> customParams_;

	// 複数エフェクト適用用の一時スクリーン
	std::vector<int> tempScreens_;

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