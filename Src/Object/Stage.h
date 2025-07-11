#pragma once
#include <vector>
#include <memory>
#include "../Object/Transform.h"

class Stage
{
public:
    Stage();
    ~Stage();

    void Init();
    void Update();
    void Draw();

	// ステージのTransform
    Transform& GetTransform();

    // ステージのポジション
    VECTOR GetPos() const;

private:
  
    Transform stageTransform_;
  };