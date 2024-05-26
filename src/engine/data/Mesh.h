#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <directxmath.h>

struct VertexData
{
    DirectX::XMFLOAT3 position = {0, 0, 0};
    DirectX::XMFLOAT3 normal = {0, 0, 0};
    DirectX::XMFLOAT2 uv = {0, 0};
    DirectX::XMFLOAT4 color = {1, 1, 1, 1};
    //
    // VertexData()
    // {
    //     this->position = DirectX::XMFLOAT3(0, 0, 0);
    //     this->normal = DirectX::XMFLOAT3(0, 0, 0);
    //     this->uv = DirectX::XMFLOAT2(0, 0);
    //     this->color = DirectX::XMFLOAT4(1, 1, 1, 1);
    // }
    //
    // VertexData(DirectX::XMFLOAT3 position)
    // {
    //     this->position = position;
    //     this->normal = DirectX::XMFLOAT3(0, 0, 0);
    //     this->uv = DirectX::XMFLOAT2(0, 0);
    //     this->color = DirectX::XMFLOAT4(1, 1, 1, 1);
    // }
    //
    // VertexData(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal)
    // {
    //     this->position = position;
    //     this->normal = normal;
    //     this->uv = DirectX::XMFLOAT2(0, 0);
    //     this->color = DirectX::XMFLOAT4(1, 1, 1, 1);
    // }
    //
    // VertexData(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 uv)
    // {
    //     this->position = position;
    //     this->normal = normal;
    //     this->uv = uv;
    //     this->color = DirectX::XMFLOAT4(1, 1, 1, 1);
    // }
    //
    // VertexData(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT4 color)
    // {
    //     this->position = position;
    //     this->normal = normal;
    //     this->uv = uv;
    //     this->color = color;
    // }
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
