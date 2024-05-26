#ifndef MATERIAL_H
#define MATERIAL_H

#include "ConstBufferData.h"
#include "Shader.h"

class Material
{
public:
    explicit Material(ID3D11Device* device, Shader* shader);
    ~Material();

    bool SetVertexVar(size_t pUid, void* value, size_t valueSize);
    bool SetPixelVar(size_t pUid, void* value, size_t valueSize);

    bool UploadMaterialProperties(ID3D11DeviceContext *context);
    void UploadDrawProperties(ID3D11DeviceContext *context);
    void UploadGlobalProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps);
    void UploadAllProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps);
    void EnumerateBuffers(_Out_ ID3D11Buffer* vsBuffers[3], _Out_ int& vsBuffersLength,
    _Out_ ID3D11Buffer* psBuffers[3], _Out_ int& psBuffersLength, ConstBufferData* globalProps) const;

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
