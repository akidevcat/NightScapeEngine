#include "VisualMeshEntity.h"
#include "../servers/RenderServer.h"

NSE::VisualMeshEntity::VisualMeshEntity()
{

}

NSE::VisualMeshEntity::~VisualMeshEntity()
{

}

void NSE::VisualMeshEntity::RenderEntity(const NSE_Camera &camera)
{
    if (!mesh)
    {
        return;
    }

    RenderServer::Get()->DrawMesh(mesh, renderingMaterial, GetModelMatrix(camera->position), camera, GetUID());
}
