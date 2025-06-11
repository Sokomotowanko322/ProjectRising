#include <DxLib.h>
#include "../Utility/CollisionUtility.h"
#include "../Object/Weapon.h"
#include "ColliderManager.h"

void ColliderManager::RegisterActor(const std::shared_ptr<ActorBase>& actor)
{
    actors_.push_back(actor);
}

void ColliderManager::DrawColliders()
{
    int idx = 0;
    for (const auto& col : colliders_)
    {
        printfDx("Collider[%d]: type=%d, ownerID=%d, pos=(%.2f, %.2f, %.2f)\n",
            idx, (int)col.type_, col.ownerID_, col.pos_.x, col.pos_.y, col.pos_.z);
        ++idx;

        if (col.type_ == ColliderType::Capsule)
        {
            // �J�v�Z���̒��Scol.pos_�A����col.dir_�A����col.height_�A���acol.radius_�̏ꍇ
            VECTOR start = VAdd(col.pos_, VScale(col.dir_, -col.length_ * 0.5f));
            VECTOR end = VAdd(col.pos_, VScale(col.dir_, col.length_ * 0.5f));
            DrawCapsule3D(
                start,
                end,
                col.radius_,
                16,                        // ������
                GetColor(255, 0, 0),         // ���ʂ̐F
                GetColor(255, 0, 0),         // ���[�̐F
                FALSE                      // ���C���[�t���[��
            );
        }
        //else if (col.type_ == ColliderType::StageTransform)
        //{
        //    // �{�b�N�X�iAABB�j�`���
        //    VECTOR min = VSub(col.pos_, VScale(col.dir_, 0.5f));
        //    VECTOR max = VAdd(col.pos_, VScale(col.dir_, 0.5f));
        //    DrawBox(max.x, max.y, max.z,GetColor(0, 255, 0),true,1);
        //}
        //// �K�v�ɉ����đ��̃^�C�v���ǉ�
    }
}

void ColliderManager::AddCollider(const ColliderData& collider)
{
    colliders_.push_back(collider);
}

void ColliderManager::Update() 
{
    for (auto i = actors_.begin(); i != actors_.end(); )
    {
        if (auto actor = i->lock())
        {
            UpdateColliders();
            CheckCollisions();

            // --- �����ŃR���C�_�[�̈ʒu���A�N�^�[�{�̂ɔ��f ---
            for (auto& col : colliders_)
            {
                if (col.type_ == ColliderType::Capsule && col.ownerID_ == actor->GetTransform().modelId)
                {
                    actor->SetPos(col.pos_);
                }
            }
            // ---------------------------------------------------

            ++i;
        }
        else
        {
            i = actors_.erase(i);
        }
    }
}

void ColliderManager::UpdateColliders()
{
    for (auto& weakActor : actors_)
    {
        auto actor = weakActor.lock();
        if (!actor) continue;

        // Weapon�^�Ə����𕪂��Ă���
        if (auto weapon = dynamic_cast<Weapon*>(actor.get())) {
            const Transform& trans = weapon->GetWeaponTransform();
            VECTOR scl = trans.scl;
            VECTOR pos = trans.pos;
            Quaternion rot = trans.quaRot;

            VECTOR tipWorld = VAdd(pos, Quaternion::PosAxis(rot, { 1200.0f * scl.x, 0.0f * scl.y, 0.0f * scl.z }));
            tipWorld.x += 30.0f; // ����̐�[�����100�P�ʈړ�
            tipWorld.y += 16.0f; // ����̐�[�����100�P�ʈړ�

            VECTOR baseWorld = VAdd(pos, Quaternion::PosAxis(rot, { 0.0f * scl.x, 0 * scl.y, 0.0f * scl.z }));

            VECTOR center = VScale(VAdd(tipWorld, baseWorld), 0.5f);
            VECTOR dir = VNorm(VSub(tipWorld, baseWorld));
            float height = VSize(VSub(tipWorld, baseWorld));

            for (auto& col : colliders_) {
                if (col.ownerID_ == trans.modelId) {
                    col.pos_ = center;
                    col.dir_ = dir;
                    col.length_ = height;
                }
            }
        }
        // �ʏ�A�N�^�[
        int actorId = actor->GetTransform().modelId;
        for (auto& col : colliders_)
        {
            if (col.type_ != ColliderType::Capsule) continue;
            if (col.ownerID_ != actorId) continue;
            col.pos_ = actor->GetPos();
        }
    }
}

void ColliderManager::CheckCollisions() {
    for (size_t i = 0; i < colliders_.size(); ++i) {
        for (size_t j = i + 1; j < colliders_.size(); ++j) {
            ColliderData& a = colliders_[i];
            ColliderData& b = colliders_[j];

            if (a.type_ == ColliderType::Capsule && b.type_ == ColliderType::Capsule) 
            {
                if (!a.isTrigger_ && !b.isTrigger_)
                {
                    ResolveCapsuleCollision(a, b); // �ǉ��F���@�ƓG�Ȃ�
                }
                else if (a.isTrigger_ && !b.isTrigger_) {
                    
                }
                else if (!a.isTrigger_ && b.isTrigger_) {
                    
                }
            }

            if (a.type_ == ColliderType::Capsule && b.type_ == ColliderType::StageTransform) {
                ResolveStageCollision(a, b);
            }
            else if (b.type_ == ColliderType::Capsule && a.type_ == ColliderType::StageTransform) {
                ResolveStageCollision(b, a);
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

void ColliderManager::ResolveCapsuleCollision(ColliderData& a, ColliderData& b)
{
    // 2�̃J�v�Z���̒��S���W�̍����x�N�g���i�@���j
    VECTOR dir = VSub(a.pos_, b.pos_);
    float dist = VSize(dir);

    // ���a�̍��v
    float rSum = a.radius_ + b.radius_;

    // �߂荞�ݗʂ��v�Z
    float penetration = rSum - dist;

    if (penetration > 0.0f && dist > 1e-4f) {
        // �@���𐳋K��
        VECTOR normal = VNorm(dir);

        // ���������]�T���������ĉ����o��
        float pushBack = penetration + 0.05f;

        // ���҂��ϓ��ɉ����o��
        a.pos_ = VAdd(a.pos_, VScale(normal, pushBack * 20.0f));
        b.pos_ = VAdd(b.pos_, VScale(normal, -pushBack * 20.0f));
    }
}

void ColliderManager::HitAttackToDamage(const ColliderData& self, const ColliderData& target) 
{
    // ownerID����A�N�^�[�����
    ActorBase* attacker = nullptr;
    ActorBase* victim = nullptr;
    for (auto& weakActor : actors_)
    {
        if (auto actor = weakActor.lock()) 
        {
            if (actor->GetTransform().modelId == self.ownerID_) 
            {
                attacker = actor.get();
            }
            if (actor->GetTransform().modelId == target.ownerID_) 
            {
                victim = actor.get();
            }
        }
    }
    if (!attacker || !victim) 
    {
        return;
    }
}