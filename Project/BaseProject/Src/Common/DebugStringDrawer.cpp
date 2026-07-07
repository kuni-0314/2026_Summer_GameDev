#include <DxLib.h>
#include "../Application.h"
#include "DebugStringDrawer.h"

DebugStringDrawer* DebugStringDrawer::instance_ = nullptr;

DebugStringDrawer& DebugStringDrawer::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new DebugStringDrawer();
	}
	return *instance_;
}

void DebugStringDrawer::DeleteInstance()
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void DebugStringDrawer::Draw() const
{
	const int SCREEN_SIZE_X = Application::SCREEN_SIZE_X;
	const int SCREEN_SIZE_Y = Application::SCREEN_SIZE_Y;
	const int TEXT_HEIGHT = 16;
	int y = 0;
	for (const auto& [name, ptr] : debugStrings_)
	{
		const std::type_info& t = ptr.type();
		if (t == typeid(bool*))
		{
			int w = GetDrawFormatStringWidth("%s : %d", name.c_str(), *std::any_cast<bool*>(ptr));
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %d", name.c_str(), *std::any_cast<bool*>(ptr));
		}
		else if (t == typeid(unsigned int*))
		{
			int w = GetDrawFormatStringWidth("%s : %u", name.c_str(), *std::any_cast<unsigned int*>(ptr));
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %u", name.c_str(), *std::any_cast<unsigned int*>(ptr));
		}
		else if (t == typeid(int*))
		{
			int w = GetDrawFormatStringWidth("%s : %d", name.c_str(), *std::any_cast<int*>(ptr));
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %d", name.c_str(), *std::any_cast<int*>(ptr));
		}
		else if (t == typeid(float*))
		{
			int w = GetDrawFormatStringWidth("%s : %f", name.c_str(), *std::any_cast<float*>(ptr));
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %f", name.c_str(), *std::any_cast<float*>(ptr));
		}
		else if (t == typeid(double*))
		{
			int w = GetDrawFormatStringWidth("%s : %f", name.c_str(), *std::any_cast<double*>(ptr));
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %f", name.c_str(), *std::any_cast<double*>(ptr));
		}
		else if (t == typeid(VECTOR*))
		{
			VECTOR v = *std::any_cast<VECTOR*>(ptr);
			int w = GetDrawFormatStringWidth("%s : (%f, %f, %f)", name.c_str(), v.x, v.y, v.z);
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : (%f, %f, %f)", name.c_str(), v.x, v.y, v.z);
		}
		else if (t == typeid(std::string*))
		{
			int w = GetDrawFormatStringWidth("%s : %s", name.c_str(), std::any_cast<std::string*>(ptr)->c_str());
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %s", name.c_str(), std::any_cast<std::string*>(ptr)->c_str());
		}
		else if (t == typeid(SIZE_T*))
		{
			unsigned long long value = static_cast<unsigned long long>(*std::any_cast<SIZE_T*>(ptr));
			int w = GetDrawFormatStringWidth("%s : %llu", name.c_str(), value);
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %llu", name.c_str(), value);
		}
		else if (t == typeid(Application::MemorySize*))
		{
			Application::MemorySize value = *std::any_cast<Application::MemorySize*>(ptr);
			int w = GetDrawFormatStringWidth("%s : %llu MB", name.c_str(), value.MB());
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %llu MB", name.c_str(), value.MB());
		}
		else if (t == typeid(BYTE*))
		{
			BYTE value = *std::any_cast<BYTE*>(ptr);
			int w = GetDrawFormatStringWidth("%s : %d", name.c_str(), value);
			DrawBox(SCREEN_SIZE_X - w, y, SCREEN_SIZE_X, y + TEXT_HEIGHT, BG_COLOR, true);
			DrawFormatString(SCREEN_SIZE_X - w, y, TEXT_COLOR, "%s : %d", name.c_str(), value);
		}
		else
		{
#ifdef _DEBUG
			__debugbreak();
#endif // _DEBUG
		}
		y += TEXT_HEIGHT;
	}
}

void DebugStringDrawer::Add(std::string name, std::any value)
{
	debugStrings_[name] = value;
}

void DebugStringDrawer::Remove(std::string name)
{
	debugStrings_.erase(name);
}
