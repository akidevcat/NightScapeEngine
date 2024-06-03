#ifndef SHADERINPUTSDATA_H
#define SHADERINPUTSDATA_H
#include <unordered_map>

#include "PixelShader.h"

class ShaderInputsData
{
public:
    ShaderInputsData() = default;
    ~ShaderInputsData() = default;

    void SetResource(size_t nameID, ID3D11ShaderResourceView* resource); // ToDo nullptr?
    void SetSampler(size_t nameID, ID3D11SamplerState* sampler);
    bool GetResource(size_t nameID, ID3D11ShaderResourceView*& resource);
    bool GetSampler(size_t nameID, ID3D11SamplerState*& sampler);

private:
    std::unordered_map<size_t, ID3D11ShaderResourceView*>  _resources = {};
    std::unordered_map<size_t, ID3D11SamplerState*>        _samplers = {};
};

#endif //SHADERINPUTSDATA_H
