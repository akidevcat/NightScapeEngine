#ifndef TRIANGLEVISUAL_H
#define TRIANGLEVISUAL_H

#include "VisualEntity.h"
#include "../data/Mesh.h"
#include "../servers/TimeServer.h"

class TriangleVisual : public VisualEntity
{
public:
    explicit TriangleVisual(ID3D11Device* device);
    ~TriangleVisual();

    void RenderEntity(RenderServer* render, TimeServer* time, Camera *camera) override;
    
private:
    Mesh* _mesh = nullptr;
};

#endif //TRIANGLEVISUAL_H
