#pragma once
#include "ColliderType.h"
#include <DxLib.h>

struct BoxCollider 
{
    VECTOR center_;
    VECTOR size_;
};

struct CapsuleCollider 
{
    VECTOR start_;
    VECTOR end_;
    float radius_;
};

struct ColliderData 
{
    ColliderType type_;
    union {
        BoxCollider box_;
        CapsuleCollider capsule_;
    };

    int ownerID_;
};