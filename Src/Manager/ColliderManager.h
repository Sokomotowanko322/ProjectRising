#pragma once
#include <vector>
#include <memory>
#include "../Collider/ColliderBase.h"

class ColliderManager 
{

public:
    
    // �R���C�_�̐ݒ�
    void AddCollider(const std::shared_ptr<ColliderBase>& collider);
    void Update();
    void Draw() const;
    void Clear();

private:
    
    std::vector<std::shared_ptr<ColliderBase>> colliders;
};