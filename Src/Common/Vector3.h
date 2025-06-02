#pragma once
#include <cmath>

struct Vector3 
{
    float x;
    float y;
    float z;

    Vector3();
    Vector3(float x_, float y_, float z_);

    Vector3 operator+(const Vector3& rhs) const;
    Vector3 operator-(const Vector3& rhs) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    float Dot(const Vector3& rhs) const;
    float Length() const;
    Vector3 Normalize() const;
};