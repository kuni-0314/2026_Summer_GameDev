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
	InitializeEffect(EffectType::NORMAL, "Normal.cso");
	InitializeEffect(EffectType::MONO, "Monotone.cso");
	InitializeEffect(EffectType::SEPIA, "Sepia.cso");
	InitializeEffect(EffectType::INVERT, "Invert.cso");
	InitializeEffect(EffectType::MOSAIC, "Mosaic.cso");
	InitializeEffect(EffectType::CHROM_ABR, "ChromaticAberration.cso");
	InitializeEffect(EffectType::VIGNETTE, "Vignette.cso");
	InitializeEffect(EffectType::SCANLINE, "Scanline.cso");
	InitializeEffect(EffectType::POSTERIZE, "Posterize.cso");
	InitializeEffect(EffectType::GLITCH, "Glitch.cso");
	InitializeEffect(EffectType::EMBOSS, "Emboss.cso");
	InitializeEffect(EffectType::RETROWAVE, "RetroWave.cso");
	InitializeEffect(EffectType::BLOOM, "Bloom.cso");
	InitializeEffect(EffectType::RIPPLE, "Ripple.cso");
	InitializeEffect(EffectType::RGB_SPLIT, "RGBSplit.cso");
	InitializeEffect(EffectType::PIXELATE, "Pixelate.cso");
	InitializeEffect(EffectType::SWIRL, "Swirl.cso");
	InitializeEffect(EffectType::RADIAL_BLUR, "RadialBlur.cso");
	InitializeEffect(EffectType::HUE_SHIFT, "HueShift.cso");
	InitializeEffect(EffectType::WAVE, "Wave.cso");
	InitializeEffect(EffectType::EDGE_DETECT, "EdgeDetection.cso");
	InitializeEffect(EffectType::OLD_FILM, "OldFilm.cso");
	InitializeEffect(EffectType::NIGHT_VISION, "NightVision.cso");
	InitializeEffect(EffectType::LIQUID_DIST, "LiquidDistortion.cso");
	InitializeEffect(EffectType::PINHOLE, "Pinhole.cso");
	InitializeEffect(EffectType::SPEED_LINES, "SpeedLines.cso");
	InitializeEffect(EffectType::FROSTED_GLASS, "FrostedGlass.cso");
	InitializeEffect(EffectType::DOT_MATRIX, "DotMatrix.cso");
	InitializeEffect(EffectType::DEPTH_FOG, "DepthFog.cso");
	InitializeEffect(EffectType::DIGITAL_RAIN, "DigitalRain.cso");
	InitializeEffect(EffectType::STROBE, "Strobe.cso");
	InitializeEffect(EffectType::SNOW_STORM, "SnowStorm.cso");
	InitializeEffect(EffectType::SCREEN_SHAKE, "ScreenShake.cso");
	InitializeEffect(EffectType::CRT, "CRT.cso");

	isInitialized_ = true;
}

void PostEffectManager::Release()
{
	materials_.clear();
	renderers_.clear();
	customParams_.clear();
	isInitialized_ = false;
}

void PostEffectManager::InitializeEffect(EffectType type, const char* shaderName)
{
	materials_[type] = std::make_unique<PixelMaterial>(shaderName, 1);
	renderers_[type] = std::make_unique<PixelRenderer>(*materials_[type]);
	renderers_[type]->MakeSquareVertex();
}

