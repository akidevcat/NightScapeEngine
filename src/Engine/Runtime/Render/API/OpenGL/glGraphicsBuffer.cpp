#include "glGraphicsBuffer.h"

NSE::glGraphicsBuffer::glGraphicsBuffer(Target target, size_t size, bool keepDataOnCPU)
    : _target(target), _keepDataOnCPU(keepDataOnCPU), _size(size)
{
    if (size % 16 != 0)
    {
        _size += 16 - size % 16;
    }

    if (size > 0)
    {
        InitializeBuffer();
        if (keepDataOnCPU)
        {
            InitializeData();
        }
    }
}

NSE::glGraphicsBuffer::glGraphicsBuffer(Target target, size_t stride, size_t count, bool keepDataOnCPU)
: _target(target), _keepDataOnCPU(keepDataOnCPU), _stride(stride)
{
    _size = stride * count;

    if (_size % 16 != 0)
    {
        _size += 16 - _size % 16;
    }

    if (_size > 0)
    {
        InitializeBuffer();
        if (keepDataOnCPU)
        {
            InitializeData();
        }
    }
}

NSE::glGraphicsBuffer::~glGraphicsBuffer()
{
    glGraphicsBuffer::Release();
}

void NSE::glGraphicsBuffer::Release()
{
    // ToDo
    // glDeleteBuffers(1, &_bufferID);

    free(_data);
    _data = nullptr;
}

void NSE::glGraphicsBuffer::SetData(void const *value, size_t valueSize, size_t offset)
{
    assert(("An attempt to write outside of the data memory", offset + valueSize <= _size));

    if (_keepDataOnCPU)
    {
        memcpy(static_cast<char*>(_data) + offset, value, valueSize);
    }
    else
    {
        UploadData(value, valueSize, offset);
    }
}

void NSE::glGraphicsBuffer::Upload()
{
    assert(_keepDataOnCPU);

    UploadData(_data, _size, 0);
}

void NSE::glGraphicsBuffer::UploadData(void const *value, size_t valueSize, size_t offset) const
{
    glNamedBufferSubData(_bufferID, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(valueSize), value);
}

void NSE::glGraphicsBuffer::InitializeData()
{
    assert(_keepDataOnCPU);

    free(_data);
    _data = malloc(_size);
    memset(_data, 0, _size);
}

void NSE::glGraphicsBuffer::InitializeBuffer()
{
    Release();

    glGenBuffers(1, &_bufferID);

    switch (_target)
    {
        case Target::Default:
            _bufferTarget = GL_ARRAY_BUFFER;
            _bufferUsage = GL_DYNAMIC_DRAW;
            break;

        case Target::Constant: // UBO
            _bufferTarget = GL_UNIFORM_BUFFER;
            _bufferUsage = GL_DYNAMIC_DRAW;
            break;

        case Target::Index: // EBO
            _bufferTarget = GL_ELEMENT_ARRAY_BUFFER;
            _bufferUsage = GL_STATIC_DRAW;
            break;

        case Target::Vertex: // VBO
            _bufferTarget = GL_ARRAY_BUFFER;
            _bufferUsage = GL_STATIC_DRAW;
            break;

        case Target::Structured: // SSBO
            _bufferTarget = GL_SHADER_STORAGE_BUFFER;
            _bufferUsage = GL_DYNAMIC_DRAW;
            break;
    }

    glBindBuffer(_bufferTarget, _bufferID);
    glBufferData(_bufferTarget, static_cast<long long>(_size), nullptr, _bufferUsage);
    glBindBuffer(_bufferTarget, GL_ZERO);
}
