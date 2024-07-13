#include "Planet.h"

#include <iostream>

#include "PlanetMeshTools.h"
#include "../../engine/servers/AssetServer.h"
#include "../../engine/servers/RenderServer.h"
#include "../data/PlanetCreationParameters.h"

using namespace NSE;

Planet::Planet(PlanetCreationParameters params, const obj_ptr<Planet> &mainPlanet, const obj_ptr<SceneEntity>& playerEntity)
{
    _planetRadius = params.radius;
    _planetAtmosphereHeight = params.atmosphereHeight;
    _planetTerrainMaxHeight = params.terrainMaxHeight;

    if (mainPlanet)
    {
        _isScaled = true;
        _mainPlanet = mainPlanet;
    }
    else
    {
        _isScaled = false;
    }

    _player = playerEntity;
}

obj_ptr<Planet> Planet::Create(PlanetCreationParameters params, Scene *mainScene, Scene *scaledScene, const obj_ptr<SceneEntity>& playerEntity)
{
    auto mainPlanet = mainScene->Create<Planet>(params, nullptr, playerEntity);
    auto scaledPlanet = scaledScene->Create<Planet>(params, mainPlanet, playerEntity);

    return mainPlanet;
}

void Planet::OnCreated()
{
    if (_isScaled)
    {
        // required for queue
        renderingMaterial = _mainPlanet->renderingMaterial;
        // create atmosphere entity
        auto scene = GetScene();
        _atmosphereEntity = scene->Create<VisualMeshEntity>();
        auto atmShader = CreateObject<Shader>(L"Assets/Shaders/PlanetAtmosphere.hlsl");
        atmShader->Compile();
        _atmosphereEntity->renderingMaterial = CreateObject<Material>(atmShader);
        _atmosphereEntity->renderingMaterial->MakeTransparent();
        float r = _planetRadius + _planetAtmosphereHeight;
        _atmosphereEntity->renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_AtmosphereRadius"), r / 1000.0f);
        _atmosphereEntity->renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_PlanetRadius"), _planetRadius / 1000.0f);
        _atmosphereEntity->mesh = AssetsServer::Get()->LoadMeshAsset("Assets/Models/IcosphereSmooth5_Inverted.obj");
        _atmosphereEntity->scale = float3{1,1,1} * r;
        _atmosphereEntity->renderingScaling = 1.0f / 1000.0f;
        // _atmosphereEntity->scale = float3{0.5,0.5,0.5};
        return;
    }

    mesh = AssetsServer::Get()->LoadMeshAsset("Assets/Models/IcosphereSmooth3.obj");
    auto shader = CreateObject<Shader>(L"Assets/Shaders/Planet.hlsl");
    shader->Compile();
    renderingMaterial = CreateObject<Material>(shader);

    // renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_IsScaledInstance"), 0.0f);
    // renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_Scaling"), 1.0f);
    // renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_IsScaledInstance"), 1.0f);
    // renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_Scaling"), 1000.0f);

    _chunkDrawBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Constant, sizeof(ChunkDrawBufferData));
    renderingMaterial->SetConstantBuffer(ShaderUtils::PropertyToID("ChunkDrawBuffer"), _chunkDrawBuffer);
    renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_AtmosphereRadius"), (_planetRadius + _planetAtmosphereHeight) / 1000.0f);
    renderingMaterial->SetFloat(ShaderUtils::PropertyToID("_PlanetRadius"), _planetRadius / 1000.0f);

    // scale = float3{1,1,1} * 60000;
    position = Vector3d{0,0, 1} * 100000 * 1;

    // Create lod 0 chunks
    for (char i = 0; i < 6; i++)
    {
        auto id = init_cid(i);
        SetupChunk(AllocateChunk(id), id);
    }
}

