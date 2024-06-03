#ifndef SHADER_H
#define SHADER_H

#include "ConstantBufferData.h"
#include "../obj_ptr.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "../entity/Object.h"

#define NSE_Shader obj_ptr<NSE::Shader>

namespace NSE
{
    class Shader : public Object
    {
    public:
        Shader() = delete;
        explicit Shader(const wchar_t* path);
        Shader(const wchar_t* vsPath, const wchar_t* psPath);
        ~Shader() override;

        void Release();
        bool Compile();

        // void UploadDrawProperties(ConstBufferData* drawProps);
        // void UploadGlobalProperties(ConstBufferData* globalProps);

        [[nodiscard]] VertexShader* GetVertexShader() const { return _vShader; }
        [[nodiscard]] PixelShader* GetPixelShader() const { return _pShader; }
        [[nodiscard]] bool IsCompiled() const { return _isCompiled; }

    private:
        bool _isCompiled = false;
        VertexShader* _vShader = nullptr;
        PixelShader* _pShader = nullptr;
    };
}

#endif //SHADER_H
