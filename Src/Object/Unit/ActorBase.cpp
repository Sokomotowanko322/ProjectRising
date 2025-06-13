#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "ActorBase.h"

ActorBase::ActorBase():
	scnMng_(SceneManager::GetInstance()),
    resMng_(ResourceManager::GetInstance()),
    position_(transform_.pos), 
	hp_(100), isInvincible_(false),
    transform_(-1),isAlive_(true)
{
}

ActorBase::~ActorBase() 
{
	// モデルハンドルが有効な場合は削除
    if (transform_.modelId != -1) 
    {
		// モデルハンドルを削除
        MV1DeleteModel(transform_.modelId);
    }
}

void ActorBase::Damage(int damageAmount)
{
    if (isInvincible_)
    {
        return;
    }
    else
    {
        hp_ -= damageAmount;
    }

    if (hp_ <= 0)
    {
        hp_ = 0;
        isAlive_ = false;
    }
   
}

int ActorBase::GetHP() const 
{
    return hp_;
}

const Transform& ActorBase::GetTransform(void) const
{
    return transform_;
}

int ActorBase::SetHP(int hp)
{
    return hp_ = hp;
}

bool ActorBase::IsAlive() const
{
    return hp_ <= 0;
}
