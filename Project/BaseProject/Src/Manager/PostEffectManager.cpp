#include <DxLib.h>
#include "../Application.h"
#include "PostEffectManager.h"
#include "SceneManager.h"

PostEffectManager& PostEffectManager::GetInstance()
{
	static PostEffectManager instance;
	return instance;
}

void PostEffectManager::Init()
{
	if (isInitialized_) return;

	// 各エフェクトの初期化(第3引数はFLOAT4の個数)
	InitializeEffect(EFFECT_TYPE::NORMAL, "Normal.cso", 1);
	InitializeEffect(EFFECT_TYPE::MONO, "Monotone.cso", 1);
	InitializeEffect(EFFECT_TYPE::SEPIA, "Sepia.cso", 1);
	InitializeEffect(EFFECT_TYPE::INVERT, "Invert.cso", 1);
	InitializeEffect(EFFECT_TYPE::MOSAIC, "Mosaic.cso", 1);
	InitializeEffect(EFFECT_TYPE::CHROM_ABR, "ChromaticAberration.cso", 1);
	InitializeEffect(EFFECT_TYPE::VIGNETTE, "Vignette.cso", 1);
	InitializeEffect(EFFECT_TYPE::SCANLINE, "Scanline.cso", 1);
	InitializeEffect(EFFECT_TYPE::POSTERIZE, "Posterize.cso", 1);
	InitializeEffect(EFFECT_TYPE::GLITCH, "Glitch.cso", 1);
	InitializeEffect(EFFECT_TYPE::EMBOSS, "Emboss.cso", 1);
	InitializeEffect(EFFECT_TYPE::RETROWAVE, "RetroWave.cso", 1);
	InitializeEffect(EFFECT_TYPE::BLOOM, "Bloom.cso", 1);
	InitializeEffect(EFFECT_TYPE::RIPPLE, "Ripple.cso", 1);
	InitializeEffect(EFFECT_TYPE::RGB_SPLIT, "RGBSplit.cso", 1);
	InitializeEffect(EFFECT_TYPE::PIXELATE, "Pixelate.cso", 1);
	InitializeEffect(EFFECT_TYPE::SWIRL, "Swirl.cso", 1);
	InitializeEffect(EFFECT_TYPE::RADIAL_BLUR, "RadialBlur.cso", 1);
	InitializeEffect(EFFECT_TYPE::HUE_SHIFT, "HueShift.cso", 1);
	InitializeEffect(EFFECT_TYPE::WAVE, "Wave.cso", 1);
	InitializeEffect(EFFECT_TYPE::EDGE_DETECT, "EdgeDetection.cso", 1);
	InitializeEffect(EFFECT_TYPE::OLD_FILM, "OldFilm.cso", 1);
	InitializeEffect(EFFECT_TYPE::NIGHT_VISION, "NightVision.cso", 1);
	InitializeEffect(EFFECT_TYPE::LIQUID_DIST, "LiquidDistortion.cso", 1);
	InitializeEffect(EFFECT_TYPE::PINHOLE, "Pinhole.cso", 1);
	InitializeEffect(EFFECT_TYPE::SPEED_LINES, "SpeedLines.cso", 1);
	InitializeEffect(EFFECT_TYPE::FROSTED_GLASS, "FrostedGlass.cso", 1);
	InitializeEffect(EFFECT_TYPE::DOT_MATRIX, "DotMatrix.cso", 1);
	InitializeEffect(EFFECT_TYPE::DEPTH_FOG, "DepthFog.cso", 1);
	InitializeEffect(EFFECT_TYPE::DIGITAL_RAIN, "DigitalRain.cso", 1);
	InitializeEffect(EFFECT_TYPE::STROBE, "Strobe.cso", 1);
	InitializeEffect(EFFECT_TYPE::SNOW_STORM, "SnowStorm.cso", 1);
	InitializeEffect(EFFECT_TYPE::SCREEN_SHAKE, "ScreenShake.cso", 2);
	InitializeEffect(EFFECT_TYPE::CRT, "CRT.cso", 1);
	InitializeEffect(EFFECT_TYPE::MAX, "FadeWhite.cso", 1);

	isInitialized_ = true;
}

