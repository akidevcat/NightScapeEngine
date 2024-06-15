#ifndef SHADERINPUTSDATA_H
#define SHADERINPUTSDATA_H
#include <unordered_map>
#include "../obj_ptr.h"

#include "PixelShader.h"

class ShaderInputsData
{
public:
    ShaderInputsData() = default;
    ~ShaderInputsData() = default;

    void SetResource(size_t nameID, ID3D11ShaderResourceView* resource); // ToDo nullptr?
    void SetSampler(size_t nameID, ID3D11SamplerState* sampler);
    // void SetBuffer(size_t nameID, ID3D11Buffer* buffer);
    bool GetResource(size_t nameID, ID3D11ShaderResourceView*& resource);
    bool GetSampler(size_t nameID, ID3D11SamplerState*& sampler);
    // bool GetBuffer(size_t nameID, ID3D11Buffer*& buffer);

    void SetConstantBuffer(size_t nameID, const NSE_ConstantBuffer& buffer);
    bool GetConstantBuffer(size_t nameID, NSE_ConstantBuffer& buffer);

private:
    std::unordered_map<size_t, ID3D11ShaderResourceView*>  _resources = {};
    std::unordered_map<size_t, ID3D11SamplerState*>        _samplers = {};
    std::unordered_map<size_t, NSE_ConstantBuffer>         _cbuffers = {};
};

#endif //SHADERINPUTSDATA_H
