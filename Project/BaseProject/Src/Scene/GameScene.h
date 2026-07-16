#pragma once
#include <memory>
#include <vector>
#include <DxLib.h>
#include "SceneBase.h"
#include "../Manager/PostEffectManager.h"
#include "../Object/Collider/ColliderBase.h"

class Stage;
class StageWall;
class SkyDome;
class Player;
class EnemyManager;
class ItemManger;
class SwordBase;
//class ColliderBase;

class GameScene : public SceneBase
{
public:
	// 攻撃コライダーデータ
	struct AttackColliderData
	{
		ColliderBase* collider;
		float damage;
		int lifeTime;
		bool isHit = false;
	};

	// 
	enum CAM_MODE
	{
		MANUAL,
		TARGETING,
	};;

	enum COMMAND
	{
		THUNDER,
		FIRE,
		HEAL,
		ALL,
		MAX
	};

	enum COMMAND_STATE
	{
		NOT_USE,
		USE
	};

	enum PLAYRE_HP_STATE
	{
		DEF,
		DAMEGE,
		WARNIG,
		STATE_MAX
	};

	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	// 攻撃コライダー生成
	void CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float damage, int lifeTime);

	void AddEnemyHitCollider(const ColliderBase* hitCollider);
	void RemoveEnemyHitCollider(const ColliderBase* hitCollider);

	ItemManger* GetItemManger() const { return itemManger_; }

	// ゲームシーンを取得
	const GameScene& GetGameScene() const { return *this; }

	// コマンドの変更
	void SelectCommand(COMMAND command);

	COMMAND GetSelectedCommand() const { return static_cast<COMMAND>(selectCommand_); }

	void  SetDamageFlag(bool flag);
	bool  GetFlag();

	CAM_MODE GetCamMode() const { return camMode_; }

	VECTOR GetTargetPos() const { return targetPos_; }

private:

	// ステージ
	Stage* stage_;

	// ステージの壁
	std::unique_ptr<StageWall> stageWall_;

	// スカイドーム
	SkyDome* skyDome_;

	// プレイヤー
	Player* player_;

	// 敵マネージャー
	EnemyManager* enemyManager_;

	// アイテムマネージャー
	ItemManger* itemManger_;

	// 武器
	//WeaponBase* weapon_;

	static constexpr int IMG_HP_X = 1550;
	static constexpr int IMG_HP_Y = 700;

	//コマンド
	COMMAND command_;

	COMMAND_STATE thunderState_;
	COMMAND_STATE fireState_;
	COMMAND_STATE healState_;

	int selectCommand_;


	// 攻撃コライダーリスト
	std::vector<AttackColliderData*> attackColliders_;

	// ポストエフェクト用スクリーン
	int postEffectScreen_;

	// 現在適用しているエフェクト(単一選択モード用)
	PostEffectManager::EFFECT_TYPE currentEffect_;

	// 複数エフェクト適用リスト
	std::vector<PostEffectManager::EFFECT_TYPE> activeEffects_;

	// エフェクトモード切り替え (true: 複数, false: 単一)
	bool multiEffectMode_;

	// エフェクトの時間パラメータ
	float effectTime_;

	int targetEnemyId_;		// ターゲット敵のID
	int hpUiCount_;			//HPUi用

	VECTOR targetPos_;		// ターゲット位置
	CAM_MODE camMode_;		// カメラモード

	// エフェクトをトグル(追加/削除)
	void ToggleEffect(PostEffectManager::EFFECT_TYPE effectType);

	// エフェクト名を取得
	const char* GetEffectName(PostEffectManager::EFFECT_TYPE effectType);

	//プレイヤーHPUI
	void PlayerHpDraw();

	//コマンドUI
	int hpIndex_;

	bool warnigOn_ = false;//警告生成フラグ
	bool DamegeOn_ = false;//ダメージ
	int wargnigHandle_;
	int audioHandle_;

	int damegeTimeCount_;
	bool damegeflag_ = false;

	int clearTimer_ = 0;
	bool isClear_ = false;


	std::vector<int> hpHandles_;			//Hpハンドル
	std::vector<int> playerUiHandles_;		//プレイヤーUI
	std::vector<int> commandHandles_;		//コマンドハンドル
	int lockOnFontHandle_;

	int fontCommandHandles_[static_cast<int>(COMMAND::MAX)][2];//セレクトコマンドファント

	//void SelectCommand(COMMAND command);
	void CommandUpdate();
	void CommandDraw();

	void PlayerFaceUIDrow();

	int lockOnImageHandle_;
};