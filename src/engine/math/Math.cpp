#include "Math.h"

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

