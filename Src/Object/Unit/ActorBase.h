#pragma once
#include "../../Object/Transform.h"
#include <DxLib.h>

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

   
	// �擾�n
    const Transform& GetTransform(void) const;
    VECTOR GetPosition() const;
    int GetHP() const;

	// �ݒ�n
    void SetHP(int hp);
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

private:

#pragma region �t���O

    // �����t���O
	bool isAlive_;
  

#pragma endregion 
};