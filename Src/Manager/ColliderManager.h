#pragma once
#include <vector>
#include <memory>
#include "../Collider/ColliderType.h"
#include "../Collider/ColliderData.h"
#include "../Object/Unit/ActorBase.h"
#include "../Object/Weapon.h"

class ColliderManager 
{
public:
    void RegisterStage(const BoxCollider& box);
    void RegisterActor(ActorBase* actor, ColliderType type); // Player, Enemy
    void RegisterWeapon(Weapon* weapon);

    void Update(); // 各コライダの位置を更新
    void CheckCollisions();

private:
    std::vector<BoxCollider> stageColliders_;
    std::vector<std::pair<ActorBase*, CapsuleCollider>> actorColliders_; // Player, Enemy
    std::vector<std::pair<Weapon*, CapsuleCollider>> weaponColliders_;
};