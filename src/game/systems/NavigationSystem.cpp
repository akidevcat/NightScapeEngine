#include "NavigationSystem.h"

#include "MainSystem.h"
#include "../../engine/entity/Object.h"

void NavigationSystem::OnUpdate()
{
    auto main = MainSystem::Get();
    auto scene = main->GetMainScene();

    // std::vector<obj_ptr<INavigatable>> targets;
    // scene->FindAllEntitiesFromBaseType<INavigatable>(targets);

    // float f= 0;
}
