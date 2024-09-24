#include "Scene.h"

size_t NSE::Scene::_uidCount = 1;

NSE::Scene::Scene()
{
    _uid = _uidCount++;
}

NSE::Scene::~Scene()
{

}

void NSE::Scene::OnDispose()
{

}
