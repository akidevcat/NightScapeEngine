#ifndef LIGHT_H
#define LIGHT_H

#include "../entity/SceneEntity.h"

namespace NSE
{
    class Light : public SceneEntity
    {
    public:
        Light();
        ~Light() override;

    public:
        DirectX::FXMVECTOR  lightColor = {1,1,1,1};
        float               lightIntensity = 1.0f;
        float               lightRadius = 1.0f;
    };
}



#endif //LIGHT_H
