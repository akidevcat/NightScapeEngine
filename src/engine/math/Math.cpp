#include "Math.h"

#include <algorithm>
#include <cstdlib>

namespace NSE
{
    Vector3d::Vector3d(const DirectX::XMVECTOR& v)
    {
        x = v.m128_f32[0];
        y = v.m128_f32[1];
        z = v.m128_f32[2];
    }

    Vector3d::operator DirectX::XMVECTOR() const
    {
        return {(float)x, (float)y, (float)z};
    }

    Vector3d::operator DirectX::XMFLOAT3() const
    {
        return {(float)x, (float)y, (float)z};
    }

    Vector3d __vectorcall operator+(Vector3d v)
    {
        return v;
    }

    Vector3d __vectorcall operator-(Vector3d v)
    {
        v.x = -v.x;
        v.y = -v.y;
        v.z = -v.z;

        return v;
    }

    Vector3d& __vectorcall operator+=(Vector3d &a, Vector3d b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;

        return a;
    }

    Vector3d& __vectorcall operator-=(Vector3d &a, Vector3d b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;

        return a;
    }

    Vector3d& __vectorcall operator*=(Vector3d &a, Vector3d b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;

        return a;
    }

    Vector3d& __vectorcall operator/=(Vector3d &a, Vector3d b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;

        return a;
    }

    Vector3d& operator*=(Vector3d &v, double s)
    {
        v.x *= s;
        v.y *= s;
        v.z *= s;

        return v;
    }

    Vector3d& operator/=(Vector3d &v, double s)
    {
        v.x /= s;
        v.y /= s;
        v.z /= s;

        return v;
    }

    Vector3d __vectorcall operator+(Vector3d a, Vector3d b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;

        return a;
    }

    Vector3d __vectorcall operator-(Vector3d a, Vector3d b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;

        return a;
    }

    Vector3d __vectorcall operator*(Vector3d a, Vector3d b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;

        return a;
    }

    Vector3d __vectorcall operator/(Vector3d a, Vector3d b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;

        return a;
    }

    Vector3d __vectorcall operator*(Vector3d v, double s)
    {
        v.x *= s;
        v.y *= s;
        v.z *= s;

        return v;
    }

    Vector3d __vectorcall operator*(double s, Vector3d v)
    {
        v.x *= s;
        v.y *= s;
        v.z *= s;

        return v;
    }

    Vector3d __vectorcall operator/(Vector3d v, double s)
    {
        v.x /= s;
        v.y /= s;
        v.z /= s;

        return v;
    }

    float Math::Random()
    {
        return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }

    unsigned int Math::MurmurHash13(unsigned int x, unsigned int y, unsigned int z)
    {
        constexpr unsigned int M = 0x5bd1e995u;
        unsigned int h = 1190494759u;
        x = ((x*M)>>24u)*M;
        y = ((y*M)>>24u)*M;
        z = ((z*M)>>24u)*M;
        h *= M; h ^= x; h *= M; h ^= y; h *= M; h ^= z;
        h ^= h>>13u; h *= M; h ^= h>>15u;
        return h;
    }

    float Math::Hash13(float3 value)
    {
        // unsigned int h = MurmurHash13(static_cast<unsigned int>(value.x), static_cast<unsigned int>(value.y), static_cast<unsigned int>(value.z));
        // return static_cast<float>(h & 0x007fffffu | 0x3f800000u) - 1.0f;
        float _;
        value.x = modff(value.x * .1031f, &_);
        value.y = modff(value.y * .1031f, &_);
        value.z = modff(value.z * .1031f, &_);
        float d = (value.x * (value.x + 31.32f) + value.y * (value.y + 31.32f) + value.z * (value.z + 31.32f));
        value += float3(d,d,d);
        return modff((value.x + value.y) * value.z, &_);
    }

    float Math::Noise3D(float3 pos)
    {
        float _;

        float3 p;
        float3 w;

        p.x = floorf(pos.x);
        p.y = floorf(pos.y);
        p.z = floorf(pos.z);
        w.x = modff(pos.x, &_);
        w.y = modff(pos.y, &_);
        w.z = modff(pos.z, &_);

        float3 u = w*w*w*(w*(w*6.0f-float3(15.0f,15.0f,15.0f))+float3(10.0f, 10.0f, 10.0f));

        float a = Hash13( p+float3(0,0,0) );
        float b = Hash13( p+float3(1,0,0) );
        float c = Hash13( p+float3(0,1,0) );
        float d = Hash13( p+float3(1,1,0) );
        float e = Hash13( p+float3(0,0,1) );
        float f = Hash13( p+float3(1,0,1) );
        float g = Hash13( p+float3(0,1,1) );
        float h = Hash13( p+float3(1,1,1) );

        float k0 =   a;
        float k1 =   b - a;
        float k2 =   c - a;
        float k3 =   e - a;
        float k4 =   a - b - c + d;
        float k5 =   a - c - e + g;
        float k6 =   a - b - e + f;
        float k7 = - a + b + c - d + e - f - g + h;

        return -1.0f+2.0f*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z);
    }

    float length(float3 v)
    {
        return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    double length(Vector3d v)
    {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    float3 normalize(float3 v)
    {
        return v / length(v);
    }

    Vector3d normalize(Vector3d v)
    {
        return v / length(v);
    }

    float saturate(float v)
    {
        return std::clamp(v, 0.0f, 1.0f);
    }

    double saturate(double v)
    {
        return std::clamp(v, 0.0, 1.0);
    }

    float3 __vectorcall operator+(float3 v)
    {
        return v;
    }

    float3 __vectorcall operator-(float3 v)
    {
        v.x = -v.x;
        v.y = -v.y;
        v.z = -v.z;

        return v;
    }

    float3& __vectorcall operator+=(float3 &a, float3 b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;

        return a;
    }

    float3& __vectorcall operator-=(float3 &a, float3 b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;

        return a;
    }

    float3& __vectorcall operator*=(float3 &a, float3 b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;

        return a;
    }

    float3& __vectorcall operator/=(float3 &a, float3 b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;

        return a;
    }

    float3& operator*=(float3 &v, float s)
    {
        v.x *= s;
        v.y *= s;
        v.z *= s;

        return v;
    }

    float3& operator/=(float3 &v, double s)
    {
        v.x /= s;
        v.y /= s;
        v.z /= s;

        return v;
    }

    float3 __vectorcall operator+(float3 a, float3 b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;

        return a;
    }

    float3 __vectorcall operator-(float3 a, float3 b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;

        return a;
    }

    float3 __vectorcall operator*(float3 a, float3 b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;

        return a;
    }

    float3 __vectorcall operator/(float3 a, float3 b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;

        return a;
    }

    float3 __vectorcall operator*(float3 v, float s)
    {
        v.x *= s;
        v.y *= s;
        v.z *= s;

        return v;
    }

    float3 __vectorcall operator*(float s, float3 v)
    {
        v.x *= s;
        v.y *= s;
        v.z *= s;

        return v;
    }

    float3 __vectorcall operator/(float3 v, float s)
    {
        v.x /= s;
        v.y /= s;
        v.z /= s;

        return v;
    }
}

