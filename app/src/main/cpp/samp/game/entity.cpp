#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "Streaming.h"
#include "game/Models/ModelInfo.h"
#include <cmath>

extern CGame* pGame;
extern CNetGame *pNetGame;

// 0.3.7
void CEntity::GetMatrix(RwMatrix* Matrix)
{
	if (!m_pEntity || !m_pEntity->m_matrix) return;

	Matrix->right.x = m_pEntity->m_matrix->m_right.x;
	Matrix->right.y = m_pEntity->m_matrix->m_right.y;
	Matrix->right.z = m_pEntity->m_matrix->m_right.z;

	Matrix->up.x = m_pEntity->m_matrix->m_up.x;
	Matrix->up.y = m_pEntity->m_matrix->m_up.y;
	Matrix->up.z = m_pEntity->m_matrix->m_up.z;

	Matrix->at.x = m_pEntity->m_matrix->m_forward.x;
	Matrix->at.y = m_pEntity->m_matrix->m_forward.y;
	Matrix->at.z = m_pEntity->m_matrix->m_forward.z;

	Matrix->pos.x = m_pEntity->m_matrix->m_pos.x;
	Matrix->pos.y = m_pEntity->m_matrix->m_pos.y;
	Matrix->pos.z = m_pEntity->m_matrix->m_pos.z;
}
// 0.3.7
void CEntity::SetMatrix(RwMatrix Matrix)
{
	if (!m_pEntity || !m_pEntity->m_matrix) return;

	m_pEntity->m_matrix->m_right.x = Matrix.right.x;
	m_pEntity->m_matrix->m_right.y = Matrix.right.y;
	m_pEntity->m_matrix->m_right.z = Matrix.right.z;

	m_pEntity->m_matrix->m_up.x = Matrix.up.x;
	m_pEntity->m_matrix->m_up.y = Matrix.up.y;
	m_pEntity->m_matrix->m_up.z = Matrix.up.z;

	m_pEntity->m_matrix->m_forward.x = Matrix.at.x;
	m_pEntity->m_matrix->m_forward.y = Matrix.at.y;
	m_pEntity->m_matrix->m_forward.z = Matrix.at.z;

	m_pEntity->m_matrix->m_pos.x = Matrix.pos.x;
	m_pEntity->m_matrix->m_pos.y = Matrix.pos.y;
	m_pEntity->m_matrix->m_pos.z = Matrix.pos.z;
}
// 0.3.7
void CEntity::GetMoveSpeedVector(CVector* Vector)
{
	Vector->x = m_pEntity->vecMoveSpeed.x;
	Vector->y = m_pEntity->vecMoveSpeed.y;
	Vector->z = m_pEntity->vecMoveSpeed.z;
}
// 0.3.7
void CEntity::SetMoveSpeedVector(CVector Vector)
{
	m_pEntity->vecMoveSpeed.x = Vector.x;
	m_pEntity->vecMoveSpeed.y = Vector.y;
	m_pEntity->vecMoveSpeed.z = Vector.z;
}
// 0.3.7
void CEntity::GetTurnSpeedVector(CVector* Vector)
{
	Vector->x = m_pEntity->vecTurnSpeed.x;
	Vector->y = m_pEntity->vecTurnSpeed.y;
	Vector->z = m_pEntity->vecTurnSpeed.z;
}
// 0.3.7
void CEntity::SetTurnSpeedVector(CVector Vector)
{
	m_pEntity->vecTurnSpeed.x = Vector.x;
	m_pEntity->vecTurnSpeed.y = Vector.y;
	m_pEntity->vecTurnSpeed.z = Vector.z;
}
// 0.3.7
uint CEntity::GetModelIndex()
{
	return m_pEntity->nModelIndex;
}
// 0.3.7

