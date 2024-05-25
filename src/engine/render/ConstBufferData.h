#ifndef CONSTBUFFERDATA_H
#define CONSTBUFFERDATA_H

#include <d3d11.h>
#include <d3d11shader.h>
#include <unordered_map>

class ConstBufferData
{
public:
    ConstBufferData() = delete;
    ConstBufferData(UINT id, size_t uid, D3D11_SHADER_BUFFER_DESC desc);
    ConstBufferData(UINT id, size_t uid, D3D11_SHADER_BUFFER_DESC desc, size_t size);
    ~ConstBufferData();

    bool ConstBufferData::SetVar(size_t pUid, void* value, size_t valueSize);

    bool IsDirty() { return _isDirty; }
    void UnmarkDirty() { _isDirty = false; }

public:
    UINT                                                   id = 0;
    size_t                                                 uid = 0;
    size_t                                                 size = 0;
    D3D11_SHADER_BUFFER_DESC                               description = {};
    ID3D11Buffer*                                          bPtr = nullptr;
    void*                                                  vPtr = nullptr;
    std::unordered_map<size_t, D3D11_SHADER_VARIABLE_DESC> vDesc = {};

private:
    bool                                                   _isDirty = false;
};

#endif //CONSTBUFFERDATA_H
