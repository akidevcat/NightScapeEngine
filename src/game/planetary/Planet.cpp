#include "Planet.h"

#include "PlanetMeshTools.h"
#include "../../engine/servers/AssetServer.h"
#include "../../engine/servers/RenderServer.h"

using namespace NSE;

Planet::Planet(const obj_ptr<Planet> &scaledPlanet, const obj_ptr<SceneEntity>& playerEntity)
{
    if (!scaledPlanet)
    {
        _isScaled = true;
    }
    else
    {
        _isScaled = false;
        _scaledPlanet = scaledPlanet;
    }

    _player = playerEntity;
}

obj_ptr<Planet> Planet::Create(Scene *mainScene, Scene *scaledScene, const obj_ptr<SceneEntity>& playerEntity)
{
    auto scaledPlanet = scaledScene->Create<Planet>(nullptr, playerEntity);
    auto mainPlanet = mainScene->Create<Planet>(scaledPlanet, playerEntity);

    return mainPlanet;
}

void Planet::OnCreated()
{
    mesh = AssetsServer::Get()->LoadMeshAsset("Assets/Models/IcosphereSmooth3.obj");
    auto shader = CreateObject<Shader>(L"Assets/Shaders/Planet.hlsl");
    shader->Compile();
    renderingMaterial = CreateObject<Material>(shader);

    scale = float3{1,1,1} * 60000;
    position = Vector3d{0,0, 1} * 100000 * 1;

    // Create lod 0 chunks
    if (_isScaled)
    {
        for (char i = 0; i < 6; i++)
        {
            AllocateChunk(init_cid(i));
            SubdivideChunk(init_cid(i));
        }
    }
}

void Planet::OnUpdate()
{
    float scaling = _isScaled ? 10000.0f : 1.0f;

    for (const auto& chunkPair : _activeChunks)
    {
        const auto& chunk = chunkPair.second;

        auto lod = cid_to_lod(chunk.ID);

        if (lod >= _maxScaledLodLevel)
            continue;

        double playerDistance = length(position + chunk.position * scaling - _player->position);
        float divisionThreshold = _planetRadius / 1.0f * powf(0.5f, lod);

        if (playerDistance < divisionThreshold)
        {
            if (!chunk.hasChildren)
            {
                SubdivideChunk(chunk.ID);
            }
        }
        else if (playerDistance > divisionThreshold * 2.0f)
        {
            // ToDo check for children' children
            MergeChunk(chunk.ID);
        }
    }
}

static DirectX::XMVECTOR FACE_ROTATIONS[6] =
{
    DirectX::XMQuaternionRotationRollPitchYaw(0, 0, 0),
    DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XM_PIDIV2, 0, 0),
    DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XM_PI, 0, 0),
    DirectX::XMQuaternionRotationRollPitchYaw(-DirectX::XM_PIDIV2, 0, 0),
    DirectX::XMQuaternionRotationRollPitchYaw(0, DirectX::XM_PIDIV2, 0),
    DirectX::XMQuaternionRotationRollPitchYaw(0, -DirectX::XM_PIDIV2, 0),
};

void Planet::RenderEntity(const obj_ptr<NSE::Camera> &camera)
{
    if (!mesh)
    {
        return;
    }

    float scaling = _isScaled ? 1.0f / 10000.0f : 1.0f;



    for (const auto& chunkPair : _activeChunks)
    {
        const auto& chunk = chunkPair.second;

        if (chunk.hasChildren)
            continue;

        auto positionCS = (float3)((position + chunk.position / scaling - camera->position) * scaling);
        // auto sc = scale;

        // positionCS *= scaling;
        // sc *= scaling;

        // auto s = XMLoadFloat3(&sc);
        auto p = XMLoadFloat3(&positionCS);

        auto matrix = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, rotation, p);

        RenderServer::Get()->DrawMesh(chunk.mesh, renderingMaterial, matrix, camera, GetUID());
    }

}

Planet::Chunk Planet::CreateChunk(ChunkID id)
{
    auto lodLevel = cid_to_lod(id);
    auto faceID = cid_to_face(id);
    float radius = _isScaled ? _planetRadius / 10000.0f : _planetRadius;

    Chunk result;

    result.ID = id;
    result.mesh = PlanetMeshTools::CreateChunkMesh(id, _chunkResolution, radius, result.position);

    return result;
}

Planet::Chunk Planet::AllocateChunk(ChunkID id)
{
    auto foundChunk = _activeChunks.find(id);

    if (foundChunk != _activeChunks.end())
        return foundChunk->second;

    if (_inactiveChunks.empty())
    {
        // actually create chunk
        auto result = CreateChunk(id);
        _activeChunks.emplace(id, result);
        return result;
    }

    auto result = _inactiveChunks.back();
    _inactiveChunks.pop_back();
    _activeChunks.emplace(id, result);
    return result;
}

void Planet::DeallocateChunk(ChunkID id)
{

}

void Planet::SubdivideChunk(ChunkID id)
{
    auto chunkIt = _activeChunks.find(id);
    assert(chunkIt != _activeChunks.end());

    for (char i = 0; i < 4; i++)
    {
        auto childID = next_cid(id, i);

        AllocateChunk(childID);
    }

    chunkIt->second.hasChildren = true;
}

void Planet::MergeChunk(ChunkID id)
{

}
