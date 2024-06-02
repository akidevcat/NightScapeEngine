#include "ObjectServer.h"

#include "../entity/SceneEntity.h"

NSE::ObjectServer::ObjectServer()
{
}

NSE::ObjectServer::~ObjectServer()
{
}

void NSE::ObjectServer::Destroy(const NSE_Object& obj)
{
    DestroyNow(obj); // ToDo
}

void NSE::ObjectServer::DestroyNow(const NSE_Object& obj)
{
    if (obj)
    {
        assert(("Use Scene::Destroy to destroy SceneEntity instead", !dynamic_cast<SceneEntity*>(obj.get()))); // ToDo check if works Maybe static assert?
        // This should erase the only shared_ptr causing the object to be destructed
        _objects.erase(obj->GetUID());
    }

    assert(!obj);
}

void NSE::ObjectServer::DestroyAny(const NSE_Object& obj)
{
    DestroyAnyNow(obj);
}

void NSE::ObjectServer::DestroyAnyNow(const NSE_Object& obj)
{
    if (obj)
    {
        // This should erase the only shared_ptr causing the object to be destructed
        _objects.erase(obj->GetUID());
    }

    assert(!obj);
}
