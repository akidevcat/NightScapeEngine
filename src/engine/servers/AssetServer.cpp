#include "AssetServer.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../addon/DDSTextureLoader11.h"
#include "ObjectServer.h"
#include "RenderServer.h"

NSE::AssetsServer::AssetsServer()
{

}

NSE::AssetsServer::~AssetsServer()
{

}

DirectX::XMFLOAT3 ParseObjLineVector3(const std::string& line)
{
    static std::stringstream s;
    static std::string word;

    s.clear();

    DirectX::XMFLOAT3 v{};

    s << line;
    s >> word; // prefix, skip
    s >> v.x >> v.y >> v.z;

    return v;
}

DirectX::XMFLOAT2 ParseObjLineVector2(const std::string& line)
{
    static std::stringstream s;
    static std::string word;

    s.clear();

    DirectX::XMFLOAT2 v{};

    s << line;
    s >> word; // prefix, skip
    s >> v.x >> v.y;

    return v;
}

void ParseObjLineFace(std::string line, uint32_t faceIndicies[9], std::string triangleNames[3])
{
    static std::stringstream s;
    static std::string word;

    s.clear();

    s << line;
    s >> word;
    s >> triangleNames[0];
    s >> triangleNames[1];
    s >> triangleNames[2];
    s.clear();

    std::replace(line.begin(), line.end(), '/', ' ');

    s << line;
    s >> word; // prefix, skip

    for (int i = 0; i < 9; i++)
        s >> faceIndicies[i];
}

NSE_Mesh NSE::AssetsServer::LoadMeshAsset(const std::string &path)
{
    std::ifstream file(path);

    assert(file.is_open());

    std::vector<DirectX::XMFLOAT3> vertices{};
    std::vector<DirectX::XMFLOAT3> normals{};
    std::vector<DirectX::XMFLOAT2> uvs{};
    std::vector<uint32_t> resultIndicies{};
    std::vector<std::pair<uint32_t, VertexData>> resultVertices{};

    uint32_t vertexCount = 0;
    std::unordered_map<std::string, std::pair<uint32_t, VertexData>> mappedVertices{};

    std::string triangleNames[3];
    uint32_t faceIndicies[9];

    std::string line;
    while (std::getline(file, line))
    {
        if (line.rfind("v ", 0) == 0)
        {
            vertices.emplace_back(ParseObjLineVector3(line));
        }
        else if (line.rfind("vn ", 0) == 0)
        {
            normals.emplace_back(ParseObjLineVector3(line));
        }
        else if (line.rfind("vt ", 0) == 0)
        {
            uvs.emplace_back(ParseObjLineVector2(line));
        }
        else if (line.rfind("f ", 0) == 0)
        {
            ParseObjLineFace(line, faceIndicies, triangleNames);

            auto it = mappedVertices.find(triangleNames[0]);
            if (it == mappedVertices.end())
            {
                auto pos = vertices[faceIndicies[0] - 1];
                auto normal = normals[faceIndicies[2] - 1];
                auto uv = uvs[faceIndicies[1] - 1];
                auto vId = vertexCount++;
                mappedVertices.emplace(triangleNames[0], std::make_pair(vId, VertexData{pos, normal, uv, {1,1,1,1}}));
                resultIndicies.emplace_back(vId);
            }
            else
            {
                auto vId = it->second.first;
                resultIndicies.emplace_back(vId);
            }

            it = mappedVertices.find(triangleNames[1]);
            if (it == mappedVertices.end())
            {
                auto pos = vertices[faceIndicies[3] - 1];
                auto normal = normals[faceIndicies[5] - 1];
                auto uv = uvs[faceIndicies[4] - 1];
                auto vId = vertexCount++;
                mappedVertices.emplace(triangleNames[1], std::make_pair(vId, VertexData{pos, normal, uv, {1,1,1,1}}));
                resultIndicies.emplace_back(vId);
            }
            else
            {
                auto vId = it->second.first;
                resultIndicies.emplace_back(vId);
            }

            it = mappedVertices.find(triangleNames[2]);
            if (it == mappedVertices.end())
            {
                auto pos = vertices[faceIndicies[6] - 1];
                auto normal = normals[faceIndicies[8] - 1];
                auto uv = uvs[faceIndicies[7] - 1];
                auto vId = vertexCount++;
                mappedVertices.emplace(triangleNames[2], std::make_pair(vId, VertexData{pos, normal, uv, {1,1,1,1}}));
                resultIndicies.emplace_back(vId);
            }
            else
            {
                auto vId = it->second.first;
                resultIndicies.emplace_back(vId);
            }

            // std::swap(resultIndicies[resultIndicies.size() - 1], resultIndicies[resultIndicies.size() - 3]);
        }
    }

    auto mesh = CreateObject<Mesh>((int)vertexCount, (int)resultIndicies.size());

    auto mVertices = mesh->GetVertices();
    auto mIndices = mesh->GetIndices();

    for (const auto& v : mappedVertices)
    {
        mVertices[v.second.first] = v.second.second;
    }
    memcpy(mIndices, resultIndicies.data(), resultIndicies.size() * sizeof(uint32_t));
    // result->vertices; //.. copy
    // result->Upload();
    mesh->Upload();

    return mesh;
}

NSE_Texture2D NSE::AssetsServer::LoadTextureAsset(const wchar_t* path)
{
    auto device = RenderServer::Get()->GetDevice();

    ID3D11Resource* resource;
    ID3D11ShaderResourceView* view;

    auto status = DirectX::CreateDDSTextureFromFile(device, path, &resource, &view);
    assert(SUCCEEDED(status));

    auto result = CreateObject<Texture2D>(resource, view);

    return result;
}
