#pragma once
#include "../../engine/math/Math.h"
#include "../../engine/data/Mesh.h"

class INavigatable
{
public:
    INavigatable() = default;
    virtual ~INavigatable() = default;

    virtual const NSE::Vector3d& GetNavigatablePosition() = 0;
    virtual const DirectX::XMVECTOR& GetNavigatableRotation() = 0;
    virtual float4 GetNavigatableColor() = 0;
    virtual float GetNavigatableMasslockRadius() = 0;
    virtual bool GetNavigatableShiftSpaceNavigatable() = 0;
    virtual bool GetNavigatableShiftSpaceVisibility() = 0;
    virtual const std::string& GetNavigatableName() = 0;
    virtual float GetNavigatableHealth() = 0;
    virtual obj_ptr<NSE::Mesh> GetNavigatablePreviewMesh() = 0;

};
