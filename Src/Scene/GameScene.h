#include <memory>
#include "SceneBase.h"

class ColliderManager;
class SkyDome;
class Player;
class NormalEnemy;

class GameScene : public SceneBase
{
public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	// ������
	void Init(void) override;
	void InitCollider(void);

	// �X�V
	void Update(void) override;

	// �`��
	void Draw(void) override;

protected:

private:
	
	// �v���C���[
	std::shared_ptr<Player> player_;
	
	// �ʏ�̓G
	std::shared_ptr<NormalEnemy> normalEnemy_;

	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;
	
	//// �G
	//std::shared_ptr<Enemy> enemy_;
	//// �X�e�[�W
	//std::shared_ptr<Stage> stage_;
	// �Փ˔���
	std::unique_ptr<ColliderManager> colMng_;

};

