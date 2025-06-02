#pragma once

#include "ColliderData.h"

class ColliderBase 
{

public:

    // �������z�֐��N���X�Ƃ��Ĉ���
    virtual ~ColliderBase() = default;
    virtual void Update() = 0;
    virtual void Draw() const = 0;

    // Collider���̎擾
    virtual const ColliderData& GetColliderData() const = 0;
};
