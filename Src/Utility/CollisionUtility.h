#pragma once
#include <DxLib.h>

class CollisionUtility
{
public:
    // �J�v�Z�����m�̐����������v�Z
    static float GetSegmentSegmentDistance(
        const VECTOR& a0, const VECTOR& a1,
        const VECTOR& b0, const VECTOR& b1
    );

    // 2�_�Ԃ̋���
    static float GetDistance(const VECTOR& a, const VECTOR& b);
};