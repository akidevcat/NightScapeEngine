#ifndef OBJECT_H
#define OBJECT_H

class Object
{
public:
    Object();
    virtual ~Object() = 0;

    size_t GetUID();

    virtual void OnDestroy() = 0;
};

#endif //OBJECT_H
