#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "SceneBase.h"

SceneBase::SceneBase(void) :
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	camera_(SceneManager::GetInstance().GetCamera())
	/*collision_(SceneManager::GetInstance().GetCollision()),
	colMng_(SceneManager::GetInstance().GetColManager())*/
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}
