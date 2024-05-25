#ifndef MATERIAL_H
#define MATERIAL_H

#include "ConstBufferData.h"
#include "Shader.h"

class Material
{
public:
    explicit Material(Shader* shader);
    ~Material();

    bool SetVertexVar(size_t pUid, void* value, size_t valueSize);
    bool SetPixelVar(size_t pUid, void* value, size_t valueSize);

    bool UploadMaterialProperties(ID3D11DeviceContext *context);
    void UploadShaderProperties(ID3D11DeviceContext *context);
    void UploadAllProperties(ID3D11DeviceContext *context);



public:
    [[nodiscard]] Shader* GetShader() const { return _shader; }
    [[nodiscard]] ConstBufferData* GetVSMaterialProps() const { return _vsMaterialProps; }
    [[nodiscard]] ConstBufferData* GetPSMaterialProps() const { return _psMaterialProps; }

private:
    Shader*          _shader = nullptr;
    ConstBufferData* _vsMaterialProps = nullptr;
    ConstBufferData* _psMaterialProps = nullptr;
};

#endif //MATERIAL_H
