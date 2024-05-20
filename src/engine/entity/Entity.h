#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
protected:
    Entity();

public:
    static void Create(Entity* entity); // ToDo
    virtual ~Entity() = 0; // ToDo

    size_t GetUID();

private:
    size_t _uid;
    static size_t _uidCount;
};

#endif //ENTITY_H