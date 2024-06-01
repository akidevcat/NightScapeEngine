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
    // static_assert(std::is_base_of_v<Object, T>);
    assert("Use Scene::Destroy to destroy SceneEntity instead", (!dynamic_cast<NSE_SceneEntity>(obj))); // ToDo
    // This should erase the only shared_ptr causing the object to be destructed
    _objects.erase(obj->GetUID());
    assert(!obj);
}