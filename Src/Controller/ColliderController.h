#include "Collider.h"

class SceneManager;
class ColliderManager;
class Collision;

class ColliderController
{

public:

	ColliderController(void);
	~ColliderController(void);

	/// <summary>
	/// �Փ˔���̓o�^
	/// </summary>
	/// <param name="obj">���g�̃I�u�W�F�N�g��</param>
	void SetCollision(OBJECT_TYPE obj);
	void SetCollisionSub(ENEMY_SUBTYPE subobj);

	//��������̃t���O���X�V
	void SetCollisionAble(OBJECT_TYPE obj, bool val);



	/// <summary>
	/// �o�^���ꂽ�I�u�W�F�N�g�ƏՓ˂��Ă��邩
	/// </summary>
	/// <param name="obj">�Փ˔�����s���I�u�W�F�N�g</param>
	/// <returns></returns>
	const bool OnCollision(OBJECT_TYPE obj);
	const std::vector<int>& TestOnCollision(OBJECT_TYPE obj);
	void DebugLog(const std::string& msg);
	/// <summary>
	/// �R���W����
	/// </summary>
	/// <param name="obj">�Փ˔�����s���I�u�W�F�N�g</param>
	/// <returns>�Փ˂��Ă���ꍇ : MV1_COLL_RESULT_POLY</returns>
	const MV1_COLL_RESULT_POLY_DIM OnCollisionResultPoly(OBJECT_TYPE obj);

	const MV1_COLL_RESULT_POLY OnCollisionResultPoly_Line(OBJECT_TYPE obj);


	const VECTOR OnCollisionResultHitDir(OBJECT_TYPE obj);

	//�|���S���Ƃ̔�������
	const int OnCollisionResultPolyTriangle(const MV1_COLL_RESULT_POLY& poly);

	void CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& poly);


private:

	ColliderManager& colMng_;

	//�o�^����Փ˔���
	OBJECT_TYPE objectType_;

	//�Ǐ]�t���[��
	ENEMY_SUBTYPE subType_;

	//�Փ˔�����s���N���X
	std::unique_ptr<Collision> collision_;

	const bool IsHit(std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB);

	const bool IsModel(std::weak_ptr<Collider> col) const;
	const bool IsSphere(std::weak_ptr<Collider> col) const;
	const bool IsCapsule(std::weak_ptr<Collider> col) const;
	const bool IsLine(std::weak_ptr<Collider> col) const;

};

