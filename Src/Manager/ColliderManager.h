#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "../Collider/ColliderData.h"
#include "../Collider/ColliderType.h"

class ColliderManager 
{
public:

    void Register(ColliderType type, const ColliderData& data);
    void Clear();
    const std::vector<ColliderData>& GetColliders(ColliderType type) const;
    void CheckAllCollisions(ColliderType typeA, ColliderType typeB);

private:
    std::unordered_map<ColliderType, std::vector<ColliderData>> colliderMap_;
};
