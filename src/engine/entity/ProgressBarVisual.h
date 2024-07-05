#ifndef PROGRESSBARVISUAL_H
#define PROGRESSBARVISUAL_H

#include "VisualMeshEntity.h"

namespace NSE
{
    class ProgressBarVisual : public VisualMeshEntity
    {
    public:
        ProgressBarVisual(const float3 *path, int pathLength);

        void RenderEntity(const obj_ptr<Camera> &camera) override;

    public:
        float progress = 0.5f;
        float4 foregroundColor = {1,1,1,1};
        float4 backgroundColor = {0.5,0.5,0.5,1};

    private:
        static NSE_Shader _shader;
    };
}

#endif //PROGRESSBARVISUAL_H
