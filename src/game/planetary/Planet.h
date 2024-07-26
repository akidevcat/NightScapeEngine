#ifndef PLANET_H
#define PLANET_H

#include "../../engine/addon/FastNoiseLite.h"
#include "../../engine/entity/VisualMeshEntity.h"
#include "../data/INavigatable.h"

struct PlanetCreationParameters;

class Planet : public NSE::VisualMeshEntity, public INavigatable
{
public:
    using ChunkID = uint32_t;

    struct Chunk
    {
        bool isSetup = false;
        bool isDeallocated = true;
        ChunkID ID{0};
        char lodLevel;
        char faceID;
        NSE::Vector3d position{};
        obj_ptr<NSE::Mesh> mesh = nullptr;
        bool hasChildren = false;
        char childrenMaxLodLevel;
    };

    struct ChunkDrawBufferData
    {
        float chunkScaling;
        float3 chunkPosition;
    };

public:
    explicit Planet(PlanetCreationParameters params, const obj_ptr<Planet>& mainPlanet, const obj_ptr<SceneEntity>& playerEntity);

    static obj_ptr<Planet> Create(PlanetCreationParameters params, NSE::Scene* mainScene, NSE::Scene* scaledScene, const obj_ptr<SceneEntity>& playerEntity);

    void RenderEntity(const NSE_Camera& camera) override;

    const NSE::Vector3d& GetNavigatablePosition() override { return position; }
    const DirectX::XMVECTOR& GetNavigatableRotation() override { return rotation; }
    float4        GetNavigatableColor() override { return float4{0.8, 0.6, 0.9, 1.0}; }
    float          GetNavigatableMasslockRadius() override { return _planetRadius + _planetAtmosphereHeight; }
    bool GetNavigatableShiftSpaceNavigatable() override { return true; }
    bool GetNavigatableShiftSpaceVisibility() override { return true; }
    const std::string& GetNavigatableName() override { return _name; }
    obj_ptr<NSE::Mesh> GetNavigatablePreviewMesh() override { return nullptr; }
    float GetNavigatableHealth() override { return -100; }

    static char cid_to_face(ChunkID id)
    {
        return (char)(id >> (32 - 3));
    }

    static char cid_to_lod(ChunkID id)
    {
        return (char)((id >> (32 - 8)) & (0b00011111));
    }

    static ChunkID next_cid(ChunkID parentID, char subdivisionQuarter)
    {
        // Increase lod level by one
        parentID += 0b1 << 24;
        // Encode subdivision quarter
        char lodLevel = cid_to_lod(parentID);
        parentID &= ~(0b11 << ((lodLevel - 1) * 2)); // Clear subdivision quarter value
        parentID |= (subdivisionQuarter & 0b11) << ((lodLevel - 1) * 2); // Set subdivision quarter value
        return parentID;
    }

    static ChunkID previous_cid(ChunkID childID)
    {
        // Decrease lod level by one
        childID -= 0b1 << 24;
        // Clear subdivision quarter
        char lodLevel = cid_to_lod(childID);
        childID &= ~(0b11 << (lodLevel * 2)); // Clear subdivision quarter value
        return childID;
    }

    static ChunkID init_cid(char faceID)
    {
        ChunkID result = 0;

        result |= (faceID & 0b111) << (32 - 3);

        return result;
    }

protected:
    Chunk CreateChunk(ChunkID id);
    Chunk& AllocateChunk(ChunkID id);
    Chunk& DeallocateChunk(ChunkID id);
    Chunk& DeallocateChunkNow(ChunkID id);
    void SetupChunk(Chunk& chunk, ChunkID id);
    void ClearChunk(Chunk& chunk);
    void SubdivideChunk(ChunkID id);
    void MergeChunk(ChunkID id);

    void OnCreated() override;
    void OnUpdate() override;

    void RenderEntityScaled(const NSE_Camera& camera);

private:
    std::string _name = "Planet";
    bool _isScaled = false;
    obj_ptr<Planet> _mainPlanet = nullptr;
    obj_ptr<SceneEntity> _player = nullptr;
    int _maxLodLevel = 10; // Should be less than 14
    int _maxScaledLodLevel = 8;
    int _chunkResolution = 8;
    float _planetRadius = 60000.0f;
    float _planetTerrainMaxHeight = 4000.0f;
    float _planetAtmosphereHeight = 4000.0f;
    float4 _planetPrimaryColor = {1,1,1,1};

    FastNoiseLite _surfaceNoise;

    NSE_GraphicsBuffer _chunkDrawBuffer = nullptr;
    obj_ptr<VisualMeshEntity> _atmosphereEntity = nullptr;
    // NSE_Material _scaledMaterial = nullptr;
    // NSE_Material _mainMaterial = nullptr;

    std::vector<Chunk> _inactiveChunks;
    std::unordered_map<ChunkID, Chunk> _activeChunks;
    std::vector<ChunkID> _deallocatingChunks;
};

#endif //PLANET_H
