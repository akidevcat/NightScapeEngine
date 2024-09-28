#pragma once

#include "../Core/Ref.h"

namespace NSE
{
    class GraphicsBuffer : public RefCounted
    {
    public:
        enum class Target : std::uint8_t
        {
            Default         = 0b00000000,
            Vertex          = 0b00000001,
            Index           = 0b00000010,
            Structured      = 0b00000100,
            Constant        = 0b00001000
        };

    public:
        GraphicsBuffer() = default;
        GraphicsBuffer(Target target, size_t size, bool keepDataOnCPU = true) {}
        GraphicsBuffer(Target target, size_t stride, size_t count, bool keepDataOnCPU = true) {}

        virtual void Release() = 0;

        virtual void SetData(void const* value, size_t valueSize, size_t offset) = 0;
        virtual void Upload() = 0;
        virtual void UploadData(void const* value, size_t valueSize, size_t offset) const = 0;

        template <typename T>
        T* As()
        {
            assert(sizeof(T) <= size());
            return static_cast<T*>(data());
        }

        [[nodiscard]] virtual size_t size() const = 0;
        [[nodiscard]] virtual size_t stride() const = 0;
        [[nodiscard]] virtual void* data() const = 0;
    };
}
