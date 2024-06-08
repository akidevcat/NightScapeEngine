#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <directxmath.h>

#include "../entity/Object.h"
#include"../obj_ptr.h"

#define NSE_Mesh obj_ptr<NSE::Mesh>
#define NSE_VertexData obj_ptr<NSE::VertexData>

namespace NSE
{
    struct VertexData
    {
        DirectX::XMFLOAT3 position = {0, 0, 0};
        DirectX::XMFLOAT3 normal = {0, 0, 0};
        DirectX::XMFLOAT2 uv = {0, 0};
        DirectX::XMFLOAT4 color = {1, 1, 1, 1};
    };

    class Mesh : public Object
    {
    public:
        Mesh();
        Mesh(int vertexCount, int indexCount);
        ~Mesh();

        int GetVertexCount() const;

        bool Upload();
        void Release();

    public:
        int vertexCount = 0;
        int indexCount = 0;
        VertexData* vertices = nullptr;
        uint32_t* indices = nullptr;

        ID3D11Buffer* vertexBuffer = nullptr;
        ID3D11Buffer* indexBuffer = nullptr;
        // ToDo Bounds, RecalculateBounds

    private:
        bool _isModified = false; // ToDo
    };
}

#endif //MESH_H
