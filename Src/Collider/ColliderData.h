#pragma once
#include "DxLib.h"
#include "ColliderType.h"

struct ColliderData 
{
    ColliderType type_;
    VECTOR pos_;      // 始点 or 中心
    VECTOR dir_;      // 単位ベクトル
    float height_;    // カプセル長さ
    float radius_;    // 半径
    int ownerID_;     // 所属アクターID
    bool isTrigger_;  // トリガーかどうか
};