bool IsPedModel(unsigned int iModelID)
{
    if(iModelID < 0 || iModelID > 20000) return false;
    auto dwModelArray = CModelInfo::ms_modelInfoPtrs;

    uintptr_t ModelInfo = reinterpret_cast<uintptr_t>(dwModelArray[iModelID]);
    if(ModelInfo && *(uintptr_t*)ModelInfo == (uintptr_t)g_libGTASA + (VER_x32 ? 0x00667658 : 0x82F310))
        return true;

    return false;
}
void CEntity::SetModelIndex(uint uiModel)
{
    if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
    if(!IsPedModel(uiModel))
        uiModel = 0;

    if(m_pEntity)
    {
        auto oldModelId = m_pEntity->m_nModelIndex;

        if (!CStreaming::TryLoadModel(uiModel))
            return;

        // CEntity::DeleteRwObject();
        m_pEntity->m_nModelIndex = uiModel;

        CStreaming::RemoveModelIfNoRefs(oldModelId);
    }
}
// 0.3.7
void CEntity::TeleportTo(float fX, float fY, float fZ)
{
	RwMatrix mat;

	if (m_pEntity && *(uint32_t*)m_pEntity != (g_libGTASA + (VER_x32 ? 0x667D14 : 0x830098))) /* CPlaceable */
	{
		uint16_t modelIndex = m_pEntity->nModelIndex;
		if (modelIndex != TRAIN_PASSENGER_LOCO &&
			modelIndex != TRAIN_FREIGHT_LOCO &&
			modelIndex != TRAIN_TRAM)
			//((void(*)(ENTITY_TYPE*, float, float, float, bool))(*(void**)(m_pEntity->vtable + 0x3C)))(m_pEntity, fX, fY, fZ, 0);
		{
            ((void(*)(ENTITY_TYPE*, float, float, float))(*(void**)(m_pEntity + (VER_x32 ? 0x3C : 0x3C*2))))(m_pEntity, fX, fY, fZ);
		}
		else
			ScriptCommand(&put_train_at, m_dwGTAId, fX, fY, fZ);
	}
}
// 0.3.7
bool CEntity::IsAdded()
{
	if (m_pEntity)
	{
		if (*(uint32_t*)m_pEntity == (g_libGTASA + /*0x5C7358*/(VER_x32 ? 0x667D14 : 0x830098))) // CPlaceable
			return false;

		if (m_pEntity->m_pMovingList)
			return true;
	}

	return false;
}
// 0.3.7
void CEntity::Add()
{
	if (!m_pEntity || *(uint32_t*)m_pEntity == (g_libGTASA + /*0x5C7358*/(VER_x32 ? 0x667D14 : 0x830098))) { // CPlaceable
		return;
	}

	if (!m_pEntity->m_pMovingList) {

		CVector vec = { 0.0f, 0.0f, 0.0f };

		SetMoveSpeedVector(vec);
		SetTurnSpeedVector(vec);

		// CWorld::Add
		((void(*)(ENTITY_TYPE*))(g_libGTASA + (VER_x32 ? 0x423418 + 1:0x507518)))(m_pEntity);

		RwMatrix mat;
		GetMatrix(&mat);
		TeleportTo(mat.pos.x, mat.pos.y, mat.pos.z);
	}
}
// 0.3.7
void CEntity::Remove()
{
	if (!m_pEntity || *(uint32_t*)m_pEntity == (g_libGTASA + /*0x5C7358*/(VER_x32 ? 0x667D14 : 0x830098))) { // CPlaceable
		return;
	}

	if (m_pEntity->m_pMovingList) {
		// CWorld::Remove
		((void(*)(ENTITY_TYPE*))(g_libGTASA + (VER_x32 ? 0x42330C + 1:0x5073A0)))(m_pEntity);
	}
}

float CEntity::GetDistanceFromCamera()
{
    if(!this)
        return 0;

    CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));
    RwMatrix matPos;
    GetMatrix(&matPos);
    return DistanceBetweenPoints(matPos.pos, TheCamera.GetPosition());
}

// 0.3.7
float CEntity::GetDistanceFromPoint(CVector Vector)
{
	RwMatrix mat;
	GetMatrix(&mat);

	float tmpX = (mat.pos.x - Vector.x) * (mat.pos.x - Vector.x);
	float tmpY = (mat.pos.y - Vector.y) * (mat.pos.y - Vector.y);
	float tmpZ = (mat.pos.z - Vector.z) * (mat.pos.z - Vector.z);

	return (float)sqrt(tmpX + tmpY + tmpZ);
}

