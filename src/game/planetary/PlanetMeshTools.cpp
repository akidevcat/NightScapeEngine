#include "PlanetMeshTools.h"

#include "../../engine/addon/FastNoiseLite.h"
#include "../../engine/math/Math.h"
#include "../../engine/servers/ObjectServer.h"

using namespace NSE;

inline void ApplyFaceRotation(Vector3d& value, char faceID)
{
    switch (faceID)
    {
        case 0:
            // xyz
            break;
        case 1:
            // yxz
            std::swap(value.x, value.y);
            value.y *= -1;
        break;
        case 2:
            // -yxz
            std::swap(value.x, value.y);
            value.x *= -1;
        break;
        case 3:
            // xzy
            std::swap(value.y, value.z);
            value.y *= -1;
            break;
        case 4:
            // xz-y
            std::swap(value.y, value.z);
            value.z *= -1;
            break;
        case 5:
            // x-yz
            value.x *= -1;
            value.y *= -1;
            break;
        default:
            assert(false);
        break;
    }
}

obj_ptr<Mesh> PlanetMeshTools::CreateChunkMesh(int resolution)
{
    auto mesh = CreateObject<Mesh>(resolution * resolution, (resolution - 1) * (resolution - 1) * 6);

    return mesh;
}

void PlanetMeshTools::SetupChunkMesh(const obj_ptr<NSE::Mesh>& mesh, uint32_t chunkID, int resolution, float radius, float maxHeight,
    NSE::Vector3d &chunkPivot)
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);

    noise.SetSeed(0);
    noise.SetFrequency(8.0f);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(3);
    noise.SetFractalGain(0.3f);
    noise.SetFractalLacunarity(4.0f);

    double scale = 1.0;
    double offsetX = 0.0;
    double offsetY = 0.0;

    char faceID = (char)(chunkID >> (32 - 3));
    char lod = (char)((chunkID >> (32 - 8)) & (0b00011111));

    for (char i = 0; i < lod; i++)
    {
        char subdivisionQuarter = (char)((chunkID >> (i * 2)) & (0b11));
        scale *= 0.5f;
        switch (subdivisionQuarter)
        {
            case 0:
                offsetX -= scale * 0.5f;
                offsetY += scale * 0.5f;
                break;
            case 1:
                offsetX += scale * 0.5f;
                offsetY += scale * 0.5f;
                break;
            case 2:
                offsetX -= scale * 0.5f;
                offsetY -= scale * 0.5f;
                break;
            case 3:
                offsetX += scale * 0.5f;
                offsetY -= scale * 0.5f;
                break;
            default:
                assert(false);
                break;
        }
    }

    auto vertices = mesh->GetVertices();
    auto indices = mesh->GetIndices();

    static Vector3d verticesDouble[16 * 16];
    assert(resolution <= 16);

    Vector3d pivot{};

    // Vector3d pivot{offsetX, 0.5, offsetY};

    // ApplyFaceRotation(pivot, faceID);
    //
    // pivot = normalize(pivot) * radius;

    for (int i = 0, y = 0; y < resolution; y++)
    {
        for (int x = 0; x < resolution; x++, i++)
        {
            double nx = (x - 1) / (double)(resolution - 1 - 2) - 0.5;
            double ny = (y - 1) / (double)(resolution - 1 - 2) - 0.5;

            Vector3d v{nx * scale + offsetX, 0.5, ny * scale + offsetY};

            ApplyFaceRotation(v, faceID);

            float3 normal;

            v = normalize(v);
            normal = (float3)v;

            float height = noise.GetNoise(v.x, v.y, v.z) * 0.5f + 0.5f;
            height *= maxHeight;

            if (x == 0 || y == 0 || x == resolution - 1 || y == resolution - 1)
                height -= maxHeight * 0.001f;

            v *= radius + height;

            verticesDouble[i] = v;

            pivot += v;

            VertexData vd;

            vd.color = {1,1,1,1};
            vd.normal = normal;
            vd.uv = float2((float)(nx + 0.5), (float)(ny + 0.5));

            vertices[i] = vd;
        }
    }

    pivot /= (resolution * resolution);

    for (int i = 0; i < resolution * resolution; i++)
    {
        vertices[i].position = (float3)(verticesDouble[i] - pivot);
    }

    for (int ti = 0, vi = 0, y = 0; y < resolution - 1; y++, vi++)
    {
        for (int x = 0; x < resolution - 1; x++, ti += 6, vi++)
        {
            indices[ti] = vi;
            indices[ti + 3] = indices[ti + 2] = vi + 1;
            indices[ti + 4] = indices[ti + 1] = vi + resolution;
            indices[ti + 5] = vi + resolution + 1;
        }
    }

    mesh->Upload();

    chunkPivot = pivot;
}
