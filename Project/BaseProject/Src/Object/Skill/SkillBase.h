#pragma once
#include <string>
#include <map>
class Player;

class SkillBase
{
public:
	// 種別
	enum class TYPE
	{
		NONE,		// 無
		PHYSICS,	// 物理
		MAGIC,		// 魔法
	};

	// 属性
	enum class ATTRIBUTE
	{
		NONE,	// 無
		FIRE,	// 火
		WATER,	// 水
		WIND,	// 風
		SHINE,	// 光
		DARK,	// 闇
		BUFF,	// バフ
		DEBUFF,	// デバフ
	};

	//スキルデータ
	struct SkillData
	{
		int id;                 //スキルID
		std::string skillName;	// スキル名
		TYPE type;				// スキルタイプ
		ATTRIBUTE attribute;	// スキル属性
		float physAtkMag;		// 物理攻撃倍率/物理攻撃バフ倍率/物理攻撃デバフ倍率
		float physDefMag;		// 物理防御倍率/物理防御バフ倍率/物理防御デバフ倍率
		float magicAtkMag;		// 魔法攻撃倍率/魔法攻撃バフ倍率/魔法攻撃デバフ倍率
		float magicDefMag;		// 魔法防御倍率/魔法防御バフ倍率/魔法防御デバフ倍率
		float recoveryMag;		// 回復倍率
	};

	// コンストラクタ
	SkillBase(const SkillBase::SkillData& data,Player*player);
	//デストラクタ
	~SkillBase();
	
	void Init();		// 初期化
	void Update();		// 更新
	void Draw();		// 描画
	void Release();		// 解放

protected:
	void LoadSkillData();	// スキルデータファイルの読み込み

	//プレイヤー
	Player* player_; 
	//スキルタイプ
	TYPE type_;
	//スキル属性
	ATTRIBUTE attribute_;

	float physAtkMag_;
	float physDefMag_;		// 物理防御倍率
	float magicAtkMag_;		// 魔法攻撃倍率
	float magicDefMag_;		// 魔法防御倍率
	float recoveryMag_;

	std::map<int, SkillData> skillDataMap_;	// スキルデータ

	
};