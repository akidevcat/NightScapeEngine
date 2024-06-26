#include "Mesh.h"

#include "../servers/RenderServer.h"

NSE::Mesh::Mesh(int vertexCount, int indexCount)
{
    _vertexCount = vertexCount;
    _indexCount = indexCount;

    _vertexBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Vertex, sizeof(VertexData), vertexCount, true);
    _indexBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Index, sizeof(uint32_t), indexCount, true);
}

NSE::Mesh::~Mesh()
{
    Release();

    DestroyObject(_vertexBuffer);
    DestroyObject(_indexBuffer);
}

void NSE::Mesh::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
    _topology = topology;
}

void NSE::Mesh::Upload() const
{
    _vertexBuffer->Upload();
    _indexBuffer->Upload();
}

void NSE::Mesh::Resize(int vertexCount, int indexCount)
{
    _vertexCount = vertexCount;
    _indexCount = indexCount;

    _vertexBuffer->Resize(vertexCount, sizeof(VertexData));
    _indexBuffer->Resize(indexCount, sizeof(uint32_t));
}

void NSE::Mesh::Release()
{
    if (_vertexBuffer)
    {
        _vertexBuffer->Release();
    }

    if (_indexBuffer)
    {
        _indexBuffer->Release();
    }
}
