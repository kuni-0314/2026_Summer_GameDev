#pragma once
#include <memory>
class Field;
class FieldPlayer;
class FieldEnemy;
class FieldSelect;
class GameScene;

class FieldManager
{
public:

	FieldManager(GameScene* gameScene);
	~FieldManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	int GetMaxFieldCell(void) const;		// 最大フィールドセル数を取得
	int GetCurrentActionCount(void) const;	// 現在の行動回数を取得
	int GetTotalActionCount(void) const;	// 総行動回数を取得
	
private:
	std::unique_ptr<Field> playerField_;
	std::unique_ptr<Field> enemyField_;
	std::unique_ptr<Field> selectField_;

	static constexpr int PLAYER_FIELD_CELL_Y = 5;												// プレイヤーフィールドのセル高さ
	static constexpr int PLAYER_FIELD_CELL_X = 5;												// プレイヤーフィールドのセル幅
	static constexpr int PLAYER_FIELD_CELL_TOTAL = PLAYER_FIELD_CELL_Y * PLAYER_FIELD_CELL_X;	// プレイヤーフィールドのセル総数
	static constexpr int PLAYER_FIELD_CELL_SIZE = 36;											// プレイヤーフィールドのセルサイズ
	static constexpr int ENEMY_FIELD_CELL_Y = 4;												// エネミーフィールドのセル高さ
	static constexpr int ENEMY_FIELD_CELL_X = 4;												// エネミーフィールドのセル幅
	static constexpr int ENEMY_FIELD_CELL_TOTAL = ENEMY_FIELD_CELL_Y * ENEMY_FIELD_CELL_X;		// エネミーフィールドのセル総数
	static constexpr int ENEMY_FIELD_CELL_SIZE = 36;											// エネミーフィールドのセルサイズ
	static constexpr int SELECT_FIELD_CELL_Y = 7;												// セレクトフィールドのセル高さ
	static constexpr int SELECT_FIELD_CELL_X = 48;												// セレクトフィールドのセル幅
	static constexpr int SELECT_FIELD_CELL_TOTAL = SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_X;	// セレクトフィールドのセル総数
	static constexpr int SELECT_FIELD_CELL_SIZE = 36;											// セレクトフィールドのセルサイズ
	static constexpr int MAX_ACTION_COUNT = PLAYER_FIELD_CELL_TOTAL + ENEMY_FIELD_CELL_TOTAL;	// 最大行動回数
	
	int currentActionCount_;	// 現在の行動回数

	int imageCellRed_;

	//tmp
	enum class SKILL_LIST
	{
		SLASH = 0,
		DEFENCE,
		HEEL,
		PROTECT,
		FRAME,
		MAX
	};
	static constexpr int BASE_CELL_SIZE = 36;
	
	static constexpr int GENERATE_SKILL_NUM = 4;
	static constexpr int SKILL_MAX_SIZE = 4;
	SKILL_LIST generatedSkills_[GENERATE_SKILL_NUM];
	bool slashPiece[SKILL_MAX_SIZE][SKILL_MAX_SIZE] = {
		{ false, false, false, false },
		{ false, true, true, false },
		{ false, true, true, false },
		{ false, false, false, false },
	};
	bool defencePiece[SKILL_MAX_SIZE][SKILL_MAX_SIZE] = {
		{ false, false, false, false },
		{ true, true, false, false },
		{ false, true, true, false },
		{ false, false, false, false },
	};
	bool heelPiece[SKILL_MAX_SIZE][SKILL_MAX_SIZE] = {
		{ false, true, false, false },
		{ false, true, false, false },
		{ false, true, true, false },
		{ false, false, false, false },
	};
	bool protectPiece[SKILL_MAX_SIZE][SKILL_MAX_SIZE] = {
		{ false, true, false, false },
		{ false, true, false, false },
		{ false, true, false, false },
		{ false, true, false, false },
	};
	bool framePiece[SKILL_MAX_SIZE][SKILL_MAX_SIZE] = {
		{ true, false, false, false },
		{ true, true, true, false },
		{ false, false, false, false },
		{ false, false, false, false },
	};

	GameScene* gameScene_;
};