void PostEffectManager::ApplyEffect(EffectType type, int srcScreen, int dstScreen, float time)
{
	if (!isInitialized_ || materials_.find(type) == materials_.end()) return;

	SetDrawScreen(dstScreen);
	ClearDrawScreen();

	materials_[type]->Begin();
	materials_[type]->SetTexture(0, srcScreen);

	FLOAT4* constBufsPtr = materials_[type]->GetConstantBuffer();
	FLOAT4 bufs = {};
	
	// カスタムパラメータがあればそれを使用、なければデフォルト
	if (customParams_.find(type) != customParams_.end())
	{
		const auto& custom = customParams_[type];
		bufs.x = custom.x;
		bufs.y = custom.y;
		bufs.z = custom.z;
		bufs.w = custom.w;
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

void PostEffectManager::ApplyEffect(EffectType type, int srcScreen, int dstScreen, const EffectParams& customParams)
{
	customParams_[type] = customParams;
	ApplyEffect(type, srcScreen, dstScreen, 0.0f);
	customParams_.erase(type); // 一時的なパラメータなので削除
}

void PostEffectManager::SetCustomParams(EffectType type, const EffectParams& params)
{
	customParams_[type] = params;
}

PostEffectManager::EffectParams PostEffectManager::GetDefaultParams(EffectType type, float time) const
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

void PostEffectManager::SetEffectParameters(EffectType type, FLOAT4& bufs, float time) const
{
	switch (type)
	{
	case EffectType::NORMAL:
		bufs.x = time;
		break;
	case EffectType::MONO:
		break;
	case EffectType::SEPIA:
		break;
	case EffectType::INVERT:
		break;
	case EffectType::MOSAIC:
		bufs.x = 16.0f;
		bufs.y = 10.0f;
		break;
	case EffectType::CHROM_ABR:
		bufs.x = 10.0f;
		break;
	case EffectType::VIGNETTE:
		bufs.x = 0.6f;
		bufs.y = 0.7f;
		break;
	case EffectType::SCANLINE:
		bufs.x = 100.0f;
		bufs.y = 0.3f;
		break;
	case EffectType::POSTERIZE:
		bufs.x = 4.0f;
		break;
	case EffectType::GLITCH:
		bufs.x = time;
		break;
	case EffectType::EMBOSS:
		bufs.x = 0.001f;
		bufs.y = 0.001f;
		break;
	case EffectType::RETROWAVE:
		bufs.x = 0.5f;
		break;
	case EffectType::BLOOM:
		bufs.x = 0.6f;
		bufs.y = 0.8f;
		break;
	case EffectType::RIPPLE:
		bufs.x = time;
		bufs.y = 2.0f;
		bufs.z = 1.5f;
		break;
	case EffectType::RGB_SPLIT:
		bufs.x = 0.025f;
		bufs.y = 0.05f;
		bufs.z = 0.025f;
		break;
	case EffectType::PIXELATE:
		bufs.x = 0.002f;
		break;
	case EffectType::SWIRL:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 1.0f;
		break;
	case EffectType::RADIAL_BLUR:
		bufs.x = -0.1f;
		break;
	case EffectType::HUE_SHIFT:
		bufs.x = time;
		break;
	case EffectType::WAVE:
		bufs.x = time;
		bufs.y = 0.1f;
		bufs.z = 2.0f;
		break;
	case EffectType::EDGE_DETECT:
		bufs.x = 0.15f;
		break;
	case EffectType::OLD_FILM:
		bufs.x = time;
		break;
	case EffectType::NIGHT_VISION:
		bufs.x = time;
		break;
	case EffectType::LIQUID_DIST:
		bufs.x = time;
		bufs.y = 0.1f;
		bufs.z = 3.0f;
		break;
	case EffectType::PINHOLE:
		bufs.x = 0.5f;
		bufs.y = 2.0f;
		break;
	case EffectType::SPEED_LINES:
		bufs.x = time;
		bufs.y = 100.0f;
		bufs.z = 100.0f;
		break;
	case EffectType::FROSTED_GLASS:
		bufs.x = 1.0f;
		bufs.y = 1.0f;
		break;
	case EffectType::DOT_MATRIX:
		bufs.x = 100.0f;
		bufs.y = 1.0f;
		break;
	case EffectType::DEPTH_FOG:
		bufs.x = 0.0f;
		bufs.y = 1.0f;
		bufs.z = 1.0f;
		break;
	case EffectType::DIGITAL_RAIN:
		bufs.x = time;
		bufs.y = 3.0f;
		bufs.z = 64.0f;
		break;
	case EffectType::STROBE:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 2.0f;
		break;
	case EffectType::SNOW_STORM:
		bufs.x = time;
		bufs.y = 40.0f;
		bufs.z = -1.0f;
		break;
	case EffectType::SCREEN_SHAKE:
		bufs.x = time;
		bufs.y = 0.01f;
		bufs.z = 0.01f;
		bufs.w = 0.01f;
		break;
	case EffectType::CRT:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 0.5f;
		bufs.w = 0.25f;
		break;
	default:
		break;
	}
}