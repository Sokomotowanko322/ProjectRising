#pragma once
#include <memory>
#include <vector>
#include "../Collider/ColliderData.h"
#include "../Object/Unit/ActorBase.h"

class ActorBase;

class ColliderManager 
{
public:

    // �A�N�^�[��o�^
    void RegisterActor(const std::shared_ptr<ActorBase>& actor);
    void DrawColliders();
    void AddCollider(const ColliderData& collider);
    void Update(); // ���t���[���A�ʒu���X�V�������蔻�菈��

private:

    // �����A�N�^�[���Ǘ�
    std::vector<std::weak_ptr<ActorBase>> actors_; 

	// �R���C�_�[�̃��X�g
    std::vector<ColliderData> colliders_;
   

    void UpdateColliders(); // �ړ��̂̈ʒu�X�V�iGetPos�Ăяo���j
    void CheckCollisions();
    void ResolveStageCollision(ColliderData& mover, const ColliderData& stage);
    void ResolveCapsuleCollision(ColliderData& a, ColliderData& b);
    void HandleWeaponHit(const ColliderData& weapon, const ColliderData& target);
};