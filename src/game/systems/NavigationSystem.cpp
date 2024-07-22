#include "NavigationSystem.h"

#include "MainSystem.h"
#include "../../engine/entity/Object.h"
#include "../../engine/servers/TimeServer.h"

using namespace NSE;

void NavigationSystem::OnUpdate()
{
    auto main = MainSystem::Get();
    auto scene = main->GetMainScene();

    _targets.clear();
    scene->FindAllEntitiesFromBaseType<INavigatable>(_targets);
}

obj_ptr<INavigatable> NavigationSystem::FindAlignedNavigatable(Vector3d origin, Vector3d direction, double cosMin, bool isShiftSpaceTarget)
{
    double maxCos = -100.0;
    obj_ptr<INavigatable> result = nullptr;

    for (const auto& m : _targets)
    {
        if (isShiftSpaceTarget && !m->GetNavigatableShiftSpaceNavigatable())
            continue;

        Vector3d dir = normalize(m->GetNavigatablePosition() - origin);
        double c = dotproduct(direction, dir);
        if (c >= cosMin && c > maxCos)
        {
            maxCos = c;
            result = m;
        }
    }

    return result;
}
