#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"

enum class SKILL_ID
{
	SLASH, //スラッシュ
	HEAL,  //ヒール
	FLAME, //フレイム
};

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
	SKILL_ID id;                 //スキルID
	std::string skillName;	// スキル名
	TYPE type;				// スキルタイプ
	ATTRIBUTE attribute;	// スキル属性
	float physAtkMag;		// 物理攻撃倍率/物理攻撃バフ倍率/物理攻撃デバフ倍率
	float physDefMag;		// 物理防御倍率/物理防御バフ倍率/物理防御デバフ倍率
	float magicAtkMag;		// 魔法攻撃倍率/魔法攻撃バフ倍率/魔法攻撃デバフ倍率
	float magicDefMag;		// 魔法防御倍率/魔法防御バフ倍率/魔法防御デバフ倍率
	float recoveryMag;		// 回復倍率
};


void LoadCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "SkillTable.csv");
	if (!ifs)
	{
		// エラーが発生
		return;
	}
	// ファイルを１行ずつ読み込む
	std::string line;// 1行の文字情報
	std::vector<std::string> strSplit; // 1行を1文字の動的配列に分割
	bool isHeader = true;
	while (getline(ifs, line))
	{
		if (isHeader)
		{
			isHeader = false;
			continue;
		}
		// １行をカンマ区切りで分割
		strSplit = AsoUtility::Split(line, ',');
		//SkillBase* enemy = nullptr;

		// 構造体に合わせて読込データを格納
		SkillData data = SkillData();
		int idx = 0;
		// ID
		data.id = static_cast<SKILL_ID>(stoi(strSplit[idx++]));
		// 種別
		data.type = static_cast<TYPE>(stoi(strSplit[idx++]));
		//　属性
		data.attribute = static_cast<ATTRIBUTE>(stoi(strSplit[idx++]));
		// 物理攻撃倍率
		data.physAtkMag = stoi(strSplit[idx++]);
		// 物理攻撃倍率
		data.physDefMag = stoi(strSplit[idx++]);
		// 物理攻撃倍率
		data.magicAtkMag = stoi(strSplit[idx++]);
		// 物理攻撃倍率
		data.magicDefMag = stoi(strSplit[idx++]);
		// 物理攻撃倍率
		data.recoveryMag = stoi(strSplit[idx++]);

		// エネミー生成
		//Create(data, player_);
	}
	ifs.close();
	
}
