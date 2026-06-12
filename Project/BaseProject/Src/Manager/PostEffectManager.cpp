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

	// 各エフェクトの初期化
	InitializeEffect(EFFECT_TYPE::NORMAL, "Normal.cso");
	InitializeEffect(EFFECT_TYPE::MONO, "Monotone.cso");
	InitializeEffect(EFFECT_TYPE::SEPIA, "Sepia.cso");
	InitializeEffect(EFFECT_TYPE::INVERT, "Invert.cso");
	InitializeEffect(EFFECT_TYPE::MOSAIC, "Mosaic.cso");
	InitializeEffect(EFFECT_TYPE::CHROM_ABR, "ChromaticAberration.cso");
	InitializeEffect(EFFECT_TYPE::VIGNETTE, "Vignette.cso");
	InitializeEffect(EFFECT_TYPE::SCANLINE, "Scanline.cso");
	InitializeEffect(EFFECT_TYPE::POSTERIZE, "Posterize.cso");
	InitializeEffect(EFFECT_TYPE::GLITCH, "Glitch.cso");
	InitializeEffect(EFFECT_TYPE::EMBOSS, "Emboss.cso");
	InitializeEffect(EFFECT_TYPE::RETROWAVE, "RetroWave.cso");
	InitializeEffect(EFFECT_TYPE::BLOOM, "Bloom.cso");
	InitializeEffect(EFFECT_TYPE::RIPPLE, "Ripple.cso");
	InitializeEffect(EFFECT_TYPE::RGB_SPLIT, "RGBSplit.cso");
	InitializeEffect(EFFECT_TYPE::PIXELATE, "Pixelate.cso");
	InitializeEffect(EFFECT_TYPE::SWIRL, "Swirl.cso");
	InitializeEffect(EFFECT_TYPE::RADIAL_BLUR, "RadialBlur.cso");
	InitializeEffect(EFFECT_TYPE::HUE_SHIFT, "HueShift.cso");
	InitializeEffect(EFFECT_TYPE::WAVE, "Wave.cso");
	InitializeEffect(EFFECT_TYPE::EDGE_DETECT, "EdgeDetection.cso");
	InitializeEffect(EFFECT_TYPE::OLD_FILM, "OldFilm.cso");
	InitializeEffect(EFFECT_TYPE::NIGHT_VISION, "NightVision.cso");
	InitializeEffect(EFFECT_TYPE::LIQUID_DIST, "LiquidDistortion.cso");
	InitializeEffect(EFFECT_TYPE::PINHOLE, "Pinhole.cso");
	InitializeEffect(EFFECT_TYPE::SPEED_LINES, "SpeedLines.cso");
	InitializeEffect(EFFECT_TYPE::FROSTED_GLASS, "FrostedGlass.cso");
	InitializeEffect(EFFECT_TYPE::DOT_MATRIX, "DotMatrix.cso");
	InitializeEffect(EFFECT_TYPE::DEPTH_FOG, "DepthFog.cso");
	InitializeEffect(EFFECT_TYPE::DIGITAL_RAIN, "DigitalRain.cso");
	InitializeEffect(EFFECT_TYPE::STROBE, "Strobe.cso");
	InitializeEffect(EFFECT_TYPE::SNOW_STORM, "SnowStorm.cso");
	InitializeEffect(EFFECT_TYPE::SCREEN_SHAKE, "ScreenShake.cso");
	InitializeEffect(EFFECT_TYPE::CRT, "CRT.cso");

	isInitialized_ = true;
}

void PostEffectManager::Release()
{
	materials_.clear();
	renderers_.clear();
	customParams_.clear();
	isInitialized_ = false;
}

