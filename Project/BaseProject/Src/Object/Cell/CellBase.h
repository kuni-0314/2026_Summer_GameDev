#pragma once
#include <memory>
#include "../../Common/Vector2.h"

class Field;

class CellBase
{

public:

	// セル色
	static constexpr unsigned int CELL_NONE_COLOR = 0x888888;
	static constexpr unsigned int CELL_ATTACK_COLOR = 0xff0000;
	static constexpr unsigned int CELL_DEFENSE_COLOR = 0x0000ff;
	static constexpr unsigned int CELL_BUFF_COLOR = 0x00ff00;
	static constexpr unsigned int CELL_DEBUFF_COLOR = 0xff00ff;

	// セルタイプ
	enum class CELL_TYPE
	{
		NONE = 0,
		ATTACK,
		DEFENSE,
		BUFF,
		DEBUFF,
	};

	CellBase(Field* field, CELL_TYPE type, Vector2 pos, int size);
	CellBase(Field* field, CELL_TYPE type, Vector2 pos, int size, int actionCount);
	virtual ~CellBase();

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Release();

	// セルタイプを取得
	CELL_TYPE GetType() const { return type_; }

	// セルタイプを設定
	void SetType(CELL_TYPE type);

	// 行動回数を取得
	int GetActionCount() const { return actionCount_; }

	// 行動回数を設定
	void SetActionCount(int actionCount) { actionCount_ = actionCount; }

	// アクティブ状態を取得
	bool IsActive() const { return isActive_; }

	// アクティブ状態を設定
	void SetActive(bool isActive) { isActive_ = isActive; }

	// マウスカーソルがセル内にあるかチェック
	bool IsMouseOver() const;

protected:

	Field* field_;
	CELL_TYPE type_;
	Vector2 pos_;
	int size_;
	int actionCount_;
	unsigned int color_;
	bool isActive_;

};

