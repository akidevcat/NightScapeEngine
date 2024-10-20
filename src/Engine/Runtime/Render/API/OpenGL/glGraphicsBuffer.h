#pragma once

#include "../../GraphicsBuffer.h"

namespace NSE
{
    class glGraphicsBuffer : public GraphicsBuffer
    {
    public:
        glGraphicsBuffer() = delete;
        glGraphicsBuffer(Target target, size_t size, bool keepDataOnCPU = true);
        glGraphicsBuffer(Target target, size_t stride, size_t count, bool keepDataOnCPU = true);
        ~glGraphicsBuffer() override;

        void Release() override;

        void SetData(void const* value, size_t valueSize, size_t offset) final;
        void Upload() final;
        void UploadData(void const* value, size_t valueSize, size_t offset) const final;

        [[nodiscard]] size_t size() const override { return _size; }
        [[nodiscard]] size_t stride() const override { return _stride; }
        [[nodiscard]] void* data() const override { return _data; }

    private:
        void InitializeData();
        void InitializeBuffer();

    private:
        GLuint          _bufferID = GL_ZERO;
        GLenum          _bufferUsage = GL_ZERO;
        GLenum          _bufferTarget = GL_ZERO;
        Target          _target;
        void*           _data = nullptr;
        bool            _keepDataOnCPU;
        size_t          _size = 0;
        size_t          _stride = 0;
        bool            _isReflected = false;
    };
}