void PostEffectManager::InitializeEffect(const EFFECT_TYPE type, const char* shaderName)
{
	materials_[type] = std::make_unique<PixelMaterial>(shaderName, 1);
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
	FLOAT4 bufs = {};
	
	// カスタムパラメータがあればそれを使用、なければデフォルト
	if (customParams_.contains(type))
	{
		//const auto& custom = customParams_[type];
		const auto& [x, y, z, w] = customParams_[type];
		bufs.x = x;
		bufs.y = y;
		bufs.z = z;
		bufs.w = w;
	}
	else
	{
		SetEffectParameters(type, bufs, time);
	}

	constBufsPtr->x = bufs.x;
	constBufsPtr->y = bufs.y;
	constBufsPtr->z = bufs.z;
	constBufsPtr->w = bufs.w;

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

void PostEffectManager::SetCustomParams(const EFFECT_TYPE type, const EffectParams& params)
{
	customParams_[type] = params;
}

PostEffectManager::EffectParams PostEffectManager::GetDefaultParams(const EFFECT_TYPE type, const float time) const
{
	EffectParams params;
	FLOAT4 bufs = {};
	SetEffectParameters(type, bufs, time);
	params.x = bufs.x;
	params.y = bufs.y;
	params.z = bufs.z;
	params.w = bufs.w;
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

void PostEffectManager::SetEffectParameters(EFFECT_TYPE type, FLOAT4& bufs, float time) const
{
	switch (type)
	{
	case EFFECT_TYPE::NORMAL:
		bufs.x = time;
		break;
	case EFFECT_TYPE::MONO:
		break;
	case EFFECT_TYPE::SEPIA:
		break;
	case EFFECT_TYPE::INVERT:
		break;
	case EFFECT_TYPE::MOSAIC:
		bufs.x = 16.0f;
		bufs.y = 10.0f;
		break;
	case EFFECT_TYPE::CHROM_ABR:
		bufs.x = 10.0f;
		break;
	case EFFECT_TYPE::VIGNETTE:
		bufs.x = 0.6f;
		bufs.y = 0.7f;
		break;
	case EFFECT_TYPE::SCANLINE:
		bufs.x = 100.0f;
		bufs.y = 0.3f;
		break;
	case EFFECT_TYPE::POSTERIZE:
		bufs.x = 4.0f;
		break;
	case EFFECT_TYPE::GLITCH:
		bufs.x = time;
		break;
	case EFFECT_TYPE::EMBOSS:
		bufs.x = 0.001f;
		bufs.y = 0.001f;
		break;
	case EFFECT_TYPE::RETROWAVE:
		bufs.x = 0.5f;
		break;
	case EFFECT_TYPE::BLOOM:
		bufs.x = 0.6f;
		bufs.y = 0.8f;
		break;
	case EFFECT_TYPE::RIPPLE:
		bufs.x = time;
		bufs.y = 2.0f;
		bufs.z = 1.5f;
		break;
	case EFFECT_TYPE::RGB_SPLIT:
		bufs.x = 0.025f;
		bufs.y = 0.05f;
		bufs.z = 0.025f;
		break;
	case EFFECT_TYPE::PIXELATE:
		bufs.x = 0.002f;
		break;
	case EFFECT_TYPE::SWIRL:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 1.0f;
		break;
	case EFFECT_TYPE::RADIAL_BLUR:
		bufs.x = -0.1f;
		break;
	case EFFECT_TYPE::HUE_SHIFT:
		bufs.x = time;
		break;
	case EFFECT_TYPE::WAVE:
		bufs.x = time;
		bufs.y = 0.1f;
		bufs.z = 2.0f;
		break;
	case EFFECT_TYPE::EDGE_DETECT:
		bufs.x = 0.15f;
		break;
	case EFFECT_TYPE::OLD_FILM:
		bufs.x = time;
		break;
	case EFFECT_TYPE::NIGHT_VISION:
		bufs.x = time;
		break;
	case EFFECT_TYPE::LIQUID_DIST:
		bufs.x = time;
		bufs.y = 0.1f;
		bufs.z = 3.0f;
		break;
	case EFFECT_TYPE::PINHOLE:
		bufs.x = 0.5f;
		bufs.y = 2.0f;
		break;
	case EFFECT_TYPE::SPEED_LINES:
		bufs.x = time;
		bufs.y = 100.0f;
		bufs.z = 100.0f;
		break;
	case EFFECT_TYPE::FROSTED_GLASS:
		bufs.x = 1.0f;
		bufs.y = 1.0f;
		break;
	case EFFECT_TYPE::DOT_MATRIX:
		bufs.x = 100.0f;
		bufs.y = 1.0f;
		break;
	case EFFECT_TYPE::DEPTH_FOG:
		bufs.x = 0.0f;
		bufs.y = 1.0f;
		bufs.z = 1.0f;
		break;
	case EFFECT_TYPE::DIGITAL_RAIN:
		bufs.x = time;
		bufs.y = 3.0f;
		bufs.z = 64.0f;
		break;
	case EFFECT_TYPE::STROBE:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 2.0f;
		break;
	case EFFECT_TYPE::SNOW_STORM:
		bufs.x = time;
		bufs.y = 40.0f;
		bufs.z = -1.0f;
		break;
	case EFFECT_TYPE::SCREEN_SHAKE:
		bufs.x = time;
		bufs.y = 0.01f;
		bufs.z = 0.01f;
		bufs.w = 0.01f;
		break;
	case EFFECT_TYPE::CRT:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 0.5f;
		bufs.w = 0.25f;
		break;
	default:
		break;
	}
}