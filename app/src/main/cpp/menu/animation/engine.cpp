//
// Created by sel on 30/04/2024.
//


#include "engine.h"

#include <cmath>
#include <limits>

inline float sqrt2(float sqr)
{
    float root = 0;

#ifdef _M_IX86

    __asm
    {
        sqrtss xmm0, sqr
        movss root, xmm0
    }
#endif
    return root;
}

namespace source_engine
{
    void VectorCopy(const vector& src, vector& dst)
    {
        dst.x = src.x;
        dst.y = src.y;
        dst.z = src.z;
    }
    void VectorLerp(const vector& src1, const vector& src2, float t, vector& dest)
    {
        dest.x = src1.x + (src2.x - src1.x) * t;
        dest.y = src1.y + (src2.y - src1.y) * t;
        dest.z = src1.z + (src2.z - src1.z) * t;
    }
    float vector_length(const vector& v)
    {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }
    void vector_cross_product(const vector& a, const vector& b, vector& result)
    {
        result.x = a.y * b.z - a.z * b.y;
        result.y = a.z * b.x - a.x * b.z;
        result.z = a.x * b.y - a.y * b.x;
    }
    float normalize_vector(vector& v)
    {
        float l = v.length();
        if (l != 0.0f) {
            v /= l;
        }
        else {
            v.x = v.y = v.z = 0.0f;
        }
        return l;
    }

    vector::vector(void)
    {
        invalidate();
    }
    vector::vector(float X, float Y, float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }
    vector::vector(float* clr)
    {
        x = clr[0];
        y = clr[1];
        z = clr[2];
    }
    vector vector::angle(vector* up)
    {
        if (!x && !y)
            return vector(0, 0, 0);

        float roll = 0;

        if (up)
        {
            vector left = (*up).cross(*this);
            roll = atan2f(left.z, (left.y * x) - (left.x * y)) * 180.0f / M_PI;
        }

        return vector(atan2f(-z, sqrt2(x * x + y * y)) * 180.0f / M_PI, atan2f(y, x) * 180.0f / M_PI, roll);
    }

    void vector::init(float ix, float iy, float iz)
    {
        x = ix; y = iy; z = iz;
    }

    vector vector::normalized() const
    {
        vector res = *this;
        float l = res.length();
        if (l != 0.0f) {
            res /= l;
        }
        else {
            res.x = res.y = res.z = 0.0f;
        }
        return res;
    }
    float vector::normalize_in_place()
    {
        return normalize_vector(*this);
    }

    void vector::random(float min_val, float max_val)
    {
        x = min_val + ((float)rand() / RAND_MAX) * (max_val - min_val);
        y = min_val + ((float)rand() / RAND_MAX) * (max_val - min_val);
        z = min_val + ((float)rand() / RAND_MAX) * (max_val - min_val);
    }

    void vector::zero()
    {
        x = y = z = 0.0f;
    }

    vector& vector::operator=(const vector& vOther)
    {
        x = vOther.x; y = vOther.y; z = vOther.z;
        return *this;
    }

    float& vector::operator[](int i)
    {
        return ((float*)this)[i];
    }

    float vector::operator[](int i) const
    {
        return ((float*)this)[i];
    }

    float* vector::base()
    {
        return (float*)this;
    }

    float const* vector::base() const
    {
        return (float const*)this;
    }

    bool vector::is_valid() const
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }

    void vector::invalidate()
    {
        x = y = z = std::numeric_limits<float>::infinity();
    }

    bool vector::operator==(const vector& src) const
    {
        return (src.x == x) && (src.y == y) && (src.z == z);
    }

    bool vector::operator!=(const vector& src) const
    {
        return (src.x != x) || (src.y != y) || (src.z != z);
    }

    void vector::copy_to_array(float* rgfl) const
    {
        rgfl[0] = x, rgfl[1] = y, rgfl[2] = z;
    }

    void vector::negate()
    {
        x = -x; y = -y; z = -z;
    }

    vector vector::project_onto(const vector& onto)
    {
        return onto * (this->dot(onto) / (onto.length_2d()));
    }

    void vector::mul_add(const vector& a, const vector& b, float scalar)
    {
        x = a.x + b.x * scalar;
        y = a.y + b.y * scalar;
        z = a.z + b.z * scalar;
    }

    float vector::dot(const vector& b) const
    {
        return (x * b.x + y * b.y + z * b.z);
    }
    void vector_clear(vector& a)
    {
        a.x = a.y = a.z = 0.0f;
    }

    float vector::length(void) const
    {
        return sqrt(x * x + y * y + z * z);
    }

    bool vector::within_a_box(vector const& boxmin, vector const& boxmax)
    {
        return (
                (x >= boxmin.x) && (x <= boxmax.x) &&
                (y >= boxmin.y) && (y <= boxmax.y) &&
                (z >= boxmin.z) && (z <= boxmax.z)
        );
    }

    float vector::dist_to(const vector& vOther) const
    {
        vector delta;
        delta = *this - vOther;
        return delta.length();
    }

    vector vector::min_(const vector& vOther) const
    {
        return vector(x < vOther.x ? x : vOther.x,
                      y < vOther.y ? y : vOther.y,
                      z < vOther.z ? z : vOther.z);
    }

    vector vector::max_(const vector& vOther) const
    {
        return vector(x > vOther.x ? x : vOther.x,
                      y > vOther.y ? y : vOther.y,
                      z > vOther.z ? z : vOther.z);
    }

    vector vector::operator-(void) const
    {
        return vector(-x, -y, -z);
    }

    vector vector::operator+(const vector& v) const
    {
        return vector(x + v.x, y + v.y, z + v.z);
    }

    vector vector::operator-(const vector& v) const
    {
        return vector(x - v.x, y - v.y, z - v.z);
    }

    vector vector::operator*(float fl) const
    {
        return vector(x * fl, y * fl, z * fl);
    }

    vector vector::operator*(const vector& v) const
    {
        return vector(x * v.x, y * v.y, z * v.z);
    }

    vector vector::operator/(float fl) const
    {
        return vector(x / fl, y / fl, z / fl);
    }

    vector vector::operator/(const vector& v) const
    {
        return vector(x / v.x, y / v.y, z / v.z);
    }

    vector operator*(float fl, const vector& v)
    {
        return v * fl;
    }

    vector vector::cross(const vector& vOther) const
    {
        vector res;
        vector_cross_product(*this, vOther, res);
        return res;
    }
}