//
// Created by paulo on 30/04/2024.
//

#ifndef HARD_MOTION_ENGINE_H
#define HARD_MOTION_ENGINE_H

#pragma once

#include <sstream>

// B1G number = b1g accuracy
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989380952572010654858632788659361533818279682303019520353018529689957736225994138912497217752834791315155748572424541506959508295331168617278558890750983817546374649393192550604009277016711390098488240128583616035637076601047101819429555961989467678374494482553797747268471040475346462080466842590694912933136770289891521047521620569660240580

namespace source_engine
{
    class vector
    {
    public:

        float x, y, z;

        vector(void);
        vector(float X, float Y, float Z);
        vector(float* clr);

        void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
        bool is_valid() const;
        void invalidate();

        float operator[](int i) const;
        float& operator[](int i);

        float* base();
        float const* base() const;

        void random(float min_val, float max_val);
        void zero();

        bool operator==(const vector& v) const;
        bool operator!=(const vector& v) const;

        vector& operator+=(const vector& v)
        {
            x += v.x; y += v.y; z += v.z;
            return *this;
        }

        vector& operator-=(const vector& v)
        {
            x -= v.x; y -= v.y; z -= v.z;
            return *this;
        }

        vector& operator*=(float fl)
        {
            x *= fl;
            y *= fl;
            z *= fl;
            return *this;
        }

        vector& operator*=(const vector& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        vector& operator/=(const vector& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        vector& operator+=(float fl)
        {
            x += fl;
            y += fl;
            z += fl;
            return *this;
        }

        vector& operator/=(float fl)
        {
            x /= fl;
            y /= fl;
            z /= fl;
            return *this;
        }
        vector& operator-=(float fl)
        {
            x -= fl;
            y -= fl;
            z -= fl;
            return *this;
        }

        void negate();

        float length() const;

        float length_sqr(void) const
        {
            return (x * x + y * y + z * z);
        }

        bool is_zero(float tolerance = 0.01f) const
        {
            return (x > -tolerance && x < tolerance&&
                    y > -tolerance && y < tolerance&&
                    z > -tolerance && z < tolerance);
        }

        float normalize_in_place();

        vector normalized() const;

        bool is_length_greater_than(float val) const
        {
            return length_sqr() > val * val;
        }

        bool IsLengthLessThan(float val) const
        {
            return length_sqr() < val * val;
        }

        vector angle(vector* up = 0);

        bool within_a_box(vector const& boxmin, vector const& boxmax);

        float dist_to(const vector& v_other) const;

        float dist_to_sqr(const vector& v_other) const
        {
            vector delta;

            delta.x = x - v_other.x;
            delta.y = y - v_other.y;
            delta.z = z - v_other.z;

            return delta.length_sqr();
        }

        void copy_to_array(float* rgfl) const;

        void mul_add(const vector& a, const vector& b, float scalar);

        float dot(const vector& v_other) const;

        vector& operator=(const vector& v_other);

        float length_2d(void) const
        {
            return sqrt(x * x + y * y);
        }

        vector  project_onto(const vector& onto);

        vector	operator-(void) const;
        vector	operator+(const vector& v) const;
        vector	operator-(const vector& v) const;
        vector	operator*(const vector& v) const;
        vector	operator/(const vector& v) const;
        vector	operator*(float fl) const;
        vector	operator/(float fl) const;

        vector cross(const vector& vOther) const;

        vector min_(const vector& vOther) const;
        vector max_(const vector& vOther) const;
    };

    float vector_length(const vector& v);
    void vector_cross_product(const vector& a, const vector& b, vector& result);
    float normalize_vector(vector& v);


    class vector_aligned : public vector
    {

    public:

        inline vector_aligned(void) {};
        inline vector_aligned(float x, float y, float z)
        {
            init(x, y, z);
        }

    public:
        explicit vector_aligned(const vector& v_other)
        {
            init(v_other.x, v_other.y, v_other.z);
        }

        vector_aligned& operator=(const vector& v_other)
        {
            init(v_other.x, v_other.y, v_other.z);
            return *this;
        }

        vector_aligned& operator=(const vector_aligned& v_other)
        {
            x = v_other.x;
            y = v_other.y;
            z = v_other.z;
            return *this;
        }

        float w;
    };
}

#endif //HARD_MOTION_ENGINE_H
