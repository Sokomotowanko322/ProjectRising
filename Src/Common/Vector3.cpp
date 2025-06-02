#include "Vector3.h"

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) 
{
}

Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) 
{
}

Vector3 Vector3::operator+(const Vector3& rhs) const 
{
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator-(const Vector3& rhs) const 
{
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator*(float scalar) const 
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const 
{
    return Vector3(x / scalar, y / scalar, z / scalar);
}

float Vector3::Dot(const Vector3& rhs) const 
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

float Vector3::Length() const 
{
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::Normalize() const 
{
    float len = Length();
    if (len == 0.0f) 
    {
        return Vector3(0, 0, 0);
    }
    return *this / len;
}