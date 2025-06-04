#include "ColliderManager.h"
#include "../Utility/CollisionUtility.h"

void ColliderManager::RegisterStage(const BoxCollider& box)
{
    stageColliders_.push_back(box);
}

void ColliderManager::RegisterActor(ActorBase* actor, ColliderType type) 
{
    CapsuleCollider capsule = actor->GetCapsuleCollider();
    actorColliders_.emplace_back(actor, capsule);
}

void ColliderManager::RegisterWeapon(Weapon* weapon) {
    CapsuleCollider capsule = weapon->GetCapsuleCollider();
    weaponColliders_.emplace_back(weapon, capsule);
}

void ColliderManager::Update() {
    // Actorの位置更新
    for (auto& [actor, capsule] : actorColliders_) {
        capsule = actor->GetCapsuleCollider();
    }
    // Weaponの位置更新
    for (auto& [weapon, capsule] : weaponColliders_) {
        capsule = weapon->GetCapsuleCollider();
    }
}

void ColliderManager::CheckCollisions() {
    // StageとActor（Player, Enemy）の衝突判定・反射
    for (auto& [actor, capsule] : actorColliders_) {
        for (const auto& stage : stageColliders_) {
            if (CollisionUtility::CapsuleToBox(capsule, stage)) {
                actor->OnBounce(stage); // 反射処理
            }
        }
    }
    // WeaponとEnemyの衝突判定・ダメージ・アニメーション
    for (auto& [weapon, wCapsule] : weaponColliders_) {
        for (auto& [actor, aCapsule] : actorColliders_) {
            if (actor->GetType() == ColliderType::Enemy &&
                CollisionUtility::CapsuleToCapsule(wCapsule, aCapsule)) {
                actor->OnDamage(weapon->GetDamage());
                actor->OnReactToAttack(weapon->GetAttackType());
            }
        }
    }
}