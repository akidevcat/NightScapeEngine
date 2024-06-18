#ifndef ASSETS_SERVER_H
#define ASSETS_SERVER_H
#include <string>

#include "BaseServer.h"
#include "../data/Mesh.h"
#include "../render/Texture2D.h"

namespace NSE
{
    class AssetsServer : public BaseServer<AssetsServer>
    {
    public:
        AssetsServer();
        ~AssetsServer();

        NSE_Mesh        LoadMeshAsset(const std::string& path);
        NSE_Texture2D   LoadTextureAsset(const wchar_t* path);
    };
}

#endif //ASSETS_SERVER_H
