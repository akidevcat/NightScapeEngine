#ifndef MATERIAL_H
#define MATERIAL_H

#include "ConstBufferData.h"
#include "Shader.h"

class Material
{
public:
    explicit Material(Shader* shader);
    ~Material();

    bool SetVertexVar(size_t pUid, void* value, size_t valueSize);
    bool SetPixelVar(size_t pUid, void* value, size_t valueSize);

    bool Upload();

public:
    [[nodiscard]] bool IsDirty() const { return _isDirty; }
    [[nodiscard]] Shader* GetShader() const { return _shader; }

private:
    bool             _isDirty = false;
    Shader*          _shader = nullptr;
    ConstBufferData* _vsMaterialProps = nullptr;
    ConstBufferData* _psMaterialProps = nullptr;
};

#endif //MATERIAL_H
