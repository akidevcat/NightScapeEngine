#ifndef FULLSCREENQUAD_H
#define FULLSCREENQUAD_H

#include "VisualEntity.h"

#define NSE_QuadVisual obj_ptr<NSE::QuadVisual>

namespace NSE
{
    class Mesh;
    class Camera;

    class QuadVisual : public VisualEntity
    {
    public:
        explicit QuadVisual();
        ~QuadVisual() override;

        void RenderEntity(const obj_ptr<Camera>& camera) override;

    private:
        obj_ptr<Mesh> _mesh;
    };
}

#endif //FULLSCREENQUAD_H
