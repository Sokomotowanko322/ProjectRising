#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "ActorBase.h"

ActorBase::ActorBase():
	scnMng_(SceneManager::GetInstance()),
    resMng_(ResourceManager::GetInstance()),
    position_(transform_.pos), 
    hp_(100), 
    transform_(-1),isAlive_(true)
{
}

ActorBase::~ActorBase() 
{
	// ���f���n���h�����L���ȏꍇ�͍폜
    if (transform_.modelId != -1) 
    {
		// ���f���n���h�����폜
        MV1DeleteModel(transform_.modelId);
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
