#pragma once
#include <functional>
#include <DxLib.h>
#include<string>
class Player;

class SkillBase
{
public:


	// 種別
	enum class TYPE
	{
		PHYSICS,
		FIRE,
		WATER,
		WIND,
		SHINE,
		DARK,
		BUFF,
		DEBUFF
	};
	//スキルデータ
	struct SkillData
	{
		int id;					// ID
		std::string skillName;	// スキル名
		SkillBase::TYPE type;	// スキルタイプ
		float physicsAtkMag;	//物理攻撃倍率
		float physicsDefMag;	//物理防御倍率
		float magicAtkMag;		//魔法攻撃倍率
		float magicDefMag;		//魔法防御倍率
		float recoveryMag;		//回復倍率
		float buffMag;			// バフ倍率
	};

	// コンストラクタ
	SkillBase();
	//デストラクタ
	~SkillBase();

protected:





	

};

