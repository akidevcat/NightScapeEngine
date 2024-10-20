#include "Mesh.h"

#include "RenderServer.h"

NSE::Mesh::Mesh()
{
    _vertexCount = 0;
    _indexCount = 0;

    _vertexBuffer = sRender->api()->CreateGraphicsBuffer(GraphicsBuffer::Target::Vertex, sizeof(VertexData), 0, true);
    _indexBuffer = sRender->api()->CreateGraphicsBuffer(GraphicsBuffer::Target::Index, sizeof(uint32_t), 0, true);
}

NSE::Mesh::Mesh(int vertexCount, int indexCount)
{
    _vertexCount = vertexCount;
    _indexCount = indexCount;

    _vertexBuffer = sRender->api()->CreateGraphicsBuffer(GraphicsBuffer::Target::Vertex, sizeof(VertexData), vertexCount, true);
    _indexBuffer = sRender->api()->CreateGraphicsBuffer(GraphicsBuffer::Target::Index, sizeof(uint32_t), indexCount, true);
}

NSE::Mesh::~Mesh()
{
    Release();
}

void NSE::Mesh::SetTopology(PrimitiveTopology topology)
{
    _topology = topology;
}

void NSE::Mesh::Upload()
{
    _vertexBuffer->Upload();
    _indexBuffer->Upload();
}

void NSE::Mesh::Resize(int vertexCount, int indexCount)
{
}

void NSE::Mesh::Release()
{
    _vertexBuffer->Release();
    _indexBuffer->Release();
}
