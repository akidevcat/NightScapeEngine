#ifndef NAVIGATIONSYSTEM_H
#define NAVIGATIONSYSTEM_H
#include <unordered_map>

#include "../../engine/obj_ptr.h"

class INavigatable
{

};

class NavigationSystem
{
    

private:
    std::unordered_map<size_t, obj_ptr<INavigatable>> _targets;
};

#endif //NAVIGATIONSYSTEM_H
