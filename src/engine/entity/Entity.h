#pragma once

#include "Object.h"

#define NSE_Entity obj_ptr<NSE::Entity>

namespace NSE
{
    class Entity : public Object
    {
    protected:
        Entity();

    public:
        ~Entity() override = 0;

    private:
    };
}