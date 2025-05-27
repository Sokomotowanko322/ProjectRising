#include <Dxlib.h>
#include <cmath>
#include "Vector2f.h"

//�x�N�g���̑傫����Ԃ�
float Vector2f::Magnitude() const
{
	return hypot(x, y);
}

//�x�N�g���̐��K��
void Vector2f::Normalize()
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}

//���K���x�N�g����Ԃ�
Vector2f Vector2f::Normalized()
{
	float mag = Magnitude();

	return Vector2f(x /= mag, y /= mag);
}

void Vector2f::operator+=(const Vector2f& v)
{
	x += v.x;
	y += v.y;
}

void Vector2f::operator-=(const Vector2f& v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2f::operator*=(const float scl)
{
	x *= scl;
	y *= scl;
}

void Vector2f::operator*=(const Vector2f& v)
{
	x *= v.x;
	y *= v.y;
}

Vector2f Vector2f::operator*(const float scl)
{
	return Vector2f(x * scl, y * scl);
}

Vector2f operator-(const Vector2f& va, const Vector2f vb)
{
	return Vector2f(va.x - vb.x, va.y - vb.y);
}

Vector2f operator+(const Vector2f& va, const Vector2f vb)
{
	return Vector2f(va.x + vb.x, va.y + vb.y);
}

//����
float Dot(const Vector2f& va, const Vector2f& vb)
{
	return va.x * vb.x + va.y * vb.y;
}

//�O��
float Cross(const Vector2f& va, const Vector2f& vb)
{
	return va.x * vb.y - va.y * vb.x;
}
