#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <directxmath.h>

struct VertexData
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 uv;
    DirectX::XMFLOAT4 color;
};

class Mesh
{
public:
    Mesh();
    Mesh(int vertexCount, int indexCount);
    ~Mesh();

    int GetVertexCount();

    bool UploadData(ID3D11Device* device);
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

#endif //MESH_H
