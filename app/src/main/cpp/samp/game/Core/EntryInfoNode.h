#pragma once

#include "../common.h"

class CEntryInfoNode
{
public:
    class CPtrListDoubleLink* m_pDoubleLinkList;
    class CPtrNodeDoubleLink* m_pDoubleLink;
    class CRepeatSector* m_pRepeatSector;
    CEntryInfoNode* m_pPrevious;
    CEntryInfoNode* m_pNext;

public:
    static void InjectHooks();
};

VALIDATE_SIZE(CEntryInfoNode, (VER_x32 ? 0x14 : 0x28));
