#pragma once
#include "../../Common/Vector2.h"

class CellBase
{
public:
	enum class CELL_TYPE
	{
		NONE = 0,
		ATTACK,
		DEFENSE,
		BUFF,
		DEBUFF,
	};

	CellBase(CELL_TYPE type, Vector2 pos, int size);
	CellBase(CELL_TYPE type, Vector2 pos, int size, int actionCount);
	virtual ~CellBase(void);

	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	const Vector2& GetPos(void) const { return pos_; }
	void SetPos(const Vector2& pos) { pos_ = pos; }

	const CELL_TYPE GetType(void) const { return type_; }

	void SetActive(bool isActive) { isActive_ = isActive; }
	bool IsActive(void) const { return isActive_; }

	void SetActionCount(int actionCount) { actionCount_ = actionCount; }
	int GetActionCount(void) const { return actionCount_; }

	static constexpr unsigned int CELL_NONE_COLOR = 0xFFFFFF;
	static constexpr unsigned int CELL_ATTACK_COLOR = 0xFF0000;
	static constexpr unsigned int CELL_DEFENSE_COLOR = 0x0000FF;
	static constexpr unsigned int CELL_BUFF_COLOR = 0x00FF00;
	static constexpr unsigned int CELL_DEBUFF_COLOR = 0xFFFF00;

protected:
	CELL_TYPE type_;
	Vector2 pos_;
	int size_;
	int actionCount_;
	unsigned int color_;
	bool isActive_;
};

