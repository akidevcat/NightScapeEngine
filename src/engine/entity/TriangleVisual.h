#ifndef TRIANGLEVISUAL_H
#define TRIANGLEVISUAL_H

#include "VisualEntity.h"
#include "../data/Mesh.h"
#include "../servers/TimeServer.h"

#define NSE_TriangleVisual obj_ptr<NSE::TriangleVisual>

namespace NSE
{
    class Camera;

    class TriangleVisual : public VisualEntity
    {
    public:
        explicit TriangleVisual();
        ~TriangleVisual() override;

        void OnUpdate() override;
        void RenderEntity(const obj_ptr<Camera>& camera) override;

    private:
        NSE_Mesh _mesh = nullptr;
    };
}

#endif //TRIANGLEVISUAL_H
