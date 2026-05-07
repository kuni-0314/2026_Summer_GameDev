#pragma once

class SkillBase
{
public:
	// ó‘Ô
	enum class STATE
	{
		NONE = 0,
		ATTACK,
		DEFENSE,
		BUFF,
		DEBUFF,
	};

	// í—Ş
	enum class TYPE
	{
		NONE = 0,
		PHYSICAL,	// •¨—
		MAGIC,		// –‚–@
	};

	// ‘®«
	enum class ATTRIBUTE
	{
		NONE = 0,	// –³
		FIRE,       // ‰Î
		WATER,      // …
		THUNDER,    // “d‹C
		WIND,       // •—
		ICE,        // •X
		LIGHT,      // Œõ
		DARK,       // ˆÅ
		POISON,     // “Å
	};

	SkillBase(void);
	~SkillBase(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	//ATTRIBITE attribite_;	// ‘®«

	// ó‘Ô


};

