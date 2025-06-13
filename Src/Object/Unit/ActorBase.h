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

   
	// 取得系
    const Transform& GetTransform(void) const;
    virtual VECTOR GetPos() const = 0; // 純粋仮想関数
    virtual void SetPos(const VECTOR& pos) = 0; // 純粋仮想関数
    int GetHP() const;

	// 設定系
    int SetHP(int hp);
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


#pragma region フラグ

    // 生存フラグ
    bool isAlive_;

    // 無敵フラグ
	bool isInvincible_;

#pragma endregion 

private:

};