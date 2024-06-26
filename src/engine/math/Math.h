#pragma once

#include <directxmath.h>

typedef DirectX::XMVECTOR xmvector;
typedef DirectX::XMMATRIX xmmatrix;
typedef DirectX::XMFLOAT2 float2;
typedef DirectX::XMFLOAT3 float3;
typedef DirectX::XMFLOAT4 float4;

namespace NSE
{
    float3 __vectorcall operator+ (float3 v);
    float3 __vectorcall operator- (float3 v);

    float3& __vectorcall operator+= (float3& a, float3 b);
    float3& __vectorcall operator-= (float3& a, float3 b);
    float3& __vectorcall operator*= (float3& a, float3 b);
    float3& __vectorcall operator/= (float3& a, float3 b);

    float3& operator*= (float3& v, float s);
    float3& operator/= (float3& v, float s);

    float3 __vectorcall operator+ (float3 a, float3 b);
    float3 __vectorcall operator- (float3 a, float3 b);
    float3 __vectorcall operator* (float3 a, float3 b);
    float3 __vectorcall operator/ (float3 a, float3 b);
    float3 __vectorcall operator* (float3 v, float s);
    float3 __vectorcall operator* (float s, float3 v);
    float3 __vectorcall operator/ (float3 v, float s);

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

        Vector3d(const xmvector& v);
        explicit operator xmvector() const;
        explicit operator float3() const;

        constexpr Vector3d(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
        explicit Vector3d(const double *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

        [[nodiscard]] double SqrMagnitude() const
        {
            return x*x + y*y + z*z;
        }

        [[nodiscard]] double Magnitude() const
        {
            return sqrt(SqrMagnitude());
        }

        Vector3d Normalized()
        {
            double m = Magnitude();
            if (m == 0)
            {
                return *this;
            }
            return {x/m, y/m, z/m};
        }

        static Vector3d Lerp(const Vector3d& from, const Vector3d& to, const double& time);

        static double Dot(const Vector3d &left, const Vector3d &right)
        {
            return left.x * right.x + left.y * right.y + left.z * right.z;
        }
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

    inline Vector3d Vector3d::Lerp(const Vector3d& from, const Vector3d& to, const double& time)
    {
        return from + (to - from) * time;
    }

    class Math
    {
        // constexpr DirectX::XMVECTOR ZeroVector
    public:
        static float Random();
    };

    float length(float3 v);
    double length(Vector3d v);
    float3 normalize(float3 v);
    Vector3d normalize(Vector3d v);
}

typedef NSE::Vector3d double3;