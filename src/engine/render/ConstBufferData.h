#ifndef CONSTBUFFERDATA_H
#define CONSTBUFFERDATA_H

#include <d3d11.h>
#include <d3d11shader.h>
#include <unordered_map>

class ConstBufferData
{
public:
    ConstBufferData() = delete;
    ConstBufferData(ID3D11Device* device, UINT id, size_t uid, size_t size, void* customDataPtr);
    ConstBufferData(ID3D11Device* device, UINT id, size_t uid, D3D11_SHADER_BUFFER_DESC desc, size_t size);
    ~ConstBufferData();

    bool ConstBufferData::SetVar(size_t pUid, void* value, size_t valueSize);
    void SetResourceView(size_t uid, ID3D11ShaderResourceView* res);

    bool IsDirty() { return _isDirty; }
    void MarkDirty() { _isDirty = true; }
    void UnmarkDirty() { _isDirty = false; }

public:
    UINT                                                   id = 0;
    size_t                                                 uid = 0;
    size_t                                                 size = 0;
    D3D11_SHADER_BUFFER_DESC                               description = {};
    ID3D11Buffer*                                          bPtr = nullptr;
    void*                                                  vPtr = nullptr;
    std::unordered_map<size_t, D3D11_SHADER_VARIABLE_DESC> vDesc = {};
    std::unordered_map<size_t, ID3D11ShaderResourceView*>  resMap = {};

private:
    bool                                                   _isDirty = false;
    bool                                                   _isCustomDataPtr = false;
};

#endif //CONSTBUFFERDATA_H
