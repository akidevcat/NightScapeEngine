#include "ObjectServer.h"

#include "../entity/SceneEntity.h"

NSE::ObjectServer::ObjectServer()
{
}

NSE::ObjectServer::~ObjectServer()
{
    /*
     * It'd be a problem just clearing the _objects map because we'd encounter object deletion while deleting objects
     * and this might cause different problems. Basically chaning map while iterating it.
     *
     * So, we copy pointers to the _objectToDelete map instead. And then, while invoking object destructors,
     * even if we encounter a Destroy() call, it'd do nothing because our _objects map is empty
     *
     * And, of course, we should never use DestroyNow inside a destructor because it'd cause problems
     */

    for (auto obj : _objects)
    {
        _objectsToDelete.try_emplace(obj.first, obj.second);
    }
    _objects.clear();

    _objectsToDelete.clear();
}

void NSE::ObjectServer::Destroy(const NSE_Object& obj)
{
    if (!obj)
    {
        return;
    }

    assert(("Use Scene::Destroy to destroy SceneEntity instead", !dynamic_cast<SceneEntity*>(obj.get())));

    auto result = _objects.find(obj->GetUID());
    if (result != _objects.end())
    {
        _objectsToDelete.try_emplace(obj->GetUID(), result->second);
        _objects.erase(obj->GetUID());
    }
}

void NSE::ObjectServer::DestroyNow(const NSE_Object& obj)
{
    if (obj)
    {
        assert(("Use Scene::DestroyNow to destroy SceneEntity instead", !dynamic_cast<SceneEntity*>(obj.get())));

        // This should erase the only shared_ptr causing the object to be destructed
        if (_objects.find(obj->GetUID()) != _objects.end())
            _objects.erase(obj->GetUID());
    }

    assert(!obj);
}

void NSE::ObjectServer::Update()
{

}

void NSE::ObjectServer::DestroyAny(const NSE_Object& obj)
{
    if (!obj)
    {
        return;
    }

    auto result = _objects.find(obj->GetUID());
    if (result != _objects.end())
    {
        _objectsToDelete.try_emplace(obj->GetUID(), result->second);
        _objects.erase(obj->GetUID());
    }
}

void NSE::ObjectServer::DestroyAnyNow(const NSE_Object& obj)
{
    if (obj)
    {
        // This should erase the only shared_ptr causing the object to be destructed
        if (_objects.find(obj->GetUID()) != _objects.end())
            _objects.erase(obj->GetUID());
    }

    assert(!obj);
}
