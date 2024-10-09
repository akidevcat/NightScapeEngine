#pragma once

#include "VertexData.h"
#include "PrimitiveTopology.h"
#include "GraphicsBuffer.h"

namespace NSE
{
    class Mesh : public RefCounted
    {
    public:
        Mesh();
        Mesh(int vertexCount, int indexCount);
        ~Mesh() override;

        [[nodiscard]] int GetVertexCount() const { return _vertexCount; }
        [[nodiscard]] int GetIndexCount() const { return _indexCount; }
        [[nodiscard]] VertexData* GetVertices() { return _vertexBuffer->As<VertexData>(); }
        [[nodiscard]] uint32_t* GetIndices() { return _indexBuffer->As<uint32_t>(); }
        [[nodiscard]] Ref<GraphicsBuffer> GetVertexBuffer() const { return _vertexBuffer; }
        [[nodiscard]] Ref<GraphicsBuffer> GetIndexBuffer() const { return _indexBuffer; }

        [[nodiscard]] PrimitiveTopology GetTopology() const { return _topology; }
        void SetTopology(PrimitiveTopology topology);

        void Upload();
        void Resize(int vertexCount, int indexCount);
        void Release();

    private:
        int _vertexCount = 0;
        int _indexCount = 0;
        SRef<GraphicsBuffer> _vertexBuffer = nullptr;
        SRef<GraphicsBuffer> _indexBuffer = nullptr;

        PrimitiveTopology _topology = PrimitiveTopology::Triangles;
    };
}
