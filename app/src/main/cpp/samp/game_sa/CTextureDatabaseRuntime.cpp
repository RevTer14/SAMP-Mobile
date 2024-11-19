//
// Created by admin on 20.02.2024.
//

#include "CTextureDatabaseRuntime.h"
#include <RenderWare.h>

CTextureDatabaseRuntime* CTextureDatabaseRuntime::Load(const char *withName, bool fullyLoad, TextureDatabaseFormat forcedFormat)
{
    return ((CTextureDatabaseRuntime* (*)(const char*, int, int))(g_libGTASA + 0x001EA864 + 1))(withName, fullyLoad, forcedFormat);
}

CTextureDatabaseRuntime* CTextureDatabaseRuntime::GetDatabase(const char *a1)
{
    return ((CTextureDatabaseRuntime* (*)(const char*))(g_libGTASA + 0x001EAC0C + 1))(a1);
}

void CTextureDatabaseRuntime::Register(CTextureDatabaseRuntime *toRegister)
{
    ((void (*)(CTextureDatabaseRuntime*))(g_libGTASA + 0x001E9B48 + 1))(toRegister);
}

void CTextureDatabaseRuntime::Unregister(CTextureDatabaseRuntime *toUnregister)
{
    ((void (*)(CTextureDatabaseRuntime*))(g_libGTASA + 0x001E9C00 + 1))(toUnregister);
}

RwTexture* CTextureDatabaseRuntime::GetTexture(const char *name)
{
    return ((RwTexture* (*)(const char*))(g_libGTASA + 0x1E9C64 + 1))(name);
}
