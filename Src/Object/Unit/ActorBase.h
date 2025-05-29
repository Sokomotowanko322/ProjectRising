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

   
	// 取得系
    const Transform& GetTransform(void) const;
    VECTOR GetPosition() const;
    int GetHP() const;

	// 設定系
    void SetHP(int hp);
    bool IsAlive() const;


protected:

    // 参照
    Transform transform_;
    ResourceManager& resMng_;
    SceneManager& scnMng_;

	// プレイヤーの参照
    std::weak_ptr<Player> player_;
    
#pragma region パラメーター

    // 位置
    VECTOR position_;

    // 体力
    int hp_;

#pragma endregion

private:

#pragma region フラグ

    // 生存フラグ
	bool isAlive_;
  

#pragma endregion 
};