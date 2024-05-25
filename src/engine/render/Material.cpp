#include "Material.h"

#include <iostream>

Material::Material(Shader* shader)
{
    _shader = shader;
    // ToDo Assert

    if (!shader->IsCompiled())
    {
        std::cerr << "Attempt to create a material with a non-compiled shader" << std::endl;
        return;
    }

    auto vMatProps = shader->GetVertexShader()->GetMaterialPropsLookup();
    auto pMatProps = shader->GetPixelShader()->GetMaterialPropsLookup();

    if (vMatProps)
    {
        _vsMaterialProps = new ConstBufferData{vMatProps->id, vMatProps->uid, vMatProps->description, vMatProps->description.Size};
        _vsMaterialProps->vDesc = vMatProps->vDesc;
    }
    if (pMatProps)
    {
        _psMaterialProps = new ConstBufferData{pMatProps->id, pMatProps->uid, pMatProps->description, pMatProps->description.Size};
        _psMaterialProps->vDesc = pMatProps->vDesc;
    }
}

Material::~Material()
{
    delete _vsMaterialProps;
    delete _psMaterialProps;
}

bool Material::SetVertexVar(size_t pUid, void *value, size_t valueSize)
{
    if (!_vsMaterialProps)
        return false;

    if (!_vsMaterialProps->SetVar(pUid, value, valueSize))
    {
        return false;
    }

    _isDirty = true;
    return true;
}

bool Material::SetPixelVar(size_t pUid, void *value, size_t valueSize)
{
    if (!_psMaterialProps)
        return false;

    if (!_psMaterialProps->SetVar(pUid, value, valueSize))
    {
        return false;
    }

    _isDirty = true;
    return true;
}

bool Material::Upload()
{
    if (!_isDirty)
    {
        return true;
    }



    _isDirty = false;
    return true;
}