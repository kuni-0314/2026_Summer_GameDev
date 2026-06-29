#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ƒŠƒ\[ƒX–¼
	enum class SRC
	{
		TITLE, //ƒ^ƒCƒgƒ‹‰æ‘œ
		TITLE_PUSH,	//push‰æ‘œ
		TITLE_GAMESTART,	//ƒXƒ^[ƒg‰æ‘œ
		TITLE_NOT_GAMESTART,//”ñ‘I‘ğƒXƒ^[ƒg‰æ‘œ
		TITLE_END,	//I—¹‰æ‘œ
		TITLE_NOT_END,//”ñ‘I‘ğI—¹‰æ‘œ
		TITLE_TUTORIAL,	//ƒ`ƒ…[ƒgƒŠƒAƒ‹‰æ‘œ
		TITLE_NOT_TUTORIAL,//”ñ‘I‘ğƒ`ƒ…[ƒgƒŠƒAƒ‹‰æ‘œ
		TITLE_OPTION,	//ƒIƒvƒVƒ‡ƒ“‰æ‘œ
		TITLE_NOT_OPTION,//”ñ‘I‘ğƒIƒvƒVƒ‡ƒ“‰æ‘œ
		TITLE_PLAYER,	//ƒ^ƒCƒgƒ‹ƒvƒŒƒCƒ„[
		PIT_FALL_PLANET,	//ƒƒCƒ“˜f¯
		SPHERE_PLANET,		//ƒTƒu˜f¯
		PLAYER,				//ƒvƒŒƒCƒ„[
		MAIN_STAGE,			//ƒXƒe[ƒWiƒQ[ƒ€ƒV[ƒ“j
		SKY_DOME,			//ƒXƒJƒCƒh[ƒ€iƒQ[ƒ€ƒV[ƒ“
		PLAYER_SHADOW,		//ƒVƒƒƒhƒE
		ENEMY_RAT,
		ENEMY_RASE,			//‚ç‚¹
		ENEMY_RASE_BALL,	//UŒ‚—pƒ‚ƒfƒ‹
		ENEMY_LARGE,
		VIEW_RANGE,			//“G‚Ì‹–ì
		CELL_RED,			// ÔFƒZƒ‹
		BATTLE_STAGE,		//ƒoƒgƒ‹ƒXƒe[ƒW
		BATTLE_WALL,		//ƒXƒe[ƒW•Ç
		ITEM_HP,			//‘Ì—ÍƒAƒCƒeƒ€
		ITEM_SKILL,			//ƒXƒLƒ‹ƒAƒCƒeƒ€
		KEY_BLADE_1,	//ƒL[ƒuƒŒ[ƒh‚P
		KEY_BLADE_2,	//ƒL[ƒuƒŒ[ƒh‚Q
		KEY_BLADE_3,	//ƒL[ƒuƒŒ[ƒh‚R
<<<<<<< HEAD
		TARGET_CURSOR_BLUE,	//ƒ^[ƒQƒbƒgƒJ[ƒ\ƒ‹Â
		TARGET_CURSOR_ORANGE,	//ƒ^[ƒQƒbƒgƒJ[ƒ\ƒ‹ƒIƒŒƒ“ƒW
=======
		IMG_PLAYER_HP_0,
		IMG_PLAYER_HP_1,
		IMG_PLAYER_HP_2,
		IMG_PLAYER_HP_3,
		IMG_PLAYER_HP_4,
		IMG_PLAYER_HP_5,
		IMG_PLAYER_HP_6,
		IMG_PLAYER_HP_7,
		IMG_PLAYER_HP_8,
		IMG_PLAYER_HP_9,
		IMG_PLAYER_HP_10,
>>>>>>> origin/dxlibãƒ†ã‚¹ãƒˆ
		MAX
	};

	// –¾¦“I‚ÉƒCƒ“ƒXƒeƒ“ƒX‚ğ¶¬‚·‚é
	static void CreateInstance();

	// Ã“IƒCƒ“ƒXƒ^ƒ“ƒX‚Ìæ“¾
	static ResourceManager& GetInstance();

	// ‰Šú‰»
	void Init();

	// ‰ğ•ú(ƒV[ƒ“Ø‘Ö‚Éˆê’U‰ğ•ú)
	void Release();

	// ƒŠƒ\[ƒX‚ÌŠ®‘S”jŠü
	void Destroy();

	// ƒŠƒ\[ƒX‚Ìƒ[ƒh
	const Resource& Load(SRC src);

	// ƒŠƒ\[ƒX‚Ì•¡»ƒ[ƒh(ƒ‚ƒfƒ‹—p)
	int LoadModelDuplicate(SRC src);

private:

	// Ã“IƒCƒ“ƒXƒ^ƒ“ƒX
	static ResourceManager* instance_;

	// ƒŠƒ\[ƒXŠÇ—‚Ì‘ÎÛ
	std::map<SRC, Resource*> resourcesMap_;

	// “Ç‚İ‚İÏ‚İƒŠƒ\[ƒX
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// ƒfƒtƒHƒ‹ƒgƒRƒ“ƒXƒgƒ‰ƒNƒ^‚ğprivate‚É‚µ‚ÄA
	// ŠO•”‚©‚ç¶¬‚Å‚«‚È‚¢—l‚É‚·‚é
	ResourceManager();
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager() = default;

	// “à•”ƒ[ƒh
	Resource& _Load(SRC src);

};
