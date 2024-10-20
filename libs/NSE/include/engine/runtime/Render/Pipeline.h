#pragma once
#include "PipelineConfiguration.h"

namespace NSE
{
    class Pipeline : public RefCounted
    {
    public:
        Pipeline() = default;
        ~Pipeline() override = default;

        virtual void Initialize() {}
        virtual void Dispose() {}

        virtual const PipelineConfiguration& configuration() const = 0;
        Ref<Shader> shader() const { return configuration().shader; }
    };
}
