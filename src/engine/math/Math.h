#pragma once

#include <directxmath.h>

namespace NSE
{
    struct Vector3d
    {
        double x;
        double y;
        double z;

        Vector3d() = default;

        Vector3d(const Vector3d&) = default;
        Vector3d& operator=(const Vector3d&) = default;

        Vector3d(Vector3d&&) = default;
        Vector3d& operator=(Vector3d&&) = default;

        Vector3d(const DirectX::XMFLOAT3& v);
        explicit operator DirectX::XMFLOAT3() const;

        constexpr Vector3d(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
        explicit Vector3d(const double *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
    };

    Vector3d __vectorcall operator+ (Vector3d v);
    Vector3d __vectorcall operator- (Vector3d v);

    Vector3d& __vectorcall operator+= (Vector3d& a, Vector3d b);
    Vector3d& __vectorcall operator-= (Vector3d& a, Vector3d b);
    Vector3d& __vectorcall operator*= (Vector3d& a, Vector3d b);
    Vector3d& __vectorcall operator/= (Vector3d& a, Vector3d b);

    Vector3d& operator*= (Vector3d& v, double s);
    Vector3d& operator/= (Vector3d& v, double s);

    Vector3d __vectorcall operator+ (Vector3d a, Vector3d b);
    Vector3d __vectorcall operator- (Vector3d a, Vector3d b);
    Vector3d __vectorcall operator* (Vector3d a, Vector3d b);
    Vector3d __vectorcall operator/ (Vector3d a, Vector3d b);
    Vector3d __vectorcall operator* (Vector3d v, double s);
    Vector3d __vectorcall operator* (double s, Vector3d v);
    Vector3d __vectorcall operator/ (Vector3d v, double s);

    class Math
    {
        // constexpr DirectX::XMVECTOR ZeroVector
    };
}
