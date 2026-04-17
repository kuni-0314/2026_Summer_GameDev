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
	static constexpr int PLAYER_FIELD_CELL_SIZE = 50;											// プレイヤーフィールドのセルサイズ
	static constexpr int ENEMY_FIELD_CELL_Y = 4;												// エネミーフィールドのセル高さ
	static constexpr int ENEMY_FIELD_CELL_X = 4;												// エネミーフィールドのセル幅
	static constexpr int ENEMY_FIELD_CELL_TOTAL = ENEMY_FIELD_CELL_Y * ENEMY_FIELD_CELL_X;		// エネミーフィールドのセル総数
	static constexpr int ENEMY_FIELD_CELL_SIZE = 50;											// エネミーフィールドのセルサイズ
	static constexpr int SELECT_FIELD_CELL_Y = 7;												// セレクトフィールドのセル高さ
	static constexpr int SELECT_FIELD_CELL_X = 48;												// セレクトフィールドのセル幅
	static constexpr int SELECT_FIELD_CELL_TOTAL = SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_X;	// セレクトフィールドのセル総数
	static constexpr int SELECT_FIELD_CELL_SIZE = 40;											// セレクトフィールドのセルサイズ
	static constexpr int MAX_ACTION_COUNT = PLAYER_FIELD_CELL_TOTAL + ENEMY_FIELD_CELL_TOTAL;	// 最大行動回数
	
	int currentActionCount_;	// 現在の行動回数

	GameScene* gameScene_;
};

