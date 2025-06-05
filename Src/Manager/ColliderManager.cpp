#include <DxLib.h>
#include "../Object/Unit/ActorBase.h"
#include "ColliderManager.h"

void ColliderManager::RegisterActor(const std::shared_ptr<ActorBase>& actor)
{
    actors_.push_back(actor);
}

void ColliderManager::AddCollider(const ColliderData& collider)
{
    colliders_.push_back(collider);
}

void ColliderManager::Update() 
{
    // �A�N�^�[���ƂɃR���C�_�[�����X�V
    for (auto i = actors_.begin(); i != actors_.end(); ) 
    {
        if (auto actor = i->lock()) 
        {
            // ������actor�̏����g����colliders_���X�V
            // ��: col.pos_ = actor->GetPos();
            UpdateColliders();
            CheckCollisions();
            ++i;
        }
        else 
        {
            // ����ς݃A�N�^�[�̓��X�g����폜
            i = actors_.erase(i);
        }
    }

  
}

void ColliderManager::UpdateColliders()
{
    for (auto& col : colliders_) 
    {
        if (col.type_ == ColliderType::Capsule) 
        {
            col.pos_ = allActor_.lock()->GetPos();
        }
    }
}

void ColliderManager::CheckCollisions() {
    for (size_t i = 0; i < colliders_.size(); ++i) {
        for (size_t j = i + 1; j < colliders_.size(); ++j) {
            const ColliderData& a = colliders_[i];
            const ColliderData& b = colliders_[j];

            // Stage����
            if (a.type_ == ColliderType::Capsule && b.type_ == ColliderType::StageTransform) {
                ResolveStageCollision(colliders_[i], b);
            }
            else if (b.type_ == ColliderType::Capsule && a.type_ == ColliderType::StageTransform) {
                ResolveStageCollision(colliders_[j], a);
            }

            // Weapon�q�b�g
            if (a.isTrigger_ && !b.isTrigger_) {
                HandleWeaponHit(a, b);
            }
            else if (!a.isTrigger_ && b.isTrigger_) {
                HandleWeaponHit(b, a);
            }
        }
    }
}

void ColliderManager::ResolveStageCollision(ColliderData& mover, const ColliderData& stage)
{
    // �X�e�[�WAABB�̒��S�E�T�C�Y�ipos_�����S�Adir_���T�C�Y�Ɖ���j
    VECTOR boxCenter = stage.pos_;
    VECTOR boxSize = stage.dir_;

    // �J�v�Z���̒��S
    VECTOR capsuleCenter = mover.pos_;

    // �ŋߐړ_��AABB��ɋ��߂�
    VECTOR closest;
    closest.x = (std::max)(boxCenter.x - boxSize.x * 0.5f, (std::min)(capsuleCenter.x, boxCenter.x + boxSize.x * 0.5f));
    closest.y = (std::max)(boxCenter.y - boxSize.y * 0.5f, (std::min)(capsuleCenter.y, boxCenter.y + boxSize.y * 0.5f));
    closest.z = (std::max)(boxCenter.z - boxSize.z * 0.5f, (std::min)(capsuleCenter.z, boxCenter.z + boxSize.z * 0.5f));

    // �@���x�N�g���iAABB�\�ʁ��J�v�Z�����S�����j
    VECTOR normal = VSub(capsuleCenter, closest);
    float len = VSize(normal);

    if (len < 1e-4f) {
        // �@�������܂�Ȃ��ꍇ�͏����
        normal = VGet(0, 1, 0);
    }
    else {
        normal = VNorm(normal);
    }

    // �����o���ʁi�J�v�Z�����߂荞��ł��镪�{�����]�T�j
    float penetration = mover.radius_ - len;
    if (penetration > 0.0f) {
        mover.pos_ = VAdd(mover.pos_, VScale(normal, penetration + 0.01f));
    }
}

void ColliderManager::HandleWeaponHit(const ColliderData& weapon, const ColliderData& target) 
{
    
}