#ifndef RIGIDENTITY_H
#define RIGIDENTITY_H

#include "CollisionEntity.h"
#include "../entity/SceneEntity.h"

namespace NSE
{
    class Engine;

    class RigidEntity : public CollisionEntity
    {
    public:
        RigidEntity();
        ~RigidEntity() override;

    private:
        virtual void OnPhysicsUpdate();

    public:
        void AddForce(float3 force);
        void AddTorque(float3 torque);

    public:
        float mass = 1;
        float drag = 0;
        float angularDrag = 0;
        float3 velocity = {};
        float3 angularVelocity = {};
        bool isKinematic = false;
        bool isGravityAffected = true;
        bool isRotationFrozen = false;
        float3 centerOfMassOffset = {};
        float maxVelocity = -1;
        float maxAngularVelocity = -1;

    private:

        friend Engine;
    };
}



#endif //RIGIDENTITY_H
