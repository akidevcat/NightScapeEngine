#ifndef STARSCAMERA_H
#define STARSCAMERA_H

#include "ShipController.h"
#include "StarDustParticles.h"
#include "../engine/entity/Camera.h"
#include "../engine/entity/QuadVisual.h"

class StarsCamera : public NSE::Camera
{
public:
    explicit StarsCamera(const NSE_Camera& parentCamera, const obj_ptr<ShipController>& controller);
    ~StarsCamera() override;

    void OnUpdate() override;

    NSE::Scene* GetStarsScene() { return _starsScene; }
private:
    NSE_Camera _parentCamera = nullptr;
    NSE::Scene* _starsScene = nullptr;
    std::vector<NSE_QuadVisual> _largeStars = {};
    NSE_Material _quadMaterial;
    NSE_Shader _quadShader;
    obj_ptr<StarDustParticles> _particles = nullptr;
    obj_ptr<ShipController> _shipController = nullptr;
};

#endif //STARSCAMERA_H
