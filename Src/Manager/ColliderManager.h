#pragma once
#include <memory>
#include <vector>
#include "../Collider/ColliderData.h"
#include "../Object/Unit/ActorBase.h"

class ActorBase;

class ColliderManager 
{
public:

    // アクターを登録
    void RegisterActor(const std::shared_ptr<ActorBase>& actor);
    void DrawColliders();
    void AddCollider(const ColliderData& collider);
    void Update(); // 毎フレーム、位置を更新しつつ当たり判定処理

private:

    // 複数アクターを管理
    std::vector<std::weak_ptr<ActorBase>> actors_; 

	// コライダーのリスト
    std::vector<ColliderData> colliders_;
   

    void UpdateColliders(); // 移動体の位置更新（GetPos呼び出し）
    void CheckCollisions();
    void ResolveStageCollision(ColliderData& mover, const ColliderData& stage);
    void ResolveCapsuleCollision(ColliderData& a, ColliderData& b);
    void HandleWeaponHit(const ColliderData& weapon, const ColliderData& target);
};