// 0.3.7
uintptr_t CEntity::GetRWObject()
{
	if (m_pEntity)
		return reinterpret_cast<uintptr_t>(m_pEntity->pRwObject);

	return 0;
}
// 0.3.7
float CEntity::GetDistanceFromLocalPlayerPed()
{
	RwMatrix	matFromPlayer;
	RwMatrix	matThis;
	float		fSX, fSY, fSZ;

	CPlayerPed* pLocalPlayerPed = pGame->FindPlayerPed();
	CLocalPlayer* pLocalPlayer = nullptr;

	if (!pLocalPlayerPed) return 10000.0f;
	if (!m_pEntity) return 10000.0f;

	GetMatrix(&matThis);

	if (pNetGame) {
		pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		if (pLocalPlayer && (pLocalPlayer->IsSpectating() || pLocalPlayer->IsInRCMode())) {
            CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));
			TheCamera.GetMatrix(&matFromPlayer);
		}
		else {
			pLocalPlayerPed->GetMatrix(&matFromPlayer);
		}
	}
	else {
		pLocalPlayerPed->GetMatrix(&matFromPlayer);
	}

	fSX = (matThis.pos.x - matFromPlayer.pos.x) * (matThis.pos.x - matFromPlayer.pos.x);
	fSY = (matThis.pos.y - matFromPlayer.pos.y) * (matThis.pos.y - matFromPlayer.pos.y);
	fSZ = (matThis.pos.z - matFromPlayer.pos.z) * (matThis.pos.z - matFromPlayer.pos.z);

	return (float)sqrt(fSX + fSY + fSZ);
}

void CEntity::SetCollisionChecking(bool bCheck)
{
	if (m_pEntity && *(uint32_t*)m_pEntity != (g_libGTASA + /*0x5C7358*/(VER_x32 ? 0x667D14 : 0x830098)))
	{
		if (bCheck)
			m_pEntity->dwProcessingFlags |= 1;
		else
			m_pEntity->dwProcessingFlags &= 0xFFFFFFFE;
	}
}

bool CEntity::GetCollisionChecking()
{
	if (m_pEntity && *(uint32_t*)m_pEntity != (g_libGTASA + /*0x5C7358*/(VER_x32 ? 0x667D14 : 0x830098)))
		return m_pEntity->dwProcessingFlags & 1;

	return true;
}

void CEntity::SetGravityProcessing(bool state)
{
	if (m_pEntity && *(uint32_t*)m_pEntity != (g_libGTASA + /*0x5C7358*/(VER_x32 ? 0x667D14 : 0x830098)))
	{
		if (state)
			m_pEntity->dwProcessingFlags &= 0x7FFFFFFD;
		else
			m_pEntity->dwProcessingFlags |= 0x80000002;
	}
}
// 0.3.7
void CEntity::UpdateMatrix(RwMatrix matrix)
{
	if (m_pEntity && m_pEntity->m_matrix)
	{
		// CPhysical::Remove
		((void (*)(ENTITY_TYPE*))(*(uintptr_t*)(*(uint32_t*)m_pEntity + (VER_x32 ? 0x10 : 0x10*2))))(m_pEntity);

		this->SetMatrix(matrix);
		this->UpdateRwMatrixAndFrame();

		// CPhysical::Add
		((void (*)(ENTITY_TYPE*))(*(uintptr_t*)(*(uint32_t*)m_pEntity + (VER_x32 ? 0x8 : 0x8*2))))(m_pEntity);
	}
}

