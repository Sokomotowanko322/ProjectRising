#pragma once
#include <DxLib.h>
#include "../../Object/Transform.h"

class SceneManager;
class ResourceManager;
class Player;

class ActorBase
{
public:
	
    ActorBase();
    virtual ~ActorBase();

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    virtual void Damage(int amount);

   
	// �擾�n
    const Transform& GetTransform(void) const;
    virtual VECTOR GetPos() const = 0; // �������z�֐�
    virtual void SetPos(const VECTOR& pos) = 0; // �������z�֐�
    int GetHP() const;

	// �ݒ�n
    int SetHP(int hp);
    bool IsAlive() const;


protected:

    // �Q��
    Transform transform_;
    ResourceManager& resMng_;
    SceneManager& scnMng_;

	// �v���C���[�̎Q��
    std::weak_ptr<Player> player_;
    
#pragma region �p�����[�^�[

    // �ʒu
    VECTOR position_;

    // �̗�
    int hp_;

#pragma endregion


#pragma region �t���O

    // �����t���O
    bool isAlive_;

    // ���G�t���O
	bool isInvincible_;

#pragma endregion 

private:

};