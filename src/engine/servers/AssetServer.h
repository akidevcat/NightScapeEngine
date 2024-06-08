#ifndef ASSETS_SERVER_H
#define ASSETS_SERVER_H
#include <string>

#include "BaseServer.h"
#include "../data/Mesh.h"

namespace NSE
{
    class AssetsServer : public BaseServer<AssetsServer>
    {
    public:
        AssetsServer();
        ~AssetsServer();

        NSE_Mesh LoadMeshAsset(const std::string& path);
        // NSE_Texture LoadTexture();
    };
}

#endif //ASSETS_SERVER_H
