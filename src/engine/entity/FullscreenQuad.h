#ifndef FULLSCREENQUAD_H
#define FULLSCREENQUAD_H

#include "VisualEntity.h"

class FullscreenQuad : public VisualEntity
{
public:
    explicit FullscreenQuad(ID3D11Device* device);
    ~FullscreenQuad();

    void RenderEntity(RenderServer* render, TimeServer* time) override;

private:
    Mesh* _mesh = nullptr;
};

#endif //FULLSCREENQUAD_H
