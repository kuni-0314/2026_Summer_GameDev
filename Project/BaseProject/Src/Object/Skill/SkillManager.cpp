#include "SkillManager.h"
#include "../../Application.h"
#include"../../Utility/AsoUtility.h"
#include "../../Scene/GameScene.h"
#include <vector>
#include <string>
#include <fstream>
#include "SkillBase.h"

SkillManager::SkillManager(GameScene* gamescene, Player* player)
{
}

SkillManager::~SkillManager(void)
{
}
void SkillManager::Init(void)
{

	LoadCsvData();


}

void SkillManager::Update(void)
{

}

void SkillManager::Draw(void)
{

}
void SkillManager::Release(void)
{

}
void SkillManager::LoadCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "EnemyData.csv");
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
		SkillBase* enemy = nullptr;

		// 構造体に合わせて読込データを格納
		SkillBase::SkillData data = SkillBase::SkillData();
		int idx = 0;
		// ID
		data.id = stoi(strSplit[idx++]);
		// 種別
		data.type = static_cast<SkillBase::TYPE>(stoi(strSplit[idx++]));
		//　属性
		data.attribute = static_cast<SkillBase::ATTRIBUTE>(stoi(strSplit[idx++]));
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