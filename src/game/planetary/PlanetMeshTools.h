#ifndef PLANETMESHTOOLS_H
#define PLANETMESHTOOLS_H

#include "../../engine/obj_ptr.h"
#include "../../engine/data/Mesh.h"
#include "../../engine/math/Math.h"

class PlanetMeshTools
{
public:
    static obj_ptr<NSE::Mesh> CreateChunkMesh(uint32_t chunkID, int resolution, float radius, NSE::Vector3d& chunkPivot);
};

#endif //PLANETMESHTOOLS_H
