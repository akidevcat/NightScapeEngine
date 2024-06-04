#ifndef RENDERPIPELINESERVER_H
#define RENDERPIPELINESERVER_H

#include "ObjectServer.h"

namespace  NSE
{
    class RenderPipelineServer : public BaseServer<RenderPipelineServer>
    {
    public:
        RenderPipelineServer();
        ~RenderPipelineServer();

        void RenderFrame();
    };
}

#endif //RENDERPIPELINESERVER_H
