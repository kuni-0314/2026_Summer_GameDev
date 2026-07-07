#pragma once
#include <unordered_map>
#include <any>
#include <string>

class DebugStringDrawer
{
public:
	// コピー禁止
	DebugStringDrawer(const DebugStringDrawer&) = delete;
	DebugStringDrawer& operator=(const DebugStringDrawer&) = delete;
	// ムーブ禁止
	DebugStringDrawer(DebugStringDrawer&&) = delete;
	DebugStringDrawer& operator=(DebugStringDrawer&&) = delete;

	static DebugStringDrawer& GetInstance();	// インスタンスの取得
	static void DeleteInstance();				// インスタンスの削除

	void Draw() const;	// 描画

	void Add(std::string key, std::any	value);	// デバッグ文字列の追加
	void Remove(std::string key);				// デバッグ文字列の削除

private:
	DebugStringDrawer() = default;
	~DebugStringDrawer() = default;

	std::unordered_map<std::string, std::any> debugStrings_;	// デバッグ文字列のマップ

	const unsigned int TEXT_COLOR = 0xFFFFFF;	// 文字色
	const unsigned int BG_COLOR = 0x000000;		// 背景色

	static DebugStringDrawer* instance_;
};