void Planet::OnUpdate()
{
    if (_isScaled)
    {
        _atmosphereEntity->position = _mainPlanet->position;
        return;
    }

    for (const auto& chunkPair : _activeChunks)
    {
        const auto& chunk = chunkPair.second;

        float scaling = (chunk.lodLevel <= _maxScaledLodLevel) ? 1000.0f : 1.0f;

        auto lod = cid_to_lod(chunk.ID);

        if (lod >= _maxLodLevel)
            continue;

        auto flCanDivide = chunk.childrenMaxLodLevel - chunk.lodLevel == 0;
        auto flCanMerge = chunk.childrenMaxLodLevel - chunk.lodLevel == 1;

        if (!flCanDivide && !flCanMerge)
        {
            continue;
        }

        double playerDistance = length(position + chunk.position * scaling - _player->position);
        float divisionThreshold = _planetRadius / 0.4f * powf(0.5f, lod);

        if (playerDistance < divisionThreshold)
        {
            if (flCanDivide)
            {
                SubdivideChunk(chunk.ID);
            }
        }
        else if (playerDistance > divisionThreshold * 2.0f)
        {
            if (flCanMerge)
            {
                MergeChunk(chunk.ID);
            }
        }
    }

    for (auto& it : _deallocatingChunks)
    {
        DeallocateChunkNow(it);
    }

    _deallocatingChunks.clear();
}

void Planet::RenderEntityScaled(const obj_ptr<NSE::Camera> &camera)
{
    float scaling = 1.0f / 1000.0f;

    for (const auto& chunkPair : _activeChunks)
    {
        const auto& chunk = chunkPair.second;

        if (chunk.lodLevel > _maxScaledLodLevel)
        {
            continue; // This is a non-scaled chunk
        }

        // if (chunk.childrenMaxLodLevel != chunk.lodLevel && chunk.lodLevel != _maxScaledLodLevel)
        if (chunk.childrenMaxLodLevel != chunk.lodLevel)
            continue;

        auto positionCS = (float3)((position + chunk.position / scaling - camera->position) * scaling);
        auto p = XMLoadFloat3(&positionCS);
        auto matrix = XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, rotation, p);

        auto drawData = _chunkDrawBuffer->As<ChunkDrawBufferData>();
        drawData->chunkScaling = 1000.0f;
        drawData->chunkPosition = (float3)chunk.position;
        _chunkDrawBuffer->Upload();

        // RenderServer::Get()->DrawMesh(chunk.mesh, renderingMaterial, matrix, camera, GetUID());
        RenderServer::Get()->DrawMesh(chunk.mesh, renderingMaterial, matrix, camera, GetUID());
    }
}

void Planet::RenderEntity(const obj_ptr<NSE::Camera> &camera)
{
    if (_isScaled)
    {
        _mainPlanet->RenderEntityScaled(camera);
        return;
    }

    for (const auto& chunkPair : _activeChunks)
    {
        const auto& chunk = chunkPair.second;

        if (chunk.lodLevel <= _maxScaledLodLevel)
        {
            continue; // This is a scaled chunk
        }

        if (chunk.childrenMaxLodLevel != chunk.lodLevel)
            continue;

        auto positionCS = (float3)(position + chunk.position - camera->position);
        auto p = XMLoadFloat3(&positionCS);
        auto matrix = XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, rotation, p);

        auto drawData = _chunkDrawBuffer->As<ChunkDrawBufferData>();
        drawData->chunkScaling = 1.0f;
        drawData->chunkPosition = (float3)chunk.position;
        _chunkDrawBuffer->Upload();

        // RenderServer::Get()->DrawMesh(chunk.mesh, renderingMaterial, matrix, camera, GetUID());
        RenderServer::Get()->DrawMesh(chunk.mesh, renderingMaterial, matrix, camera, GetUID());
    }

}

Planet::Chunk Planet::CreateChunk(ChunkID id)
{
    auto lodLevel = cid_to_lod(id);
    auto faceID = cid_to_face(id);

    Chunk result;

    result.isSetup = false;
    result.mesh = PlanetMeshTools::CreateChunkMesh(_chunkResolution);
    result.position = {};

    return result;
}

