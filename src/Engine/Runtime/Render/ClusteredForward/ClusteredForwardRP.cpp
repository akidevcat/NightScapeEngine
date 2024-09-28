#include "ClusteredForwardRP.h"

#include "../RenderServer.h"

void NSE::ClusteredForwardRP::OnLoad()
{

}

void NSE::ClusteredForwardRP::OnUnload()
{

}

void NSE::ClusteredForwardRP::Render(const Components::Camera &camera, const Components::Transform &transform)
{
    sRender->api()->ClearRenderTargetColor(float4{0, 1, 1, 1});
}
