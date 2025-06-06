#pragma once
#include <map>
#include <functional>
#include "../Object/Transform.h"
#include "../Object/Unit/ActorBase.h"

class SkyDome : public ActorBase
{

public:

	static constexpr float MODEL_SCALE = 100.0f;
	static constexpr VECTOR SCALES = { MODEL_SCALE, MODEL_SCALE, MODEL_SCALE };

	// ���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// �R���X�g���N�^
	SkyDome(const Transform& syncTransform);

	// �f�X�g���N�^
	~SkyDome(void);

	void Init(void);
	void Update(void) override;
	void Draw(void) override;
	void SetPos(const VECTOR& pos)override;
	VECTOR GetPos(void)const override;

private:

	// ���@�̏��
	const Transform& syncTransform_;

	// ���
	STATE state_;
	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// ��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateStay(void);
	void ChangeStateFollow(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateStay(void);
	void UpdateFollow(void);

};
