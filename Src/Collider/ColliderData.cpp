#include <algorithm>
#include "ColliderData.h"

ColliderData::ColliderData()
{
}

ColliderData::~ColliderData()
{
}

float CollisionUtility::Clamp(float value, float min, float max)
{
   return (std::max)(min, (std::min)(value, max));
}

float CollisionUtility::SegmentToSegmentDistance(const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2)
{
    Vector3 d1 = q1 - p1; // ü•ª1‚Ì•ûŒü
    Vector3 d2 = q2 - p2; // ü•ª2‚Ì•ûŒü
    Vector3 r = p1 - p2;

    float a = d1.Dot(d1); // ü•ª1‚Ì’·‚³?
    float e = d2.Dot(d2); // ü•ª2‚Ì’·‚³?
    float f = d2.Dot(r);

    float s, t;

    if (a <= 1e-6f && e <= 1e-6f) 
    {
        // —¼•û‚Ìü•ª‚ª“_
        return r.Length();
    }
    if (a <= 1e-6f)
    {
        // ü•ª1‚ª“_
        s = 0.0f;
        t = Clamp(f / e, 0.0f, 1.0f);
    }
    else
    {
        float c = d1.Dot(r);
        if (e <= 1e-6f) 
        {
            // ü•ª2‚ª“_
            t = 0.0f;
            s = Clamp(-c / a, 0.0f, 1.0f);
        }
        else
        {
            float b = d1.Dot(d2);
            float denom = a * e - b * b;

            if (denom != 0.0f) 
            {
                s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
            }
            else 
            {
                s = 0.0f;
            }

            t = (b * s + f) / e;

            if (t < 0.0f) 
            {
                t = 0.0f;
                s = Clamp(-c / a, 0.0f, 1.0f);
            }
            else if (t > 1.0f) 
            {
                t = 1.0f;
                s = Clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }

    Vector3 c1 = p1 + d1 * s;
    Vector3 c2 = p2 + d2 * t;
    return (c1 - c2).Length();
}

bool CollisionUtility::CheckCollision(const ColliderData& a, const ColliderData& b)
{
    if (a.shape == ColliderShape::Capsule && b.shape == ColliderShape::Capsule) 
    {
        return CheckCapsuleCapsule(a.capsule, b.capsule);
    }
}

bool CollisionUtility::CheckCapsuleCapsule(const Capsule& a, const Capsule& b)
{
    Vector3 aTop = a.basePos + Vector3(0, a.height, 0);
    Vector3 bTop = b.basePos + Vector3(0, b.height, 0);

    float dist = SegmentToSegmentDistance(a.basePos, aTop, b.basePos, bTop);
    float radiusSum = a.radius + b.radius;

    return dist < radiusSum;
}
