#include <DxLib.h>
#include <algorithm>
#include <cmath>
#include "CollisionUtility.h"

VECTOR CollisionUtility::ClosestPointOnSegment(const VECTOR& a, const VECTOR& b, const VECTOR& point)
{
    VECTOR ab = VSub(b, a);
    float t = VDot(VSub(point, a), ab) / VDot(ab, ab);
    t = std::clamp(t, 0.0f, 1.0f);
    return VAdd(a, VScale(ab, t));
}

VECTOR CollisionUtility::ClosestPointOnAABB(const VECTOR& point, const BoxCollider& box)
{
    VECTOR result;
    result.x = (std::max)(box.center_.x - box.size_.x * 0.5f, (std::min)(point.x, box.center_.x + box.size_.x * 0.5f));
    result.y = (std::max)(box.center_.y - box.size_.y * 0.5f, (std::min)(point.y, box.center_.y + box.size_.y * 0.5f));
    result.z = (std::max)(box.center_.z - box.size_.z * 0.5f, (std::min)(point.z, box.center_.z + box.size_.z * 0.5f));
    return result;
}

bool CollisionUtility::CapsuleToBox(const CapsuleCollider& capsule, const BoxCollider& box)
{
    // カプセル線分の最近接点とAABBの距離で判定
    VECTOR closest = ClosestPointOnSegment(capsule.start_, capsule.end_, box.center_);
    float distance = DistancePointToAABB(closest, box);
    return distance <= capsule.radius_;
}

bool CollisionUtility::CapsuleToCapsule(const CapsuleCollider& a, const CapsuleCollider& b)
{
    float dist = DistanceSegmentToSegment(a.start_, a.end_, b.start_, b.end_);
    return dist <= (a.radius_ + b.radius_);
}

float CollisionUtility::DistanceSegmentToSegment(const VECTOR& p1, const VECTOR& q1, const VECTOR& p2, const VECTOR& q2)
{
    VECTOR d1 = VSub(q1, p1);
    VECTOR d2 = VSub(q2, p2);
    VECTOR r = VSub(p1, p2);
    float a = VDot(d1, d1);
    float e = VDot(d2, d2);
    float f = VDot(d2, r);

    float s, t;
    if (a <= 0.00001f && e <= 0.00001f)
    {
        return VSize(VSub(p1, p2));
    }
    if (a <= 0.00001f)
    {
        s = 0.0f;
        t = std::clamp(f / e, 0.0f, 1.0f);
    }
    else
    {
        float c = VDot(d1, r);
        if (e <= 0.00001f)
        {
            t = 0.0f;
            s = std::clamp(-c / a, 0.0f, 1.0f);
        }
        else
        {
            float b = VDot(d1, d2);
            float denom = a * e - b * b;
            if (denom != 0.0f)
            {
                s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
            }
            else
            {
                s = 0.0f;
            }
            t = (b * s + f) / e;
            if (t < 0.0f)
            {
                t = 0.0f;
                s = std::clamp(-c / a, 0.0f, 1.0f);
            }
            else if (t > 1.0f)
            {
                t = 1.0f;
                s = std::clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }

    VECTOR cp1 = VAdd(p1, VScale(d1, s));
    VECTOR cp2 = VAdd(p2, VScale(d2, t));
    return VSize(VSub(cp1, cp2));
}

float CollisionUtility::DistancePointToAABB(const VECTOR& point, const BoxCollider& box)
{
    VECTOR min = VSub(box.center_, VScale(box.size_, 0.5f));
    VECTOR max = VAdd(box.center_, VScale(box.size_, 0.5f));

    float dx = (std::max)({ min.x - point.x, 0.0f, point.x - max.x });
    float dy = (std::max)({ min.y - point.y, 0.0f, point.y - max.y });
    float dz = (std::max)({ min.z - point.z, 0.0f, point.z - max.z });

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}