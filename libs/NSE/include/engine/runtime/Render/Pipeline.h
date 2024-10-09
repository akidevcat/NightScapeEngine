#pragma once
#include "PipelineConfiguration.h"

namespace NSE
{
    class Pipeline : public RefCounted
    {
    public:
        Pipeline() = default;
        ~Pipeline() override = default;

        virtual PipelineConfiguration& GetConfiguration() = 0;
        virtual const PipelineConfiguration& GetConfiguration() const = 0;
    };
}
