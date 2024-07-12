#ifndef VISUALMESHENTITY_H
#define VISUALMESHENTITY_H
#include "Camera.h"
#include "VisualEntity.h"
#include "../data/Mesh.h"

namespace NSE
{
    class VisualMeshEntity : public VisualEntity
    {
    public:
        VisualMeshEntity();
        ~VisualMeshEntity() override;

        void RenderEntity(const NSE_Camera& camera) override;

    public:
        NSE_Mesh mesh;
        float renderingScaling = 1.0f;
    };
}


#endif //VISUALMESHENTITY_H
