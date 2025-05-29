#pragma once
#include <vector>
#include <memory>
#include "../Collider/Collider.h"

class ColliderManager 
{
public:

    // �R���C�_��t����
    void AddCollider(std::shared_ptr<Collider> collider);

	// �R���C�_���O��
    void RemoveCollider(std::shared_ptr<Collider> collider);

    // �Փ˔���Ȃǂ̍X�V
    void Update(); 

private:

	// �R���C�_�̃��X�g
    std::vector<std::shared_ptr<Collider>> colliders_;
};