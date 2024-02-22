//
// Created by admin on 20.02.2024.
//

#include "CTextureDatabaseRuntime.h"

CTextureDatabaseRuntime* TextureDatabaseRuntime::Load(const char *withName, bool fullyLoad, TextureDatabaseFormat forcedFormat)
{
    return ((TextureDatabase* (*)(const char*, int, int))(g_libGTASA + 0x001EA864 + 1))(withName, fullyLoad, forcedFormat);
}

CTextureDatabaseRuntime* TextureDatabaseRuntime::GetDatabase(const char *a1)
{
    return ((TextureDatabase* (*)(const char*))(g_libGTASA + 0x001EAC0C + 1))(a1);
}

void CTextureDatabaseRuntime::Register(CTextureDatabaseRuntime *toRegister)
{
    ((void (*)(TextureDatabase*))(g_libGTASA + 0x001E9B48 + 1))(toRegister);
}

void CTextureDatabaseRuntime::Unregister(CTextureDatabaseRuntime *toUnregister)
{
    ((void (*)(TextureDatabase*))(g_libGTASA + 0x001E9C00 + 1))(toUnregister);
}

RwTexture* CTextureDatabaseRuntime::GetTexture(const char *name)
{
    return ((RwTexture* (*)(const char*))(g_libGTASA + 0x1E9C64 + 1))(name);
}
