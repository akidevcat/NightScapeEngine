#ifndef SCREENSERVER_H
#define SCREENSERVER_H
#include "BaseServer.h"

namespace NSE
{
    class ScreenServer : public BaseServer<ScreenServer>
    {
    public:
        ScreenServer(int resolutionX, int resolutionY);

        [[nodiscard]] int GetWidth() const { return _width; }
        [[nodiscard]] int GetHeight() const { return _height; }

    private:
        int _width, _height;
    };
}

#endif //SCREENSERVER_H
