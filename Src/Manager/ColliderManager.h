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

    // �f�o�b�O�`��
    void DrawColliders();

	// �R���C�_�[��ǉ�
    void AddCollider(const ColliderData& collider);
    
    // �܂Ƃ߂čX�V
    void Update();

private:

    // �����A�N�^�[���Ǘ�
    std::vector<std::weak_ptr<ActorBase>> actors_; 

	// �R���C�_�[�̃��X�g
    std::vector<ColliderData> colliders_;
   
    // �R���C�_�̈ʒu�Ȃǂ��X�V����
    void UpdateColliders();
	
    // �����蔻����`�F�b�N
    void CheckCollisions();
    
	// �Փˎ�������
    void ResolveStageCollision(ColliderData& mover, const ColliderData& stage);
    void ResolveCapsuleCollision(ColliderData& a, ColliderData& b);
    void HitAttackToDamage(const ColliderData& self, const ColliderData& target);
};