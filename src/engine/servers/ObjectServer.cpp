#include "ObjectServer.h"

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
    assert("Use Scene::Destroy to destroy SceneEntity instead", (!dynamic_cast<NSE_SceneEntity>(obj))); // ToDo check if works Maybe static assert?

    if (obj)
    {
        // This should erase the only shared_ptr causing the object to be destructed
        _objects.erase(obj->GetUID());
    }

    assert(!obj);
}