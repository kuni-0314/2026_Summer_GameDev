#pragma once
#include <map>
#include <memory>
#include "SceneBase.h"
#include "../Object/Collider/ColliderBase.h"
class StageWall;
class Stage;
class SkyDome;
class Player;
class EnemyManager;
class FieldManager;
class ItemManger;
class PixelMaterial;
class PixelRenderer;

class GameScene : public SceneBase
{

public:
	enum EFFECT
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
	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	ItemManger* GetItemManger(void) const { return itemManger_; }

	struct AttackColliderData
	{
		ColliderBase* collider = nullptr;
		float damage = 0.0f;
		int lifeTime = 0;
	};
	
	void CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float Damage, int lifeTime);

	void SetEffectParameters(int effectType, FLOAT4& bufs, float time);

	void ApplyEffect(int effectType, int srcScreen, int dstScreen, float time);

private:
	Stage* stage_;
	std::unique_ptr<StageWall> stageWall_;
	SkyDome* skyDome_;
	Player* player_;
	EnemyManager* enemyManager_;
	FieldManager* fieldManager_;
	ItemManger* itemManger_;

	int targetEnemyId_;

	VECTOR targetPos_;

	std::vector<AttackColliderData*> attackColliders_;
	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;
	int postEffectScreen_;
	// エフェクトのマテリアルとレンダラー
	std::unique_ptr<PixelMaterial> materials_[MAX];
	std::unique_ptr<PixelRenderer> renderers_[MAX];
	// 現在適用しているエフェクト
	int currentEffect_;
};