void PostEffectManager::Release()
{
	materials_.clear();
	renderers_.clear();
	customParams_.clear();
	
	// 一時スクリーンを解放
	for (int screen : tempScreens_)
	{
		DeletePostEffectScreen(screen);
	}
	tempScreens_.clear();
	
	isInitialized_ = false;
}

void PostEffectManager::InitializeEffect(const EFFECT_TYPE type, const char* shaderName, int paramCount)
{
	materials_[type] = std::make_unique<PixelMaterial>(shaderName, paramCount);
	renderers_[type] = std::make_unique<PixelRenderer>(*materials_[type]);
	renderers_[type]->MakeSquareVertex();
}

void PostEffectManager::ApplyEffect(const EFFECT_TYPE type, const int srcScreen, const int dstScreen, const float time)
{
	if (!isInitialized_ || !materials_.contains(type)) return;

	SetDrawScreen(dstScreen);
	ClearDrawScreen();

	materials_[type]->Begin();
	materials_[type]->SetTexture(0, srcScreen);

	FLOAT4* constBufsPtr = materials_[type]->GetConstantBuffer();
	EffectParams params;
	
	// カスタムパラメータがあればそれを使用、なければデフォルト
	if (customParams_.contains(type))
	{
		params = customParams_[type];
	}
	else
	{
		SetEffectParameters(type, params, time);
	}

	// 定数バッファに設定
	// GPU用バッファのスロット数と、実際に設定されたパラメータ数の小さい方まで処理
	const int bufferSlotCount = materials_[type]->constBufFloat4Size_;
	const int paramCount = static_cast<int>(params.Size());
	const int copyCount = min(bufferSlotCount, paramCount);
	
	for (int i = 0; i < copyCount; ++i)
	{
		// C++のvectorから → GPUバッファへ順番にコピー
		constBufsPtr[i] = params[i];
	}

	materials_[type]->UpdateConstantBuffer(CONSTANT_BUF_SLOT_BEGIN_PS);
	renderers_[type]->Draw();
	materials_[type]->SetTexture(0, -1);
	materials_[type]->End();
}

void PostEffectManager::ApplyEffect(const EFFECT_TYPE type, const int srcScreen, const int dstScreen, const EffectParams& customParams)
{
	customParams_[type] = customParams;
	ApplyEffect(type, srcScreen, dstScreen, 0.0f);
	customParams_.erase(type); // 一時的なパラメータなので削除
}

void PostEffectManager::ApplyEffects(const std::vector<EFFECT_TYPE>& effectTypes, 
                                     const int srcScreen, 
                                     const int dstScreen, 
                                     const float time)
{
	if (!isInitialized_) return;
	
	// エフェクトがない場合は元画像をコピー
	if (effectTypes.empty())
	{
		SetDrawScreen(dstScreen);
		ClearDrawScreen();
		DrawGraph(0, 0, srcScreen, TRUE);
		return;
	}

	// エフェクトが1つだけの場合
	if (effectTypes.size() == 1)
	{
		ApplyEffect(effectTypes[0], srcScreen, dstScreen, time);
		return;
	}

	// 必要な一時スクリーン数を確保 (エフェクト数 - 1)
	const int tempScreenCount = static_cast<int>(effectTypes.size()) - 1;
	EnsureTempScreens(tempScreenCount);

	// 最初のエフェクト: srcScreen → tempScreens_[0]
	ApplyEffect(effectTypes[0], srcScreen, tempScreens_[0], time);

	// 中間のエフェクト: tempScreens_[i-1] → tempScreens_[i]
	for (size_t i = 1; i < effectTypes.size() - 1; ++i)
	{
		ApplyEffect(effectTypes[i], tempScreens_[i - 1], tempScreens_[i], time);
	}

	// 最後のエフェクト: tempScreens_[last-1] → dstScreen
	ApplyEffect(effectTypes.back(), tempScreens_[tempScreenCount - 1], dstScreen, time);
}

