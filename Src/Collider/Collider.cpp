#include "Collider.h"

Collider::Collider(Type type, ActorBase* owner)
{
}

Collider::~Collider()
{
}

bool Collider::CheckCollision(const Collider& other) const
{
	return false;
}

Collider::Type Collider::GetType() const
{
	return type_;
}

ActorBase* Collider::GetOwner() const
{
	return owner_;
}
