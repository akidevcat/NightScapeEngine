#ifndef SHADERINPUTSDESCRIPTION_H
#define SHADERINPUTSDESCRIPTION_H

#include <d3d11shader.h>
#include <unordered_map>

class ShaderInputsDescription
{
public:
    explicit ShaderInputsDescription(ID3D11ShaderReflection* reflection);
    ~ShaderInputsDescription() = default;

    bool GetDescription(size_t nameID, D3D11_SHADER_INPUT_BIND_DESC& description);
    std::unordered_map<size_t, D3D11_SHADER_INPUT_BIND_DESC>::iterator begin() { return _bindDescriptions.begin(); }
    std::unordered_map<size_t, D3D11_SHADER_INPUT_BIND_DESC>::iterator end() { return _bindDescriptions.end(); }

private:
    std::unordered_map<size_t, D3D11_SHADER_INPUT_BIND_DESC>  _bindDescriptions = {}; // ToDo maybe split into samplers, cbuffers and textures?
};

#endif //SHADERINPUTSDESCRIPTION_H