Planet::Chunk& Planet::AllocateChunk(ChunkID id)
{
    auto foundChunk = _activeChunks.find(id);

    if (foundChunk != _activeChunks.end())
        return foundChunk->second;

    if (_inactiveChunks.empty())
    {
        // actually create chunk
        auto result = CreateChunk(id);
        result.isDeallocated = false;
        auto it = _activeChunks.emplace(id, result);
        return it.first->second;
    }

    auto result = _inactiveChunks.back();
    result.isDeallocated = false;
    _inactiveChunks.pop_back();
    auto it = _activeChunks.emplace(id, result);
    return it.first->second;
}

Planet::Chunk& Planet::DeallocateChunk(ChunkID id)
{
    auto chunkIt = _activeChunks.find(id);
    assert(chunkIt != _activeChunks.end());

    chunkIt->second.isDeallocated = true;
    auto& chunk = _deallocatingChunks.emplace_back(chunkIt->second.ID);

    return chunkIt->second;
}

Planet::Chunk& Planet::DeallocateChunkNow(ChunkID id)
{
    auto chunkIt = _activeChunks.find(id);
    assert(chunkIt != _activeChunks.end());

    auto& it = _inactiveChunks.emplace_back(chunkIt->second);
    _activeChunks.erase(chunkIt);

    it.isDeallocated = true;

    return it;
}

void Planet::SetupChunk(Chunk &chunk, ChunkID id)
{
    chunk.ID = id;
    chunk.lodLevel = cid_to_lod(id);
    chunk.faceID = cid_to_face(id);
    chunk.hasChildren = false;

    float scaling = (chunk.lodLevel <= _maxScaledLodLevel) ? 1.0f / 1000.0f : 1.0f;

    PlanetMeshTools::SetupChunkMesh(chunk.mesh, id, _chunkResolution, _planetRadius * scaling, _planetTerrainMaxHeight * scaling, chunk.position);

    chunk.childrenMaxLodLevel = chunk.lodLevel;
    chunk.isSetup = true;
}

void Planet::ClearChunk(Chunk &chunk)
{
    assert(chunk.isDeallocated);

    chunk.position = {};
    chunk.lodLevel = 0;
    chunk.faceID = 0;
    chunk.ID = UINT_MAX;
    chunk.childrenMaxLodLevel = 0;
}

void Planet::SubdivideChunk(ChunkID id)
{
    auto chunkIt = _activeChunks.find(id);
    assert(chunkIt != _activeChunks.end());

    for (char i = 0; i < 4; i++)
    {
        auto childID = next_cid(id, i);

        SetupChunk(AllocateChunk(childID), childID);
    }

    auto lod = cid_to_lod(id);
    auto maxLod = (char)(lod + 1);
    auto parentID = id;

    do
    {
        chunkIt = _activeChunks.find(parentID);
        assert(chunkIt != _activeChunks.end());
        chunkIt->second.hasChildren = true;
        chunkIt->second.childrenMaxLodLevel = maxLod;
        lod--;
        parentID = previous_cid(parentID);
    }
    while (lod > 0);
}

void Planet::MergeChunk(ChunkID id)
{
    auto chunkIt = _activeChunks.find(id);
    assert(chunkIt != _activeChunks.end());

    auto& chunk = chunkIt->second;

    assert(chunk.childrenMaxLodLevel > chunk.lodLevel);

    for (char i = 0; i < 4; i++)
    {
        auto childID = next_cid(id, i);

        ClearChunk(DeallocateChunk(childID));
    }

    auto lod = cid_to_lod(id);
    auto maxLod = lod;
    auto parentID = id;

    do
    {
        chunkIt = _activeChunks.find(parentID);
        assert(chunkIt != _activeChunks.end());
        chunkIt->second.hasChildren = true;
        chunkIt->second.childrenMaxLodLevel = maxLod;
        lod--;
        parentID = previous_cid(parentID);
    }
    while (lod > 0);
}
