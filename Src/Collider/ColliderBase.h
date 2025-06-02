#pragma once

#include "ColliderData.h"

class ColliderBase 
{

public:

    // ƒˆ‰¼‘zŠÖ”ƒNƒ‰ƒX‚Æ‚µ‚Äˆµ‚¤
    virtual ~ColliderBase() = default;
    virtual void Update() = 0;
    virtual void Draw() const = 0;

    // Colliderî•ñ‚Ìæ“¾
    virtual const ColliderData& GetColliderData() const = 0;
};
