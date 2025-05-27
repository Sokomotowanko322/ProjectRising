#pragma once
struct Vector2f
{
	float x, y;

	Vector2f() : x(0.0f), y(0.0f) {};

	Vector2f(float vX, float vY) : x(vX), y(vY) {};
	Vector2f(int vX, int vY) {
		x = (float)vX;
		y = (float)vY;
	};

	//�x�N�g���̑傫����Ԃ�
	float Magnitude()const;

	//�x�N�g���̐��K��
	void Normalize();

	//���K���x�N�g����Ԃ�
	Vector2f Normalized();

	void operator+=(const Vector2f& v);
	void operator-=(const Vector2f& v);
	void operator*=(const float scl);
	void operator*=(const Vector2f& v);

	Vector2f operator* (const float scl);

};

Vector2f operator-(const Vector2f& va, const Vector2f vb);
Vector2f operator+(const Vector2f& va, const Vector2f vb);

//���ς�Ԃ�
float Dot(const Vector2f& va, const Vector2f& vb);

//�O�ς�Ԃ�
float Cross(const Vector2f& va, const Vector2f& vb);
