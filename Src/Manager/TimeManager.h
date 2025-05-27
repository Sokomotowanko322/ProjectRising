#include <chrono>
#include <string>
#include "../Singleton.h"

/// <summary>
/// ���ԊǗ�
/// </summary>
class TimeManager
	: public Singleton<TimeManager>
{
public:

	/// <summary>
	/// Dxlib_End()�O�̃��\�[�X�j��
	/// </summary>
	void Destroy(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �f���^�^�C���̃��Z�b�g
	/// </summary>
	void ResetDeltaTime(void);

	/// <summary>
	/// ���Ԓ�~��Ԃ̐ؑ�
	/// </summary>
	/// <param name="isStop">
	/// true : ��~��
	/// false : ��~���Ă��Ȃ�
	/// </param>
	void SetStop(bool isStop);

	/// <summary>
	/// ���Ԓ�~��������
	/// </summary>
	/// <returns>
	/// true : ��~��
	/// false : ��~���Ă��Ȃ�
	/// </returns>
	const bool GetIsStop(void);

	/// <summary>
	/// ���o�ߎ��Ԃ̎擾
	/// </summary>
	/// <returns>time_</returns>
	const float GetTime(void);

	/// <summary>
	/// �f���^�^�C���̎擾
	/// </summary>
	/// <returns>deltaTime_</returns>
	const float GetDeltaTime(void);

private:

	friend class Singleton<TimeManager>;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;

	//���o�ߎ���
	float time_;

	//�f���^�^�C��
	float deltaTime_;

	//���Ԓ�~����
	bool isStop_;

	TimeManager(void);
	~TimeManager() = default;
};