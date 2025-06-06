#include <algorithm>
#include <cmath>
#include "CollisionUtility.h"

float CollisionUtility::GetSegmentSegmentDistance(const VECTOR& a0, const VECTOR& a1, const VECTOR& b0, const VECTOR& b1)
{
    VECTOR d1 = VSub(a1, a0);
    VECTOR d2 = VSub(b1, b0);
    VECTOR r = VSub(a0, b0);

    float a = VDot(d1, d1);
    float e = VDot(d2, d2);
    float f = VDot(d2, r);

    float s, t;

    if (a <= 1e-6f && e <= 1e-6f) {
        // —¼•û“_
        return GetDistance(a0, b0);
    }

    if (a <= 1e-6f) {
        // a‚ª“_
        s = 0.0f;
        t = std::clamp(f / e, 0.0f, 1.0f);
    }
    else {
        float c = VDot(d1, r);
        if (e <= 1e-6f) {
            // b‚ª“_
            t = 0.0f;
            s = std::clamp(-c / a, 0.0f, 1.0f);
        }
        else {
            float b = VDot(d1, d2);
            float denom = a * e - b * b;

            if (denom != 0.0f) {
                s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
            }
            else {
                s = 0.0f;
            }

            t = (b * s + f) / e;

            if (t < 0.0f) {
                t = 0.0f;
                s = std::clamp(-c / a, 0.0f, 1.0f);
            }
            else if (t > 1.0f) {
                t = 1.0f;
                s = std::clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }

    VECTOR closestA = VAdd(a0, VScale(d1, s));
    VECTOR closestB = VAdd(b0, VScale(d2, t));

    return GetDistance(closestA, closestB);
}

float CollisionUtility::GetDistance(const VECTOR& a, const VECTOR& b)
{
	VECTOR d = VSub(a, b);
	return std::sqrt(VDot(d, d));
}