// 0.3.7
void CEntity::UpdateRwMatrixAndFrame()
{
	if (m_pEntity && *(uint32_t*)m_pEntity != (g_libGTASA + (VER_x32 ? 0x667D14 : 0x830098))) // CPlaceable
	{
		if (m_pEntity->pRwObject)
		{
			if (m_pEntity->m_matrix)
			{
				uintptr_t pRwMatrix = *(uintptr_t*)(m_pEntity->pRwObject + 4) + 0x10;

				// CMatrix::UpdateRwMatrix
				((void (*)(RwMatrix*, uintptr_t))(g_libGTASA + (VER_x32 ? 0x44EE3E + 1 : 0x537688)))(
                        reinterpret_cast<RwMatrix *>(m_pEntity->m_matrix), pRwMatrix);

				// CEntity::UpdateRwFrame
				((void (*)(ENTITY_TYPE*))(g_libGTASA + (VER_x32 ? 0x3EC038 + 1:0x4CBAEC)))(m_pEntity);
			}
		}
	}
}
// 0.3.7
void CEntity::Render()
{
	if(!m_pEntity && IsGameEntityArePlaceable(m_pEntity))
		return;

	uintptr_t pRwObject = GetRWObject();

	int iModel = GetModelIndex();
	if (iModel >= 400 && iModel <= 611 && pRwObject) {
		// CVisibilityPlugins::SetupVehicleVariables
		((void (*)(uintptr_t))(g_libGTASA + 0x5D4B40 + 1))(pRwObject);
	}

	// CEntity::PreRender
	((void (*)(ENTITY_TYPE*))(*(void**)(*(uint32_t*)m_pEntity + (VER_x32 ? 0x48: 0x48*2))))(m_pEntity);

	if (pRwObject) {
		// CRenderer::RenderOneNonRoad
		((void (*)(ENTITY_TYPE*))(g_libGTASA + (VER_x32 ? 0x41030C + 1:0x4F56E0)))(m_pEntity);
	}
}

RpHAnimHierarchy* CEntity::GetAnimHierarchyFromSkinClump() 
{
    return nullptr;
	//return ((RpHAnimHierarchy* (*)(uint32_t))(g_libGTASA+0x5D1021))(*(uint32_t *)(*((uint32_t *)this+1) + 24));
}

void CEntity::UpdateRpHAnim()
{
	if(!m_pEntity) return;

	((void (*)(uint32_t))(g_libGTASA + (VER_x32 ? 0x3EC046 + 1:0x4CBB00)))(*((uint32_t *)this+1));
}

bool CEntity::IsStationary()
{
	if(!IsAdded()) return false; // movespeed vectors are invalid if its not added

	if(m_pEntity->vecMoveSpeed.x == 0.0f &&
	   m_pEntity->vecMoveSpeed.y == 0.0f &&
	   m_pEntity->vecMoveSpeed.z == 0.0f)
	{
		return true;
	}
	return false;
}

bool CEntity::EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	if(!m_pEntity) return false;

	RwMatrix matWorld;
	CVector vecMoveSpeed;
	GetMatrix(&matWorld);
	GetMoveSpeedVector(&vecMoveSpeed);

	if(matWorld.pos.x > fPX)
	{
		if(vecMoveSpeed.x != 0.0f)
		{
			vecMoveSpeed.x = -0.2f;
			vecMoveSpeed.z = 0.1f;
		}

		SetMoveSpeedVector(vecMoveSpeed);
		matWorld.pos.z += 0.04f;
		SetMatrix(matWorld);
		return true;
	}
	else if(matWorld.pos.x < fZX)
	{
		if(vecMoveSpeed.x != 0.0f)
		{
			vecMoveSpeed.x = 0.2f;
			vecMoveSpeed.z = 0.1f;
		}

		SetMoveSpeedVector(vecMoveSpeed);
		matWorld.pos.z += 0.04f;
		SetMatrix(matWorld);
		return true;
	}
	else if(matWorld.pos.y > fPY)
	{
		if(vecMoveSpeed.y != 0.0f)
		{
			vecMoveSpeed.y = -0.2f;
			vecMoveSpeed.z = 0.1f;
		}

		SetMoveSpeedVector(vecMoveSpeed);
		matWorld.pos.z += 0.04f;
		SetMatrix(matWorld);
		return true;
	}
	else if(matWorld.pos.y < fNY)
	{
		if(vecMoveSpeed.y != 0.0f)
		{
			vecMoveSpeed.y = 0.2f;
			vecMoveSpeed.z = 0.1f;
		}

		SetMoveSpeedVector(vecMoveSpeed);
		matWorld.pos.z += 0.04f;
		SetMatrix(matWorld);
		return true;
	}

	return false;
}

bool CEntity::HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	if(m_pEntity)
	{
		RwMatrix matWorld;
		GetMatrix(&matWorld);

		if(matWorld.pos.x > fPX || matWorld.pos.x < fZX || matWorld.pos.y > fPY || matWorld.pos.y < fNY)
			return true;
	}

	return false;
}