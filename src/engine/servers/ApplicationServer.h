#ifndef APPLICATIONSERVER_H
#define APPLICATIONSERVER_H

#include "BaseServer.h"

namespace NSE
{
    class ApplicationServer : public BaseServer<ApplicationServer>
    {
    public:
        ApplicationServer();
        ~ApplicationServer();

        void Exit();
        bool Update();

    private:
        bool _isExitState = false;
    };
}

#endif //APPLICATIONSERVER_H
