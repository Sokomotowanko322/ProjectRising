#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/ColliderManager.h"
#include "../Object/Unit/Player.h"
#include "../Object/Unit/NormalEnemy.h"
#include "Stage.h"

Stage::Stage()
{
    
}

Stage::~Stage()
{
    MV1DeleteModel(stageTransform_.modelId);
}

void Stage::Init()
{
    // ステージモデルのロード
    stageTransform_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::STAGE);
    stageTransform_.pos = { 0.0f, -200.0f, 0.0f };
    stageTransform_.scl = { 100.0f, 100.0f, 100.0f };
    stageTransform_.quaRot = Quaternion();
}

void Stage::Update()
{
	stageTransform_.Update();
}

void Stage::Draw()
{
    // ステージモデル描画
    MV1DrawModel(stageTransform_.modelId);
}

Transform& Stage::GetTransform()
{
    return stageTransform_;
}

VECTOR Stage::GetPos() const
{
    return stageTransform_.pos;
}
