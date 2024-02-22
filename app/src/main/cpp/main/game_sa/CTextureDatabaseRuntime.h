//
// Created by admin on 20.02.2024.
//

#ifndef SA_MP_LAUNCHER_CTEXTUREDATABASERUNTIME_H
#define SA_MP_LAUNCHER_CTEXTUREDATABASERUNTIME_H

#include "../main.h"

class CTextureDatabaseRuntime {
public:
    enum TextureDatabaseFormat
    {
        DF_UNC = 0,
        DF_DXT = 1,
        DF_360 = 2,
        DF_PS3 = 3,
        DF_PVR = 4,
        DF_ETC = 5,
        DF_Default = 6,
        DF_ALL = 7
    };
    static RwTexture* GetTexture(const char *name);
    // static unsigned int *storedTexels;
    static CTextureDatabaseRuntime* Load(const char *withName, bool fullyLoad, TextureDatabaseFormat forcedFormat);

    static void Register(CTextureDatabaseRuntime *thiz);

    static void Unregister(CTextureDatabaseRuntime *toUnregister);

    static CTextureDatabaseRuntime *GetDatabase(const char *a1);
};


#endif //SA_MP_LAUNCHER_CTEXTUREDATABASERUNTIME_H
