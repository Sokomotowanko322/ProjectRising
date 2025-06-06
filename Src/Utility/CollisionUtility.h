#pragma once
#include <DxLib.h>

class CollisionUtility
{
public:
    // カプセル同士の線分距離を計算
    static float GetSegmentSegmentDistance(
        const VECTOR& a0, const VECTOR& a1,
        const VECTOR& b0, const VECTOR& b1
    );

    // 2点間の距離
    static float GetDistance(const VECTOR& a, const VECTOR& b);
};