#ifndef STARSCAMERA_H
#define STARSCAMERA_H

#include "../engine/entity/Camera.h"
#include "../engine/entity/QuadVisual.h"

class StarsCamera : public NSE::Camera
{
public:
    StarsCamera(const NSE_Camera& parentCamera, const NSE_RenderTexture& targetRT);
    ~StarsCamera();

    void OnUpdate() override;
private:
    NSE_Camera _parentCamera = nullptr;
    NSE::Scene* _starsScene = nullptr;
    std::vector<NSE_QuadVisual> _largeStars = {};
    NSE_Material _quadMaterial;
    NSE_Shader _quadShader;
};

#endif //STARSCAMERA_H