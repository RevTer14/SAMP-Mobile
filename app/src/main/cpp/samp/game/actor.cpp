#include "../main.h"
#include "game.h"
#include "Streaming.h"

extern CGame* pGame;

// 0.3.7
CActor::CActor(int iSkin, float fX, float fY, float fZ, float fAngle)
{
	m_pPed = nullptr;
	m_dwGTAId = 0;
	m_bInvulnerable = false;

    if (!CStreaming::TryLoadModel(iSkin))
        throw std::runtime_error("Model not loaded");

	uint32_t dwRet;
	ScriptCommand(&create_actor, 5, iSkin, fX, fY, fZ - 1.0f, &dwRet);
	ScriptCommand(&set_actor_z_angle, dwRet, fAngle);

	m_dwGTAId = dwRet;
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);

	ScriptCommand(&set_actor_can_be_decapitated, m_dwGTAId, 0);
	ScriptCommand(&set_actor_decision_marker, m_dwGTAId, 0x10006);

    CStreaming::RemoveModelIfNoRefs(iSkin);
}
// 0.3.7
CActor::~CActor()
{
	if (m_pPed && GamePool_Ped_GetAt(m_dwGTAId) &&
		IsValidGamePed(m_pPed))
	{
		// CPlayerPed::Destructor
        // CPopulation::RemovePed
        ((void (*)(uintptr_t))(g_libGTASA + (VER_x32 ? 0x004CE6A0 + 1 : 0x5CDC64)))((uintptr_t)m_pPed);
		m_pPed = nullptr;
	}
	else
	{
		m_dwGTAId = 0;
		m_pPed = nullptr;
	}
}
// 0.3.7
void CActor::SetHealth(float fHealth)
{
	if (m_pPed) {
		m_pPed->m_fHealth = fHealth;

		if (m_pPed->m_fHealth <= 0.0f) {
			ScriptCommand(&kill_actor, m_dwGTAId);
		}
	}
}
// 0.3.7
void CActor::SetInvulnerable(bool bInvulnerable)
{
	m_bInvulnerable = bInvulnerable;

	if (bInvulnerable) {
		ScriptCommand(&set_actor_immunities, m_dwGTAId, 1, 1, 1, 1, 1);
	}
	else {
		ScriptCommand(&set_actor_immunities, m_dwGTAId, 0, 0, 0, 1, 0);
	}
}
// 0.3.7 (adapted)
void CActor::ApplyAnimation(const char* szAnimName, const char* szAnimLib, float fDelta,
	int bLoop, int bLockX, int bLockY, int bFreeze, int iTime)
{
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!strcasecmp(szAnimLib, "SEX")) return;

	if (!pGame->IsAnimationLoaded(szAnimLib)) {
		pGame->RequestAnimation(szAnimLib);
		return;
	}

	ScriptCommand(&apply_animation, m_dwGTAId, szAnimName, szAnimLib, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
}
// 0.3.7
void CActor::ClearAnimation()
{
	if (m_pPed) {
		// CPedIntelligence::FlushImmediately
		//((void (*)(PED_TASKS_TYPE*, bool))(g_libGTASA + 0x4C0A44 + 1))(m_pPed->Tasks, true);
	}
}
// 0.3.7
void CActor::SetFacingAngle(float fAngle)
{
	if (m_pPed && GamePool_Ped_GetAt(m_dwGTAId)) {
		m_pPed->m_fAimingRotation = DegToRad(fAngle);
	}
}