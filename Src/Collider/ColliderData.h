#pragma once

#include <DxLib.h>
#include "../Common/Vector3.h"

enum class ColliderShape 
{
    Capsule,
    Sphere,
    AABB,
};

struct Capsule 
{
    Vector3 basePos;  // カプセルの下端の中心位置
    float radius;
    float height;
};

struct Sphere 
{
    Vector3 center;
    float radius;
};

struct AABB 
{
    Vector3 min;
    Vector3 max;
};

struct ColliderData 
{
    ColliderShape shape;

    union 
    {
        Capsule capsule;
        Sphere sphere;
        AABB aabb;
    };

    ColliderData();
    ~ColliderData();
};

namespace CollisionUtility
{
    float Clamp(float value, float min, float max);
    float SegmentToSegmentDistance(
        const Vector3& p1, const Vector3& q1,
        const Vector3& p2, const Vector3& q2
    );
    bool CheckCapsuleCapsule(const Capsule& a, const Capsule& b);
    bool CheckCollision(const ColliderData& a, const ColliderData& b);
}