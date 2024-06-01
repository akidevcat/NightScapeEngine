#ifndef ENTITY_H
#define ENTITY_H
#include "Object.h"

#define NSE_Entity obj_ptr<NSE::Entity>

namespace NSE
{
    class Entity : public Object
    {
    protected:
        Entity();

    public:
        virtual ~Entity() = 0;

    private:
    };
}

#endif //ENTITY_H