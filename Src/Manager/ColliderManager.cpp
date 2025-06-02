#include "ColliderManager.h"
#include "../Collider/ColliderData.h"
#include <iostream>

void ColliderManager::Register(ColliderType type, const ColliderData& data) 
{
    colliderMap_[type].push_back(data);
}

void ColliderManager::Clear() {
    colliderMap_.clear();
}

const std::vector<ColliderData>& ColliderManager::GetColliders(ColliderType type) const
{
    static const std::vector<ColliderData> empty;
    auto it = colliderMap_.find(type);
    if (it != colliderMap_.end())
    {
        return it->second;
    }
    return empty;
}

void ColliderManager::CheckAllCollisions(ColliderType typeA, ColliderType typeB) 
{
    const auto& collidersA = GetColliders(typeA);
    const auto& collidersB = GetColliders(typeB);

    for (const auto& colA : collidersA)
    {
        for (const auto& colB : collidersB) 
        {
            if (CollisionUtility::CheckCollision(colA, colB))
            {
                std::cout << "Collision detected between type " << static_cast<int>(typeA)
                    << " and type " << static_cast<int>(typeB) << std::endl;
                // 必要に応じてコールバック処理やイベント通知を追加
            }
        }
    }
}