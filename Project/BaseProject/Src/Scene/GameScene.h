#pragma once
//#include <map>
#include <memory>
#include "SceneBase.h"
#include "../Manager/PostEffectManager.h"
#include "../Object/Collider/ColliderBase.h"
class StageWall;
class Stage;
class SkyDome;
class Player;
class EnemyManager;
//class FieldManager;
class ItemManger;
class PixelMaterial;
class PixelRenderer;

class GameScene : public SceneBase
{
public:
	enum EFFECT : std::uint8_t
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
	GameScene();

	// デストラクタ
	~GameScene() override;

	// 初期化
	void Init() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 解放
	void Release() override;

	ItemManger* GetItemManger() const { return itemManger_; }

	struct AttackColliderData
	{
		ColliderBase* collider = nullptr;
		float damage = 0.0f;
		int lifeTime = 0;
	};
	
	void CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float Damage, int lifeTime);

	void SetEffectParameters(int effectType, FLOAT4& bufs, float time);
private:
	Stage* stage_;
	std::unique_ptr<StageWall> stageWall_;
	SkyDome* skyDome_;
	Player* player_;
	EnemyManager* enemyManager_;
	//FieldManager* fieldManager_;
	ItemManger* itemManger_;
	
	int targetEnemyId_;

	VECTOR targetPos_;

	std::vector<AttackColliderData*> attackColliders_;

	// シェーダー関連を簡略化
	PostEffectManager::EFFECT_TYPE currentEffect_;
	int postEffectScreen_;
	float effectTime_;

	int a_;
};
