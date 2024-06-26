#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <directxmath.h>

#include "../entity/Object.h"
#include"../obj_ptr.h"
#include "../render/GraphicsBuffer.h"

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
        Mesh() = delete;
        Mesh(int vertexCount, int indexCount);
        ~Mesh() override;

        [[nodiscard]] int GetVertexCount() const { return _vertexCount; }
        [[nodiscard]] int GetIndexCount() const { return _indexCount; }
        [[nodiscard]] VertexData* GetVertices() const { return _vertexBuffer->As<VertexData>(); }
        [[nodiscard]] uint32_t* GetIndices() const { return _indexBuffer->As<uint32_t>(); }
        [[nodiscard]] NSE_GraphicsBuffer GetVertexBuffer() const { return _vertexBuffer; }
        [[nodiscard]] NSE_GraphicsBuffer GetIndexBuffer() const { return _indexBuffer; }

        [[nodiscard]] D3D_PRIMITIVE_TOPOLOGY GetTopology() const { return _topology; }
        void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);

        void Upload() const;
        void Resize(int vertexCount, int indexCount);
        void Release();

    public:
        // ToDo Bounds, RecalculateBounds

    private:
        bool _isModified = false; // ToDo

        int _vertexCount = 0;
        int _indexCount = 0;
        NSE_GraphicsBuffer _vertexBuffer = nullptr;
        NSE_GraphicsBuffer _indexBuffer = nullptr;

        D3D_PRIMITIVE_TOPOLOGY _topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    };
}

#endif //MESH_H
