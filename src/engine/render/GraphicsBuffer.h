#ifndef GRAPHICSBUFFER_H
#define GRAPHICSBUFFER_H

#include "../obj_ptr.h"
#include "../entity/Object.h"

#include <unordered_map>
#include <d3d11.h>
#include <d3d11shader.h>

namespace NSE
{
    class GraphicsBuffer : public Object
    {
    public:

        enum class Target : std::uint8_t
        {
            Vertex          = 0b00000001,
            Index           = 0b00000010,
            Structured      = 0b00000100,
            Constant        = 0b00001000
        };

    public:
        GraphicsBuffer() = delete;
        GraphicsBuffer(Target target, size_t size, bool keepDataOnCPU = true);
        GraphicsBuffer(Target target, size_t stride, size_t count, bool keepDataOnCPU = true);
        ~GraphicsBuffer() override;

        void Set(void const* value, size_t valueSize, size_t offset);
        void Resize(size_t newSize);
        void Upload();
        void Upload(void const* value, size_t valueSize, size_t offset) const;
        void UploadForce();
        void Release();

        void ReflectAsConstantBuffer(ID3D11ShaderReflection* shaderReflection, size_t bufferID, bool autoResize = true);

        [[nodiscard]] ID3D11Buffer* GetD3DBuffer() const { return _d3dBuffer; }
        [[nodiscard]] Target GetTarget() const { return _target; }
        // [[nodiscard]] size_t GetNameID() const { return _nameID; }
        [[nodiscard]] bool IsReflected() const { return _isReflected; }
        [[nodiscard]] D3D11_SHADER_BUFFER_DESC GetD3DDescription() const { assert(_isReflected); return _d3dDescription; }
        [[nodiscard]] bool GetD3DVariableDescription(size_t nameID, D3D11_SHADER_VARIABLE_DESC& desc) const;
        [[nodiscard]] void* GetDataPointer() const { assert(_keepDataOnCPU); return _data; }

        template <typename T>
        T* As()
        {
            assert(sizeof(T) <= _size);
            _isDirty = true;
            return static_cast<T*>(_data);
        }

    private:
        void InitializeBuffer();
        void InitializeData();

    private:
        ID3D11Buffer*   _d3dBuffer = nullptr;
        Target          _target;
        void*           _data = nullptr;
        bool            _keepDataOnCPU;
        size_t          _size = 0;
        size_t          _stride = 0;
        bool            _isDirty = false;
        bool            _isReflected = false;
        // size_t          _nameID = 0;

        D3D11_SHADER_BUFFER_DESC                               _d3dDescription = {};
        std::unordered_map<size_t, D3D11_SHADER_VARIABLE_DESC> _d3dVariableDescriptions = {};
    };
}

typedef obj_ptr<NSE::GraphicsBuffer> NSE_GraphicsBuffer;

#endif //GRAPHICSBUFFER_H
