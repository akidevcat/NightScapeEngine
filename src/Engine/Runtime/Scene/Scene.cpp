// #include "pch.h"
#include "Scene.h"

size_t NSE::Scene::_uidCount = 1;

NSE::Scene::Scene()
{
    _uid = _uidCount++;

    // auto v = _registry.view<TestComponent>();
    // v.storage()
}

NSE::Scene::~Scene()
{

}

void NSE::Scene::OnDispose()
{

}
