#ifndef PLANET_H
#define PLANET_H

#include "../../engine/entity/VisualMeshEntity.h"

class Planet : public NSE::VisualMeshEntity
{
public:
    using ChunkID = uint32_t;

    // struct ChunkID
    // {
    //     char FaceID = 0;
    //     int LodID = 0;
    //
    //     explicit ChunkID(char faceID);
    // };

    struct Chunk
    {
        ChunkID ID{0};
        NSE::Vector3d position{};
        obj_ptr<NSE::Mesh> mesh = nullptr;
        bool hasChildren = false;
    };

public:
    explicit Planet(const obj_ptr<Planet>& scaledPlanet, const obj_ptr<SceneEntity>& playerEntity);

    static obj_ptr<Planet> Create(NSE::Scene* mainScene, NSE::Scene* scaledScene, const obj_ptr<SceneEntity>& playerEntity);

    void RenderEntity(const NSE_Camera& camera) override;

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

    static ChunkID init_cid(char faceID)
    {
        ChunkID result = 0;

        result |= (faceID & 0b111) << (32 - 3);

        return result;
    }

protected:
    Chunk CreateChunk(ChunkID id);
    Chunk AllocateChunk(ChunkID id);
    void DeallocateChunk(ChunkID id);
    void SubdivideChunk(ChunkID id);
    void MergeChunk(ChunkID id);

    void OnCreated() override;
    void OnUpdate() override;

private:
    bool _isScaled = false;
    obj_ptr<Planet> _scaledPlanet = nullptr;
    obj_ptr<SceneEntity> _player = nullptr;
    int _maxLodLevel = 8; // Should be less than 14
    int _maxScaledLodLevel = 4;
    int _chunkResolution = 16;
    float _planetRadius = 60000.0f;

    std::vector<Chunk> _inactiveChunks;
    std::unordered_map<ChunkID, Chunk> _activeChunks;
};

#endif //PLANET_H
