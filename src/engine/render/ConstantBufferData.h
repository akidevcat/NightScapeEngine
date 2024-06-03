#ifndef CONSTANTBUFFERDATA_H
#define CONSTANTBUFFERDATA_H
#include <cassert>

class ConstantBufferData
{
public:
    ConstantBufferData() = delete;
    explicit ConstantBufferData(size_t size);

    template <typename T>
        T* As()
    {
        assert((sizeof(T) <= _size));
        _isDirty = true;
        return static_cast<T*>(_dataPtr);
    } // ToDo

    void Set(size_t valueOffset, void* value, size_t valueSize);
    void Set(void* structPtr, void* memberPtr, void* value, size_t valueSize); // ToDo

    [[nodiscard]] size_t GetSize() const { return _size; }
    [[nodiscard]] bool IsDirty() const { return _isDirty; }
    void MarkDirty() { _isDirty = true; }
    void ResetDirty() { _isDirty = false; }

    [[nodiscard]] void* GetDataPointer() const { return _dataPtr; } // ToDo make friend to ConstantBuffer?

private:
    size_t _size;
    void* _dataPtr = nullptr;
    bool _isDirty = false;
};

#endif //CONSTANTBUFFERDATA_H
