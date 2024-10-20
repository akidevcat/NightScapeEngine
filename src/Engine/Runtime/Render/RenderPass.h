#pragma once

#include "../Core/Ref.h"
#include "RenderPassConfiguration.h"

namespace NSE
{
    class RenderPass : public RefCounted
    {
    public:
        RenderPass() = default;
        ~RenderPass() override = default;

        virtual void Initialize() {}
        virtual void Dispose() {}

        virtual const RenderPassConfiguration& configuration() const = 0;
    };
}
