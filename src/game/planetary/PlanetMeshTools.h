#ifndef PLANETMESHTOOLS_H
#define PLANETMESHTOOLS_H

#include "../../engine/obj_ptr.h"
#include "../../engine/data/Mesh.h"
#include "../../engine/math/Math.h"

class FastNoiseLite;

class PlanetMeshTools
{
public:
    static obj_ptr<NSE::Mesh> CreateChunkMesh(int resolution);
    static void SetupChunkMesh(const obj_ptr<NSE::Mesh>& mesh, uint32_t chunkID, int resolution, float radius, float maxHeight, NSE::Vector3d& chunkPivot, const FastNoiseLite& noise);
};

#endif //PLANETMESHTOOLS_H
