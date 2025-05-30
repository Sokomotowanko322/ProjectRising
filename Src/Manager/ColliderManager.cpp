#include "ColliderManager.h"

void ColliderManager::AddCollider(const std::shared_ptr<ColliderBase>& collider)
{
    if (collider) 
    {
        colliders.push_back(collider);
    }
}

void ColliderManager::Update()
{
    for (auto& collider : colliders) 
    {
        collider->Update();
    }
}

void ColliderManager::Draw() const 
{
    for (const auto& collider : colliders) 
    {
        collider->Draw();
    }
}

void ColliderManager::Clear()
{
    colliders.clear();
}