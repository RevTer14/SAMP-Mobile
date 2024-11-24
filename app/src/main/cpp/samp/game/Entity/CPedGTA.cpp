//
// Created by roman on 11/24/2024.
//

#include "CPedGTA.h"
#include "patch.h"

void CPedGTA::GetBonePosition(RwV3d *posn, uint32 boneTag, bool bCalledFromCamera) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x004A4B0C + 1 : 0x59AEE4), this, posn, boneTag, bCalledFromCamera);
}