#ifndef MATERIAL_H
#define MATERIAL_H

#include "../obj_ptr.h"
#include "ConstBufferData.h"
#include "Shader.h"
#include "../entity/Object.h"

#define NSE_Material obj_ptr<NSE::Material>

namespace NSE
{
    class Material : public Object
    {
    public:
        explicit Material(const NSE_Shader& shader);
        ~Material();

        bool SetVertexVar(size_t pUid, void* value, size_t valueSize);
        bool SetPixelVar(size_t pUid, void* value, size_t valueSize);
        void SetVSResource(size_t uid, ID3D11ShaderResourceView* resource);
        void SetPSResource(size_t uid, ID3D11ShaderResourceView* resource);

        bool UploadMaterialProperties(ID3D11DeviceContext *context);
        void UploadDrawProperties(ConstBufferData* drawProps);
        void UploadGlobalProperties(ConstBufferData* globalProps);
        void UploadAllProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps, ConstBufferData* drawProps);
        void EnumerateBuffers(_Out_ ID3D11Buffer* vsBuffers[3], _Out_ int& vsBuffersLength,
                              _Out_ ID3D11Buffer* psBuffers[3], _Out_ int& psBuffersLength,
                              ConstBufferData* globalProps, ConstBufferData* drawProps) const;

    public:
        [[nodiscard]] NSE_Shader GetShader() const { return _shader; }
        [[nodiscard]] ConstBufferData* GetVSMaterialProps() const { return _vsMaterialProps; }
        [[nodiscard]] ConstBufferData* GetPSMaterialProps() const { return _psMaterialProps; }

    private:
        NSE_Shader       _shader = nullptr;
        ConstBufferData* _vsMaterialProps = nullptr;
        ConstBufferData* _psMaterialProps = nullptr;
    };
}

#define NSE_Material obj_ptr<NSE::Material>

#endif //MATERIAL_H
