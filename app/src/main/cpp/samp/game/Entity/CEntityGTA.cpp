//
// Created by roman on 11/24/2024.
//

#include "CEntityGTA.h"
#include "game/game.h"
#include "game/Collision/ColStore.h"
#include "patch.h"
#include "game/General.h"
#include "game/Models/BaseModelInfo.h"
#include "game/Models/ModelInfo.h"
#include "game/References.h"

extern CGame* pGame;

CEntityGTA::CEntityGTA() : CPlaceable() {
    m_nStatus = STATUS_ABANDONED;
    m_nType = ENTITY_TYPE_NOTHING;

    m_nFlags = 0;
    m_bIsVisible = true;
    m_bBackfaceCulled = true;

    m_nScanCode = 0;
    m_nAreaCode = eAreaCodes::AREA_CODE_NORMAL_WORLD;
    m_nModelIndex = 0xFFFF;
    m_pRwObject = nullptr;
    m_nIplIndex = 0;
    m_nRandomSeed = CGeneral::GetRandomNumber();
    m_pReferences = nullptr;
    m_pStreamingLink = nullptr;
    m_nNumLodChildren = 0;
    m_nNumLodChildrenRendered = 0;
    m_pLod = nullptr;
}

CEntityGTA::~CEntityGTA()
{
    if (m_pLod)
        m_pLod->m_nNumLodChildren--;

    CEntityGTA::DeleteRwObject();
    CEntityGTA::ResolveReferences();
}

void CEntityGTA::ResolveReferences()
{
    auto refs = m_pReferences;
    while (refs) {
        if (*refs->m_ppEntity == this)
            *refs->m_ppEntity = nullptr;

        refs = refs->m_pNext;
    }

    refs = m_pReferences;
    if (!refs)
        return;

    refs->m_ppEntity = nullptr;
    while (refs->m_pNext)
        refs = refs->m_pNext;

    refs->m_pNext = CReferences::pEmptyList;
    CReferences::pEmptyList = m_pReferences;
    m_pReferences = nullptr;
}

void CEntityGTA::Add()
{
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x3ED8D8 + 1 : 0x4CD574), this);
}

void CEntityGTA::SetInterior(int interiorId, bool needRefresh)
{
    m_nAreaCode = static_cast<eAreaCodes>(interiorId);

    if ( this == pGame->FindPlayerPed()->m_pPed )
    {
        CGame::currArea = interiorId;

        auto pos = GetPosition();
        CColStore::RequestCollision(&pos, m_nAreaCode);

        if(interiorId == 0) {
            CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x00420898 + 1 : 0x504194), false);
            // CTimeCycle::StopExtraColour(0);
        }

    }

}

float CEntityGTA::GetDistanceFromLocalPlayerPed() const
{
    auto pLocalPlayerPed = pGame->FindPlayerPed();

    return DistanceBetweenPoints(GetPosition(), pLocalPlayerPed->m_pPed->GetPosition());
}

float CEntityGTA::GetDistanceFromCamera()
{
    if(!this)
        return 0;

    CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));
    return DistanceBetweenPoints(GetPosition(), TheCamera.GetPosition());
}

float CEntityGTA::GetDistanceFromPoint(float X, float Y, float Z) const
{
    CVector vec(X, Y, Z);

    return DistanceBetweenPoints(GetPosition(), vec);
}

void CEntityGTA::SetCollisionChecking(bool bCheck)
{
    m_bCollisionProcessed = bCheck;
}

void CEntityGTA::UpdateRpHAnim() {
    if (const auto atomic = GetFirstAtomic(m_pRwClump)) {
        if (RpSkinGeometryGetSkin(RpAtomicGetGeometry(atomic)) && !m_bDontUpdateHierarchy) {
            RpHAnimHierarchyUpdateMatrices(GetAnimHierarchyFromSkinClump(m_pRwClump));
        }
    }
}

void CEntityGTA::UpdateRwFrame()
{
    if (!m_pRwObject)
        return;

    RwFrameUpdateObjects(static_cast<RwFrame*>(rwObjectGetParent(m_pRwObject)));
}

void CEntityGTA::DeleteRwObject()
{
    if(!*(uintptr*)this) return;

    (( void (*)(CEntityGTA*))(*(void**)(*(uintptr*)this + (VER_x32 ? 0x24 : 0x24*2))))(this);
}

void CEntityGTA::UpdateRW() {
    if (!m_pRwObject)
        return;

    auto parentMatrix = GetModellingMatrix();
    if (m_matrix)
        m_matrix->UpdateRwMatrix(parentMatrix);
    else
        m_placement.UpdateRwMatrix(parentMatrix);
}


RwMatrix* CEntityGTA::GetModellingMatrix() {
    if (!m_pRwObject)
        return nullptr;

    return RwFrameGetMatrix(RwFrameGetParent(m_pRwObject));
}

CBaseModelInfo* CEntityGTA::GetModelInfo() const {
    return CModelInfo::GetModelInfo(m_nModelIndex);
}

void CEntityGTA::Add(const CRect* rect) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x3ED8FC + 1 : 0x4CD5C0), this, rect);
}

void CEntityGTA::Remove() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x3EDBE8 + 1 : 0x4CD888), this);
}

void CEntityGTA::SetModelIndex(uint32 index) {

}

void CEntityGTA::SetModelIndexNoCreate(uint32 index) {

}

void CEntityGTA::SetIsStatic(bool isStatic) {

}