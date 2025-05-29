#pragma once
#include <memory>
#include <DxLib.h>

class ActorBase; // �O���錾

class Collider 
{
public:

    enum class Type 
    {
        Player,
        Enemy,
		Weapon,
    };

    Collider(Type type, ActorBase* owner);
    virtual ~Collider();

    virtual bool CheckCollision(const Collider& other) const = 0;

    Type GetType() const;
    ActorBase* GetOwner() const;

protected:

    Type type_;

    // ���L��
    ActorBase* owner_;

};