#pragma once
#include <vector>
#include <memory>
#include "../Collider/Collider.h"

class ColliderManager 
{
public:

    // コライダを付ける
    void AddCollider(std::shared_ptr<Collider> collider);

	// コライダを外す
    void RemoveCollider(std::shared_ptr<Collider> collider);

    // 衝突判定などの更新
    void Update(); 

private:

	// コライダのリスト
    std::vector<std::shared_ptr<Collider>> colliders_;
};