#pragma once

#include "../../engine/entity/VisualMeshEntity.h"
#include "../data/INavigatable.h"
#include "../data/ITargetable.h"

class DroidEntity : public NSE::VisualMeshEntity, public INavigatable
{
public:
    DroidEntity();

    void OnUpdate() override;

    const NSE::Vector3d& GetNavigatablePosition() override { return position; }
    const DirectX::XMVECTOR& GetNavigatableRotation() override { return rotation; }
    float4 GetNavigatableColor() override { return {1,0.8,0.8,1}; }
    float GetNavigatableMasslockRadius() override { return 10.0f; }
    bool GetNavigatableShiftSpaceNavigatable() override { return false; }
    bool GetNavigatableShiftSpaceVisibility() override { return false; }
    const std::string& GetNavigatableName() override { return _name; }
    obj_ptr<NSE::Mesh> GetNavigatablePreviewMesh() override { return mesh; }
    float GetNavigatableHealth() override { return _health; }

private:
    std::string _name = "APD mk1";
    float _health = 100.0f;
    bool _isApproachingPhase = false;
    float _approachThresholdDistance = 30.0f;
    float _escapeThresholdDistance = 200.0f;
};
