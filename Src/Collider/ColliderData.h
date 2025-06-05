#pragma once
#include "DxLib.h"
#include "ColliderType.h"

struct ColliderData 
{
    ColliderType type_;
    VECTOR pos_;      // �n�_ or ���S
    VECTOR dir_;      // �P�ʃx�N�g��
    float height_;    // �J�v�Z������
    float radius_;    // ���a
    int ownerID_;     // �����A�N�^�[ID
    bool isTrigger_;  // �g���K�[���ǂ���
};