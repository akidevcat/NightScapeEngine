#ifndef LIGHT_H
#define LIGHT_H

#include "../entity/SceneEntity.h"

#define NSE_Light obj_ptr<NSE::Light>

namespace NSE
{
    class Light : public SceneEntity
    {
    public:
        Light();
        ~Light() override;

    public:
        DirectX::XMFLOAT4  lightColor = {1,1,1,0};
        float               lightIntensity = 1.0f;
        float               lightRadius = 1.0f;
    };
}



#endif //LIGHT_H
