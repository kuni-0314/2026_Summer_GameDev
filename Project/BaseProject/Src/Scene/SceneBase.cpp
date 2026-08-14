#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "SceneBase.h"

SceneBase::SceneBase() 
	: 
	resMng_(ResourceManager::GetInstance()),
	sceMng_(SceneManager::GetInstance())
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
}

void SceneBase::Update()
{
}

void SceneBase::Draw()
{
}
