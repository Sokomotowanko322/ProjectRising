#pragma once
#include <DxLib.h>
#include "../Collider/ColliderType.h"
#include "../Collider/ColliderData.h"

namespace CollisionUtility
{
    // ������̍ŋߐړ_
    VECTOR ClosestPointOnSegment(const VECTOR& a, const VECTOR& b, const VECTOR& point);

    // AABB��̍ŋߐړ_
    VECTOR ClosestPointOnAABB(const VECTOR& point, const BoxCollider& box);

    // �J�v�Z��-�{�b�N�X�Փ˔���
    bool CapsuleToBox(const CapsuleCollider& capsule, const BoxCollider& box);

    // �J�v�Z��-�J�v�Z���Փ˔���
    bool CapsuleToCapsule(const CapsuleCollider& a, const CapsuleCollider& b);

    // ����-�����ԋ���
    float DistanceSegmentToSegment(const VECTOR& p1, const VECTOR& q1, const VECTOR& p2, const VECTOR& q2);

    // �_-AABB�ԋ���
    float DistancePointToAABB(const VECTOR& point, const BoxCollider& box);
}