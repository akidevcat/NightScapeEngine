#ifndef SHADER_H
#define SHADER_H

#include "VertexShader.h"
#include "PixelShader.h"

class Shader
{
public:
    Shader() = delete;
    explicit Shader(const wchar_t* path);
    Shader(const wchar_t* vsPath, const wchar_t* psPath);
    ~Shader();

    void Release();
    bool Compile(ID3D11Device* device);

public:
    [[nodiscard]] VertexShader* GetVertexShader() const { return _vShader; }
    [[nodiscard]] PixelShader* GetPixelShader() const { return _pShader; }
    [[nodiscard]] bool IsCompiled() const { return _isCompiled; }

private:
    bool _isCompiled = false;
    VertexShader* _vShader = nullptr;
    PixelShader* _pShader = nullptr;
};

#endif //SHADER_H
