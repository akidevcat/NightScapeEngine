#include "Math.h"

namespace NSE
{
    Vector3d::Vector3d(const DirectX::XMFLOAT3& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
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
}