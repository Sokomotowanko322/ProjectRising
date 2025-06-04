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
    // Actor�̈ʒu�X�V
    for (auto& [actor, capsule] : actorColliders_) {
        capsule = actor->GetCapsuleCollider();
    }
    // Weapon�̈ʒu�X�V
    for (auto& [weapon, capsule] : weaponColliders_) {
        capsule = weapon->GetCapsuleCollider();
    }
}

void ColliderManager::CheckCollisions() {
    // Stage��Actor�iPlayer, Enemy�j�̏Փ˔���E����
    for (auto& [actor, capsule] : actorColliders_) {
        for (const auto& stage : stageColliders_) {
            if (CollisionUtility::CapsuleToBox(capsule, stage)) {
                actor->OnBounce(stage); // ���ˏ���
            }
        }
    }
    // Weapon��Enemy�̏Փ˔���E�_���[�W�E�A�j���[�V����
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