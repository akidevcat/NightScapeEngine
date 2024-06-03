#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include <d3d11.h>
#include <d3d11shader.h>
#include <unordered_map>

#include "ConstantBufferData.h"
#include "ConstantBufferDescription.h"
#include "../entity/Object.h"

#define NSE_ConstantBuffer obj_ptr<NSE::ConstantBuffer>

namespace NSE
{
    class ConstantBuffer : public Object
    {
    public:
        ConstantBuffer() = delete;
        explicit ConstantBuffer(size_t nameID);
        ConstantBuffer(size_t nameID, size_t size);

        void Resize(size_t size);
        void Reflect(ID3D11ShaderReflection* reflection, size_t bufferID, bool autoResize = false);
        void EnableBufferData();
        void DisableBufferData();
        void UploadData(ConstantBufferData* data) const;
        void UploadData() const;
        void Release();

        [[nodiscard]] bool                       IsReflected() const { return _isReflected; }
        [[nodiscard]] ID3D11Buffer*              GetGPUBuffer() const { return _buffer; }
        [[nodiscard]] ConstantBufferDescription* GetDescription() const { return _description; }
        [[nodiscard]] ConstantBufferData*        GetBufferData() const { return _data; }

    private:
        void InitBuffer();
        void InitBufferData();

    private:
        size_t                     _nameID;
        size_t                     _size;
        bool                       _isReflected = false;
        bool                       _isBufferDataEnabled = false;
        ID3D11Buffer*              _buffer = nullptr;
        ConstantBufferDescription* _description = nullptr;
        ConstantBufferData*        _data = nullptr;

    };
}

#endif //CONSTANTBUFFER_H