void PostEffectManager::SetCustomParams(const EFFECT_TYPE type, const EffectParams& params)
{
	customParams_[type] = params;
}

PostEffectManager::EffectParams PostEffectManager::GetDefaultParams(const EFFECT_TYPE type, const float time) const
{
	EffectParams params;
	SetEffectParameters(type, params, time);
	return params;
}

int PostEffectManager::CreatePostEffectScreen()
{
	return MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void PostEffectManager::DeletePostEffectScreen(int screen)
{
	if (screen != -1)
	{
		DeleteGraph(screen);
	}
}

void PostEffectManager::EnsureTempScreens(int count)
{
	// 既存のスクリーンより多く必要な場合は追加作成
	while (static_cast<int>(tempScreens_.size()) < count)
	{
		tempScreens_.push_back(CreatePostEffectScreen());
	}
}

void PostEffectManager::SetEffectParameters(EFFECT_TYPE type, EffectParams& params, float time) const
{
	FLOAT4 buf = {};
	
	switch (type)
	{
	case EFFECT_TYPE::NORMAL:
		buf.x = time;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::MONO:
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::SEPIA:
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::INVERT:
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::MOSAIC:
		buf.x = 16.0f;
		buf.y = 10.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::CHROM_ABR:
		buf.x = 10.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::VIGNETTE:
		buf.x = 0.6f;
		buf.y = 0.7f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::SCANLINE:
		buf.x = 100.0f;
		buf.y = 0.3f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::POSTERIZE:
		buf.x = 4.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::GLITCH:
		buf.x = time;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::EMBOSS:
		buf.x = 0.001f;
		buf.y = 0.001f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::RETROWAVE:
		buf.x = 0.5f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::BLOOM:
		buf.x = 0.6f;
		buf.y = 0.8f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::RIPPLE:
		buf.x = time;
		buf.y = 2.0f;
		buf.z = 1.5f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::RGB_SPLIT:
		buf.x = 0.025f;
		buf.y = 0.05f;
		buf.z = 0.025f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::PIXELATE:
		buf.x = 0.002f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::SWIRL:
		buf.x = time;
		buf.y = 1.0f;
		buf.z = 1.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::RADIAL_BLUR:
		buf.x = -0.1f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::HUE_SHIFT:
		buf.x = time;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::WAVE:
		buf.x = time;
		buf.y = 0.1f;
		buf.z = 2.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::EDGE_DETECT:
		buf.x = 0.15f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::OLD_FILM:
		buf.x = time;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::NIGHT_VISION:
		buf.x = time;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::LIQUID_DIST:
		buf.x = time;
		buf.y = 0.1f;
		buf.z = 3.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::PINHOLE:
		buf.x = 0.5f;
		buf.y = 2.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::SPEED_LINES:
		buf.x = time;
		buf.y = 100.0f;
		buf.z = 100.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::FROSTED_GLASS:
		buf.x = 1.0f;
		buf.y = 1.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::DOT_MATRIX:
		buf.x = 100.0f;
		buf.y = 1.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::DEPTH_FOG:
		buf.x = 0.0f;
		buf.y = 1.0f;
		buf.z = 1.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::DIGITAL_RAIN:
		buf.x = time;
		buf.y = 3.0f;
		buf.z = 64.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::STROBE:
		buf.x = time;
		buf.y = 1.0f;
		buf.z = 2.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::SNOW_STORM:
		buf.x = time;
		buf.y = 40.0f;
		buf.z = -1.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::SCREEN_SHAKE:
		buf.x = time;
		buf.y = 0.01f;
		buf.z = 0.01f;
		buf.w = 0.01f;
		params.params.push_back(buf);
		buf = {};
		buf.x = 100.0f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::CRT:
		buf.x = time;
		buf.y = 1.0f;
		buf.z = 0.5f;
		buf.w = 0.25f;
		params.params.push_back(buf);
		break;
	case EFFECT_TYPE::FADE_WHITE:
		buf.x = time;
		params.params.push_back(buf);
		break;
	default:
		params.params.push_back(buf);
		break;
	}
}