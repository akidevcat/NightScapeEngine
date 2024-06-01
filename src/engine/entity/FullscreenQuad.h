#ifndef FULLSCREENQUAD_H
#define FULLSCREENQUAD_H

#include "VisualEntity.h"

#define NSE_FullscreenQuad obj_ptr<NSE::FullscreenQuad>

namespace NSE
{
    class Mesh;
    class Camera;

    class FullscreenQuad : public VisualEntity
    {
    public:
        explicit FullscreenQuad();
        ~FullscreenQuad() override;

        void RenderEntity(const obj_ptr<Camera>& camera) override;

    private:
        obj_ptr<Mesh> _mesh;
    };
}

#endif //FULLSCREENQUAD_H
