#ifndef SHIPCONTROLLER_H
#define SHIPCONTROLLER_H
#include "StarDustParticles.h"
#include "ShipMarkersVisual.h"
#include "ShipRadarController.h"
#include "../engine/data/Light.h"
#include "../engine/entity/ProgressBarVisual.h"
#include "../engine/entity/VisualMeshEntity.h"
#include "../engine/entity/SpriteVisual.h"
#include "../engine/entity/TextVisual.h"
#include "data/INavigatable.h"
#include "ship/PulseWeaponEntity.h"

class ShipController : public NSE::VisualMeshEntity
{
public:
    explicit ShipController(NSE::Scene* scene, float screenAspect);
    ~ShipController() override;

    void OnCreated() override;
    void OnUpdate() override;

    NSE_Camera GetCamera() { return _camera; }
    obj_ptr<ShipRadarController> GetRadar() { return _shipRadar; }

    [[nodiscard]] NSE::Vector3d GetShipVelocity() const { return _shipVelocity; }
    [[nodiscard]] obj_ptr<INavigatable> GetNavTarget() const { return _navTarget; }

private:
    void UpdateInfoText();

    obj_ptr<INavigatable> GetClosestMasslocked(double& outSurfaceDistance);
    void SetInfoText(const std::string& text, float textTimeout = 1.0f);

public:
    float shipSpeed = 3.0f;
    float shipRotationSpeed = 1.0f;

private:
    float _integrity = 100.0f;
    float _fuel = 100.0f;
    float _exposure = 100.0f;

    bool _isShipDragEnabled = true;
    bool _isShiftSpaceActive = false;
    float _shiftSpaceActivationTimer = 0.0f;
    const float _shiftSpaceActivationTimeout = 3.0f;
    // obj_ptr<INavigatable> _shiftSpaceTarget = nullptr;

    obj_ptr<INavigatable> _navTarget = nullptr;

    float _textTimeout = 0.0f;
    float _camMomentumX = 0;
    float _camMomentumY = 0;
    float _camMomentumR = 0;
    float _speedMomentum = 0;
    NSE::Vector3d _shipVelocity = {};

    NSE_Camera _camera = nullptr;
    NSE_Light _cockpitLight = nullptr;
    obj_ptr<ShipRadarController> _shipRadar = nullptr;
    obj_ptr<NSE::ProgressBarVisual> _fuelBar = nullptr;
    obj_ptr<NSE::ProgressBarVisual> _integrityBar = nullptr;
    obj_ptr<NSE::ProgressBarVisual> _exposureBar = nullptr;
    obj_ptr<NSE::SpriteVisual> _crosshair = nullptr;
    obj_ptr<NSE::TextVisual> _infoText = nullptr;
    obj_ptr<NSE::TextVisual> _targetText = nullptr;
    obj_ptr<NSE::TextVisual> _targetHealthText = nullptr;
    obj_ptr<NSE::VisualMeshEntity> _targetMesh = nullptr;
    obj_ptr<NSE::ProgressBarVisual> _targetHealthBar = nullptr;
    obj_ptr<ShipMarkersVisual> _markers = nullptr;
    obj_ptr<StarDustParticles> _dustParticles = nullptr;

    obj_ptr<PulseWeaponEntity> _weapon = nullptr;
 };

#endif //SHIPCONTROLLER_H
