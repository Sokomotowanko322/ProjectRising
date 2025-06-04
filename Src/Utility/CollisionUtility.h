#pragma once
#include <DxLib.h>
#include "../Collider/ColliderType.h"
#include "../Collider/ColliderData.h"

namespace CollisionUtility
{
    // 線分上の最近接点
    VECTOR ClosestPointOnSegment(const VECTOR& a, const VECTOR& b, const VECTOR& point);

    // AABB上の最近接点
    VECTOR ClosestPointOnAABB(const VECTOR& point, const BoxCollider& box);

    // カプセル-ボックス衝突判定
    bool CapsuleToBox(const CapsuleCollider& capsule, const BoxCollider& box);

    // カプセル-カプセル衝突判定
    bool CapsuleToCapsule(const CapsuleCollider& a, const CapsuleCollider& b);

    // 線分-線分間距離
    float DistanceSegmentToSegment(const VECTOR& p1, const VECTOR& q1, const VECTOR& p2, const VECTOR& q2);

    // 点-AABB間距離
    float DistancePointToAABB(const VECTOR& point, const BoxCollider& box);
}