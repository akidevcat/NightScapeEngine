#ifndef PROGRESSBARVISUAL_H
#define PROGRESSBARVISUAL_H

#include "SpriteVisual.h"
#include "VisualMeshEntity.h"

namespace NSE
{
    class ProgressBarVisual : public VisualEntity
    {
    public:
        ProgressBarVisual();

        void RenderEntity(const obj_ptr<Camera> &camera) override;

    public:
        SpriteTexture sprite{};
        bool isScreenSpace = false;
        bool invertX = false;
        bool invertY = false;
        float progress = 0.5f;
        float4 color = {1,1,1,1};
        float4 foregroundColor = {1,1,1,1};
        float4 backgroundColor = {0.5,0.5,0.5,1};

    private:
        NSE_Mesh _mesh = nullptr;
        static NSE_Shader _shader;
    };
}

#endif //PROGRESSBARVISUAL_H
