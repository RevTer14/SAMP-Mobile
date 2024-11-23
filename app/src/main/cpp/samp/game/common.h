#pragma once

#include <cstdint>
#include "Vector.h"
#include "game/Enums/eEntityStatus.h"
#include "game/Enums/eEntityType.h"
#include "Quaternion.h"
#include "game/Enums/eSurfaceType.h"
#include "game/Animation/AnimBlendFrameData.h"
#include "SimpleTransform.h"
#include "MatrixLink.h"

#define PADDING(x,y) uint8_t x[y]

#define VER_2_1 true

#define IN_VEHICLE(x) ((x->bInVehicle & 0x100) >> 8)
#define IS_CROUCHING(x) ((x->bIsDucking >> 26) & 1)
#define IS_FIRING(x) (x->bFiringWeapon)
#define IS_TARGETING(x) (x->bIsAimingGun)

#define ASSERT assert
#define nil nullptr

typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef intptr_t  intptr;
typedef uintptr_t uintptr;

typedef uint8     bool8;
typedef uint16    bool16;
typedef uint32    bool32;

#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

VALIDATE_SIZE(bool, 1);
VALIDATE_SIZE(char, 1);
VALIDATE_SIZE(short, 2);
VALIDATE_SIZE(int, 4);
VALIDATE_SIZE(float, 4);
VALIDATE_SIZE(long long, 8);

typedef unsigned char byte;
// Macro for unused function arguments - Use it to avoid compiler warnings of unused arguments
#define UNUSED(x) (void)(x);

constexpr float E              = 2.71828f;          // e
constexpr float E_CONST        = 0.577f;            // Euler-Mascheroni constant
constexpr float FRAC_1_TAU     = 0.159154f;         // 1 / τ
constexpr float FRAC_1_PI      = 0.318309f;         // 1 / π
constexpr float FRAC_2_TAU     = 0.318309f;         // 2 / τ
constexpr float FRAC_2_PI      = 0.636619f;         // 2 / π
constexpr float FRAC_2_SQRT_PI = 1.12837f;          // 2 / √π
constexpr float FRAC_4_TAU     = 0.636619f;         // 4 / τ
constexpr float FRAC_1_SQRT_2  = 0.707106f;         // 1 / √2
constexpr float FRAC_PI_2      = 1.57079f;          // π / 2
constexpr float FRAC_PI_3      = 1.04719f;          // π / 3
constexpr float FRAC_PI_4      = 0.785398f;         // π / 4
constexpr float FRAC_PI_6      = 0.523598f;         // π / 6
constexpr float FRAC_PI_8      = 0.392699f;         // π / 8
constexpr float FRAC_TAU_2     = 3.14159f;          // τ / 2 = π
constexpr float FRAC_TAU_3     = 2.09439f;          // τ / 3
constexpr float FRAC_TAU_4     = 1.57079f;          // τ / 4
constexpr float FRAC_TAU_6     = 1.04719f;          // τ / 6
constexpr float FRAC_TAU_8     = 0.785398f;         // τ / 8
constexpr float FRAC_TAU_12    = 0.523598f;         // τ / 12
constexpr float LN_2           = 0.693147f;         // ln(2)
constexpr float LN_10          = 2.30258f;          // ln(10)
constexpr float LOG2_E         = 1.44269f;          // log2(e)
constexpr float LOG10_E        = 0.434294f;         // log10(e)
constexpr float LOG10_2        = 0.301029f;         // log10(2)
constexpr float LOG2_10        = 3.32192f;          // log2(10)
constexpr float PI             = 3.14159f;          // π
constexpr float HALF_PI        = PI / 2.0f;         // π / 2
constexpr float PI_6           = PI / 6.0f;         // π / 6
constexpr float SQRT_2         = 1.41421f;          // √2
constexpr float SQRT_3         = 1.73205f;          // √3
constexpr float TWO_PI         = 6.28318f;          // τ (TAU)

constexpr float COS_45 = SQRT_2; // cos(45deg)

constexpr float sq(float x) { return x * x; }

enum eLights
{
	// these have to correspond to their respective panels
	LEFT_HEADLIGHT = 0,
	RIGHT_HEADLIGHT,
	LEFT_TAIL_LIGHT,
	RIGHT_TAIL_LIGHT,
	/*  LEFT_BRAKE_LIGHT,
		RIGHT_BRAKE_LIGHT,
		FRONT_LEFT_INDICATOR,
		FRONT_RIGHT_INDICATOR,
		REAR_LEFT_INDICATOR,
		REAR_RIGHT_INDICATOR,*/

	MAX_LIGHTS            // MUST BE 16 OR LESS
};

enum eDoors
{
	BONNET = 0,
	BOOT,
	FRONT_LEFT_DOOR,
	FRONT_RIGHT_DOOR,
	REAR_LEFT_DOOR,
	REAR_RIGHT_DOOR,
	MAX_DOORS
};

enum eDoorStatus
{
	DT_DOOR_INTACT = 0,
	DT_DOOR_SWINGING_FREE,
	DT_DOOR_BASHED,
	DT_DOOR_BASHED_AND_SWINGING_FREE,
	DT_DOOR_MISSING
};

enum eWheelPosition
{
	FRONT_LEFT_WHEEL = 0,
	REAR_LEFT_WHEEL,
	FRONT_RIGHT_WHEEL,
	REAR_RIGHT_WHEEL,

	MAX_WHEELS

};

#pragma pack(push, 1)
typedef struct _DAMAGE_MANAGER_INTERFACE            // 28 bytes due to the way its packed (24 containing actual data)
{
	float fWheelDamageEffect;
	uint8_t  bEngineStatus;            // old - wont be used
	uint8_t  Wheel[MAX_WHEELS];
	uint8_t  Door[MAX_DOORS];
	uint32_t Lights;            // 2 bits per light
	uint32_t Panels;            // 4 bits per panel
} DAMAGE_MANAGER_INTERFACE;
#pragma pack(pop)

enum ePanels
{
	FRONT_LEFT_PANEL = 0,
	FRONT_RIGHT_PANEL,
	REAR_LEFT_PANEL,
	REAR_RIGHT_PANEL,
	WINDSCREEN_PANEL,            // needs to be in same order as in component.h
	FRONT_BUMPER,
	REAR_BUMPER,

	MAX_PANELS            // MUST BE 8 OR LESS
};

enum eComponentStatus
{
	DT_PANEL_INTACT = 0,
	//  DT_PANEL_SHIFTED,
	DT_PANEL_BASHED,
	DT_PANEL_BASHED2,
	DT_PANEL_MISSING
};

#pragma pack(push, 1)
typedef struct _ANIMATION_DATA {
	union {
		int iValue;
		struct {
			unsigned short sId : 16;
			unsigned char cFrameDelta : 8;
			unsigned char cLoopA : 1;
			unsigned char cLoopX : 1;
			unsigned char cLoopY : 1;
			unsigned char cLoopF : 1;
			unsigned char cTime : 2;
		};
	};
} ANIMATION_DATA;
#pragma pack(pop)

//-----------------------------------------------------------

#pragma pack(push, 1)
typedef struct _WEAPON_SLOT_TYPE
{
	uint32_t dwType;
	uint32_t dwState;
	uint32_t dwAmmoInClip;
	uint32_t dwAmmo;
	PADDING(_pwep1, 12);
} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytes
#pragma pack(pop)

//-----------------------------------------------------------

#pragma pack(push, 1)
typedef struct _ENTITY_TYPE
{
	// ENTITY STUFF
    CSimpleTransform    m_placement; // sa name - m_transform
    CMatrixLink         *m_matrix;
    union {
        struct RwObject* pRwObject;
        struct RpClump*  pRwClump;
        struct RpAtomic* pRpAtomic;
    };						// 24-28	;pRWObject			- 2.0
    union {
        struct {
            /* https://github.com/multitheftauto/mtasa-blue/blob/master/Client/game_sa/CEntitySA.h */
            bool m_bUsesCollision : 1;               // does entity use collision
            bool m_bCollisionProcessed : 1;          // has object been processed by a ProcessEntityCollision function
            bool m_bIsStatic : 1;                    // is entity static
            bool m_bHasContacted : 1;                // has entity processed some contact forces
            bool m_bIsStuck : 1;                     // is entity stuck
            bool m_bIsInSafePosition : 1;            // is entity in a collision free safe position
            bool m_bWasPostponed : 1;                // was entity control processing postponed
            bool m_bIsVisible : 1;                   // is the entity visible

            bool m_bIsBIGBuilding : 1;               // Set if this entity is a big building
            bool m_bRenderDamaged : 1;               // use damaged LOD models for objects with applicable damage
            bool m_bStreamingDontDelete : 1;         // Don't let the streaming remove this
            bool m_bRemoveFromWorld : 1;             // remove this entity next time it should be processed
            bool m_bHasHitWall : 1;                  // has collided with a building (changes subsequent collisions)
            bool m_bImBeingRendered : 1;             // don't delete me because I'm being rendered
            bool m_bDrawLast : 1;                    // draw object last
            bool m_bDistanceFade : 1;                // Fade entity because it is far away

            bool m_bDontCastShadowsOn : 1;           // Don't cast shadows on this object
            bool m_bOffscreen : 1;                   // offscreen flag. This can only be trusted when it is set to true
            bool m_bIsStaticWaitingForCollision : 1; // this is used by script created entities - they are static until the collision is loaded below them
            bool m_bDontStream : 1;                  // tell the streaming not to stream me
            bool m_bUnderwater : 1;                  // this object is underwater change drawing order
            bool m_bHasPreRenderEffects : 1;         // Object has a prerender effects attached to it
            bool m_bIsTempBuilding : 1;              // whether the building is temporary (i.e. can be created and deleted more than once)
            bool m_bDontUpdateHierarchy : 1;         // Don't update the animation hierarchy this frame

            bool m_bHasRoadsignText : 1;             // entity is roadsign and has some 2dEffect text stuff to be rendered
            bool m_bDisplayedSuperLowLOD : 1;
            bool m_bIsProcObject : 1;                // set object has been generated by procedural object generator
            bool m_bBackfaceCulled : 1;              // has backface culling on
            bool m_bLightObject : 1;                 // light object with directional lights
            bool m_bUnimportantStream : 1;           // set that this object is unimportant, if streaming is having problems
            bool m_bTunnel : 1;                      // Is this model part of a tunnel
            bool m_bTunnelTransition : 1;            // This model should be rendered from within and outside the tunnel
        };
        uint32 dwProcessingFlags;
    };
    uint32 flags2;
    union {
        struct {
            uint16 m_nRandomSeedUpperByte : 8;
            uint16 m_nRandomSeedSecondByte : 8;
        };
        uint16 m_nRandomSeed;
    };
	uint16_t nModelIndex; 		// 38-40	;ModelIndex			- 2.0
    uint16_t            m_nModelIndex;
    uintptr_t           m_pReferences;
    uintptr_t           m_pStreamingLink;
    uint16_t            m_nScanCode;
    uint8_t             m_nIplIndex;
    uint8_t byteAreaCode;		// 51-52
    union {
        int32       m_nLodIndex; // -1 - without LOD model
        _ENTITY_TYPE* m_pLod;
    };
    uint8_t         m_nNumLodChildren; // numLodChildren;
    int8_t          m_nNumLodChildrenRendered; // numLodChildrenRendered;
    eEntityType     m_nType : 3;          // Mask: & 0x7  = 7
    eEntityStatus   m_nStatus : 5;        // Mask: & 0xF8 = 248 (Remember: In the original code unless this was left shifted the value it's compared to has to be left shifted by 3!)
    uint8_t         pad_0;
    float       m_fPrevDistFromCam;
    uint32_t    m_LastCollisionTime;
    union {
        struct {
            uint32 bMakeMassTwiceAsBig : 1;
            uint32 bApplyGravity : 1;
            uint32 bDisableCollisionForce : 1;
            uint32 bCollidable : 1;
            uint32 bDisableTurnForce : 1;
            uint32 bDisableMoveForce : 1;
            uint32 bInfiniteMass : 1;
            uint32 bDisableZ : 1;

            uint32 bSubmergedInWater : 1;
            uint32 bOnSolidSurface : 1;
            uint32 bBroken : 1;
            uint32 bProcessCollisionEvenIfStationary : 1; // ref @ 0x6F5CF0
            uint32 bSkipLineCol : 1;                               // only used for peds
            uint32 bDontApplySpeed : 1;
            uint32 b15 : 1;
            uint32 bProcessingShift : 1;

            uint32 b17 : 1;
            uint32 bDisableSimpleCollision : 1; // ref @ CPhysical::ProcessCollision
            uint32 bBulletProof : 1;
            uint32 bFireProof : 1;
            uint32 bCollisionProof : 1;
            uint32 bMeleeProof : 1;
            uint32 bInvulnerable : 1;
            uint32 bExplosionProof : 1;

            uint32 bDontCollideWithFlyers : 1;
            uint32 bAttachedToEntity : 1;
            uint32 bAddMovingCollisionSpeed : 1;
            uint32 bTouchingWater : 1;
            uint32 bCanBeCollidedWith : 1;
            uint32 bDestroyed : 1;
            uint32 b31 : 1;
            uint32 b32 : 1;
        } physicalFlags;
        uint32 flags;
    };
    CVector         vecMoveSpeed;
    CVector         vecTurnSpeed;
    CVector         m_vecFrictionMoveSpeed;
    CVector         m_vecFrictionTurnSpeed;
    CVector         m_vecForce;
    CVector         m_vecTorque;
    float           m_fMass;
    float           m_fTurnMass;
    float           m_fVelocityFrequency;
    float           m_fAirResistance;
    float           m_fElasticity;
    float           m_fBuoyancyConstant;
    CVector         m_vecCentreOfMass;
    uintptr_t  m_pCollisionList;

    uintptr_t m_pMovingList;

    uint8_t         m_nFakePhysics;
    uint8_t         m_nNumEntitiesCollided;
    eSurfaceType    m_nContactSurface;
    uint8_t         _pad4;

    _ENTITY_TYPE*        m_apCollidedEntities[6];
    float           m_fMovingSpeed; // m_fTrueDistanceTravelled
    float           m_fDamageIntensity; // m_fDamageImpulseMagnitude
    _ENTITY_TYPE         *m_pDamageEntity;
    CVector         m_vecLastCollisionImpactVelocity;
    CVector         m_vecLastCollisionPosn;
    uint16_t        m_nPieceType;
    uint8_t         _pad5[2];
    _ENTITY_TYPE         *m_pAttachedTo;
    CVector         m_vecAttachPosnOffset;
    CVector         m_vecAttachTiltOffset;
    CQuaternion     m_qAttachedEntityRotation;
    _ENTITY_TYPE         *m_pEntityIgnoredCollision;
    float           m_fContactSurfaceBrightness;
    float           m_fDynamicLighting;
    uintptr_t m_pShadowData;
} ENTITY_TYPE;
#pragma pack(pop)

//-----------------------------------------------------------

#pragma pack(push, 1)
typedef struct _PED_TASKS_TYPE
{
	uint32_t *pdwPed; 					// 0-4
	// Basic Tasks
	uint32_t *pdwDamage; 				// 4-8
	uint32_t *pdwFallEnterExit; 		// 8-12
	uint32_t *pdwSwimWasted;		 	// 12-16
	uint32_t *pdwJumpJetPack; 			// 16-20
	uint32_t *pdwAction; 				// 20-24
	// Extended Tasks
	uint32_t *pdwFighting; 				// 24-28
	uint32_t *pdwCrouching; 			// 28-32
	uint32_t *pdwExtUnk1; 				// 32-36
	uint32_t *pdwExtUnk2; 				// 36-40
	uint32_t *pdwExtUnk3; 				// 40-44
	uint32_t *pdwExtUnk4; 				// 44-48
} PED_TASKS_TYPE;
#pragma pack(pop)

class CTaskManager
{
public:
	void* m_aPrimaryTasks[5];
	void* m_aSecondaryTasks[6];
	class CPed* m_pPed;
};

class CPedIntelligence
{
public:
	class CPed* m_pPed;
	CTaskManager   m_TaskMgr;
};

#pragma pack(push, 1)
typedef struct _PED_TYPE
{
	ENTITY_TYPE entity; 				// 0000-0188	;entity				- 2.0
	PADDING(_pad100, 896);				// 0188-1084
	CPedIntelligence* pPedIntelligence; // 1084-1088
	PED_TASKS_TYPE *Tasks; 				// 1088-1092
	uintptr_t dwPlayerInfoOffset;		// 1092-1096	;dwPlayerInfoOffset - 2.0
	PADDING(_pad106, 4);				// 1096-1100
	uint32_t dwAction;					// 1100-1104	;Action				- 2.0
	PADDING(_pad101, 52);				// 1104-1156
    union {
        /* https://github.com/multitheftauto/mtasa-blue/blob/master/Client/game_sa/CPedSA.h */
        struct {
            // 1st byte starts here (m_nPedFlags)
            bool bIsStanding : 1 = false;            // is ped standing on something
            bool bWasStanding : 1 = false;           // was ped standing on something
            bool bIsLooking : 1 = false;             // is ped looking at something or in a direction
            bool bIsRestoringLook : 1 = false;       // is ped restoring head position from a look
            bool bIsAimingGun : 1 = false;           // is ped aiming gun
            bool bIsRestoringGun : 1 = false;        // is ped moving gun back to default posn
            bool bCanPointGunAtTarget : 1 = false;   // can ped point gun at target
            bool bIsTalking : 1 = false;             // is ped talking(see Chat())

            bool bInVehicle : 1 = false;             // is in a vehicle
            bool bIsInTheAir : 1 = false;            // is in the air
            bool bIsLanding : 1 = false;             // is landing after being in the air
            bool bHitSomethingLastFrame : 1 = false; // has been in a collision last frame
            bool bIsNearCar : 1 = false;             // has been in a collision last frame
            bool bRenderPedInCar : 1 = true;         // has been in a collision last frame
            bool bUpdateAnimHeading : 1 = false;     // update ped heading due to heading change during anim sequence
            bool bRemoveHead : 1 = false;            // waiting on AntiSpazTimer to remove head - TODO: See `RemoveBodyPart` - The name seems to be incorrect. It should be like `bHasBodyPartToRemove`.

            bool bFiringWeapon : 1 = false;         // is pulling trigger
            bool bHasACamera : 1;                   // does ped possess a camera to document accidents
            bool bPedIsBleeding : 1 = false;        // Ped loses a lot of blood if true
            bool bStopAndShoot : 1 = false;         // Ped cannot reach target to attack with fist, need to use gun
            bool bIsPedDieAnimPlaying : 1 = false;  // is ped die animation finished so can dead now
            bool bStayInSamePlace : 1 = false;      // when set, ped stays put
            bool bKindaStayInSamePlace : 1 = false; // when set, ped doesn't seek out opponent or cover large distances. Will still shuffle and look for cover
            bool bBeingChasedByPolice : 1 = false;  // use nodes for route find

            bool bNotAllowedToDuck : 1 = false;     // Is this ped allowed to duck at all?
            bool bCrouchWhenShooting : 1 = false;   // duck behind cars etc
            bool bIsDucking : 1 = false;            // duck behind cars etc
            bool bGetUpAnimStarted : 1 = false;     // don't want to play getup anim if under something
            bool bDoBloodyFootprints : 1 = false;   // bIsLeader
            bool bDontDragMeOutCar : 1 = false;
            bool bStillOnValidPoly : 1 = false;     // set if the polygon the ped is on is still valid for collision
            bool bAllowMedicsToReviveMe : 1 = true;

            // 5th byte starts here (m_nSecondPedFlags)
            bool bResetWalkAnims : 1 = false;
            bool bOnBoat : 1 = false;               // flee but only using nodes
            bool bBusJacked : 1 = false;            // flee but only using nodes
            bool bFadeOut : 1 = false;              // set if you want ped to fade out
            bool bKnockedUpIntoAir : 1 = false;     // has ped been knocked up into the air by a car collision
            bool bHitSteepSlope : 1 = false;        // has ped collided/is standing on a steep slope (surface type)
            bool bCullExtraFarAway : 1 = false;     // special ped only gets culled if it's extra far away (for roadblocks)
            bool bTryingToReachDryLand : 1 = false; // has ped just exited boat and trying to get to dry land

            bool bCollidedWithMyVehicle : 1 = false;
            bool bRichFromMugging : 1 = false;        // ped has lots of cash cause they've been mugging people
            bool bChrisCriminal : 1 = false;          // Is a criminal as killed during Chris' police mission (should be counted as such)
            bool bShakeFist : 1 = false;              // test shake hand at look entity
            bool bNoCriticalHits : 1 = false;         // ped cannot be killed by a single bullet
            bool bHasAlreadyBeenRecorded : 1 = false; // Used for replays
            bool bUpdateMatricesRequired : 1 = false; // if PedIK has altered bones so matrices need updated this frame
            bool bFleeWhenStanding : 1 = false;       //

            bool bMiamiViceCop : 1 = false;
            bool bMoneyHasBeenGivenByScript : 1 = false;
            bool bHasBeenPhotographed : 1 = false;
            bool bIsDrowning : 1 = false;
            bool bDrownsInWater : 1 = true;
            bool bHeadStuckInCollision : 1 = false;
            bool bDeadPedInFrontOfCar : 1 = false;
            bool bStayInCarOnJack : 1 = false;

            bool bDontFight : 1 = false;
            bool bDoomAim : 1 = true;
            bool bCanBeShotInVehicle : 1 = true;
            bool bPushedAlongByCar : 1 = false; // ped is getting pushed along by car collision (so don't take damage from horz velocity)
            bool bNeverEverTargetThisPed : 1 = false;
            bool bThisPedIsATargetPriority : 1 = false;
            bool bCrouchWhenScared : 1 = false;
            bool bKnockedOffBike : 1 = false; // TODO: Maybe rename to `bIsJumpingOut` or something similar, see x-refs

            // 9th byte starts here (m_nThirdPedFlags)
            bool bDonePositionOutOfCollision : 1 = false;
            bool bDontRender : 1 = false;
            bool bHasBeenAddedToPopulation : 1 = false;
            bool bHasJustLeftCar : 1 = false;
            bool bIsInDisguise : 1 = false;
            bool bDoesntListenToPlayerGroupCommands : 1 = false;
            bool bIsBeingArrested : 1 = false;
            bool bHasJustSoughtCover : 1 = false;

            bool bKilledByStealth : 1 = false;
            bool bDoesntDropWeaponsWhenDead : 1 = false;
            bool bCalledPreRender : 1 = false;
            bool bBloodPuddleCreated : 1 = false; // Has a static puddle of blood been created yet
            bool bPartOfAttackWave : 1 = false;
            bool bClearRadarBlipOnDeath : 1 = false;
            bool bNeverLeavesGroup : 1 = false;        // flag that we want to test 3 extra spheres on col model
            bool bTestForBlockedPositions : 1 = false; // this sets these indicator flags for various positions on the front of the ped

            bool bRightArmBlocked : 1 = false;
            bool bLeftArmBlocked : 1 = false;
            bool bDuckRightArmBlocked : 1 = false;
            bool bMidriffBlockedForJump : 1 = false;
            bool bFallenDown : 1 = false;
            bool bUseAttractorInstantly : 1 = false;
            bool bDontAcceptIKLookAts : 1 = false;
            bool bHasAScriptBrain : 1 = false;

            bool bWaitingForScriptBrainToLoad : 1 = false;
            bool bHasGroupDriveTask : 1 = false;
            bool bCanExitCar : 1 = true;
            bool CantBeKnockedOffBike : 2 = false; // (harder for mission peds)   normal(also for mission peds)
            bool bHasBeenRendered : 1 = false;
            bool bIsCached : 1 = false;
            bool bPushOtherPeds : 1 = false;   // GETS RESET EVERY FRAME - SET IN TASK: want to push other peds around (eg. leader of a group or ped trying to get in a car)

            // 13th byte starts here (m_nFourthPedFlags)
            bool bHasBulletProofVest : 1 = false;
            bool bUsingMobilePhone : 1 = false;
            bool bUpperBodyDamageAnimsOnly : 1 = false;
            bool bStuckUnderCar : 1 = false;
            bool bKeepTasksAfterCleanUp : 1 = false; // If true ped will carry on with task even after cleanup
            bool bIsDyingStuck : 1 = false;
            bool bIgnoreHeightCheckOnGotoPointTask : 1 = false; // set when walking round buildings, reset when task quits
            bool bForceDieInCar : 1 = false;

            bool bCheckColAboveHead : 1 = false;
            bool bIgnoreWeaponRange : 1 = false;
            bool bDruggedUp : 1 = false;
            bool bWantedByPolice : 1 = false; // if this is set, the cops will always go after this ped when they are doing a KillCriminal task
            bool bSignalAfterKill : 1 = true;
            bool bCanClimbOntoBoat : 1 = false;
            bool bPedHitWallLastFrame : 1 = false; // useful to store this so that AI knows (normal will still be available)
            bool bIgnoreHeightDifferenceFollowingNodes : 1 = false;

            bool bMoveAnimSpeedHasBeenSetByTask : 1 = false;
            bool bGetOutUpsideDownCar : 1 = true;
            bool bJustGotOffTrain : 1 = false;
            bool bDeathPickupsPersist : 1 = false;
            bool bTestForShotInVehicle : 1 = false;
            bool bUsedForReplay : 1 = false; // This ped is controlled by replay and should be removed when replay is done.
        };
        uint32_t dwStateFlags;
    };
	PADDING(_pad102, 12);				// 1160-1172
	AnimBlendFrameData* m_pPedBones[19];// 1172-1248
	PADDING(_pad174, 100);				// 1248-1348
	float fHealth;		 				// 1348-1352	;Health				- 2.0
	float fMaxHealth;					// 1352-1356	;MaxHealth			- 2.0
	float fArmour;						// 1356-1360	;Armour				- 2.0
	PADDING(_pad103, 12);				// 1360-1372
	float fRotation1;					// 1372-1376	;Rotation1			- 2.0
	float fRotation2;					// 1376-1380	;Rotation2			- 2.0
	PADDING(_pad104, 44);				// 1380-1424
	uintptr_t pVehicle;					// 1424-1428	;pVehicle			- 2.0
	PADDING(_pad105, 8);				// 1428-1436
	uint32_t dwPedType;					// 1436-1440	;dwPedType			- 2.0
	PADDING(_pad107, 4);				// 1440-1444
	WEAPON_SLOT_TYPE WeaponSlots[13];	// 1444-1808	;WeaponSlots		- 2.0
	PADDING(_pad108, 12);				// 1808-1820
	uint8_t byteCurWeaponSlot;			// 1820-1821	;byteCurWeaponSlot	- 2.0
	PADDING(_pad109, 75);				// 1821-1896
	uint32_t dwWeaponUsed;				// 1896-1900	;dwWeaponUsed		- 2.0
	ENTITY_TYPE* pdwDamageEntity;		// 1900-1904	;pdwDamageEntity
} PED_TYPE;
#pragma pack(pop)

struct VehicleAudioPropertiesStruct
{
	int16_t VehicleType;		//	1: +  0
	int16_t EngineOnSound;	//  2: +  2
	int16_t EngineOffSound;	//  3: +  4
	int16_t field_4;			//  4: +  6
	float field_5;			//  5: +  8
	float field_6;			//  6: + 12
	char HornTon;				//  7: + 16
	char field_8[3];			//	8: + 17, zeros
	float HornHigh;			//  9: + 20
	char DoorSound;			// 10: + 24
	char field_11[1];			// 11: + 25, zeros
	char RadioNum;			// 12: + 26
	char RadioType;			// 13: + 27
	char field_14;			// 14: + 28
	char field_15[3];			// 15: + 29, zeros
	float field_16;			// 16: + 32
};
//-----------------------------------------------------------

#pragma pack(push, 1)
typedef struct _VEHICLE_TYPE
{
	ENTITY_TYPE entity;				// 0000-0188	;entity
	PADDING(_pad99_9, 128);				// 0188-0316
	uintptr_t pVehicleAudio;			// 0316-0320
	PADDING(_pad200, 748);			// 0320-1068
	union {
		uint8_t byteFlags;				// 1068-1076	;byteFlags
		struct {
			unsigned char bIsLawEnforcer : 1;
			unsigned char bIsAmbulanceOnDuty : 1;
			unsigned char bIsFireTruckOnDuty : 1;
			unsigned char bIsLocked : 1;
			unsigned char bEngineOn : 1;
			unsigned char bIsHandbrakeOn : 1;
			unsigned char bLightsOn : 1;
			unsigned char bFreebies : 1;

			unsigned char bIsVan : 1;
			unsigned char bIsBus : 1;
			unsigned char bIsBig : 1;
			unsigned char bLowVehicle : 1;
			unsigned char bComedyControls : 1;
			unsigned char bWarnedPeds : 1;
			unsigned char bCraneMessageDone : 1;
			unsigned char bTakeLessDamage : 1;

			unsigned char bIsDamaged : 1;
			unsigned char bHasBeenOwnedByPlayer : 1;
			unsigned char bFadeOut : 1;
			unsigned char bIsBeingCarJacked : 1;
			unsigned char bCreateRoadBlockPeds : 1;
			unsigned char bCanBeDamaged : 1;
			unsigned char bOccupantsHaveBeenGenerated : 1;
			unsigned char bGunSwitchedOff : 1;

			unsigned char bVehicleColProcessed : 1;
			unsigned char bIsCarParkVehicle : 1;
			unsigned char bHasAlreadyBeenRecorded : 1;
			unsigned char bPartOfConvoy : 1;
			unsigned char bHeliMinimumTilt : 1;
			unsigned char bAudioChangingGear : 1;
			unsigned char bIsDrowning : 1;
			unsigned char bTyresDontBurst : 1;

			unsigned char bCreatedAsPoliceVehicle : 1;
			unsigned char bRestingOnPhysical : 1;
			unsigned char bParking : 1;
			unsigned char bCanPark : 1;
			unsigned char bFireGun : 1;
			unsigned char bDriverLastFrame : 1;
			unsigned char bNeverUseSmallerRemovalRange : 1;
			unsigned char bIsRCVehicle : 1;

			unsigned char bAlwaysSkidMarks : 1;
			unsigned char bEngineBroken : 1;
			unsigned char bVehicleCanBeTargetted : 1;
			unsigned char bPartOfAttackWave : 1;
			unsigned char bWinchCanPickMeUp : 1;
			unsigned char bImpounded : 1;
			unsigned char bVehicleCanBeTargettedByHS : 1;
			unsigned char bSirenOrAlarm : 1;

			unsigned char bHasGangLeaningOn : 1;
			unsigned char bGangMembersForRoadBlock : 1;
			unsigned char bDoesProvideCover : 1;
			unsigned char bMadDriver : 1;
			unsigned char bUpgradedStereo : 1;
			unsigned char bConsideredByPlayer : 1;
			unsigned char bPetrolTankIsWeakPoint : 1;
			unsigned char bDisableParticles : 1;

			unsigned char bHasBeenResprayed : 1;
			unsigned char bUseCarCheats : 1;
			unsigned char bDontSetColourWhenRemapping : 1;
			unsigned char bUsedForReplay : 1;
		} dwFlags;
	};

	uint32_t dwCreationTime;			// 1076-1080
	uint8_t byteColor1;				// 1080-1081	;byteColor1			- 2.0
	uint8_t byteColor2;				// 1081-1082	;byteColor2			- 2.0
	uint8_t byteColor3;				// 1082-1083	;byteColor3			- 2.0
	uint8_t byteColor4;				// 1083-1084	;byteColor4			- 2.0
	PADDING(_pad206, 36);			// 1084-1124
	uint16_t wAlarmState;			// 1120-1122	;wAlarmState		- 2.0
	PADDING(_pad207, 2);			// 1122-1124
	PED_TYPE *pDriver;				// 1124-1128	;driver				- 2.0
	PED_TYPE *pPassengers[7];		// 1128-1156	;pPassengers		- 2.0
	PADDING(_pad201, 8);			// 1156-1164
	uint8_t byteMaxPassengers;		// 1164-1165	;byteMaxPassengers	- 2.0
	PADDING(_pad236, 7); 			// 1165-1172
	uint32_t pFireObject;			// 1172-1176
	PADDING(_pad241__, 20); 		// 1176-1196
	uint8_t byteMoreFlags;			// 1196-1197
	PADDING(_pad275_, 31); 			// 1197-1228
	float fHealth;					// 1228-1232	;fHealth			- 2.0
	_VEHICLE_TYPE* pTractor;		// 1232-1236	;pTractor			- 2.0
	_VEHICLE_TYPE* pTrailer;		// 1236-1240	;pTrailer			- 2.0
	PADDING(_pad208, 48);			// 1240-1288
	uint32_t dwDoorsLocked;			// 1288-1292	;dwDoorsLocked		- 2.0
	PADDING(_pad202, 172);			// 1292-1464
	union {
		struct {
			PADDING(_pad245, 1);		// 1464-1465
			uint8_t byteWheelStatus[4]; // 1465-1469
			uint8_t byteDoorStatus[6];	// 1469-1475
			uint8_t byteDamageUnk;		// 1475-1476
			uint32_t dwLightStatus;		// 1476-1480
			uint32_t dwPanelStatus;		// 1480-1484
		};
		struct {
			float fTrainSpeed;			// 1464-1468
			PADDING(_pad212, 16);		// 1468-1484
		};
	};
	PADDING(_pad213, 28);				// 1484-1512
	_VEHICLE_TYPE* pNextCarriage;	// 1512-1516	;pNextCarriage		- 2.0
	PADDING(_pad211, 112);				// 1516-1628
	float fBikeLean;					// 1628-1632
	uint32_t dwBikeUnk;					// 1632-1636
	PADDING(pad211_, 12);				// 1636-1648
	uint8_t byteBikeWheelStatus[2];		// 1648-1650
	PADDING(_pad21331, 526);			// 1650-2176
	uint16_t wHydraThrusters;		// 2176-2178	;wHydraThrusters
	PADDING(_pad245123, 350);  			// 2178-2528
	float fPlaneLandingGear;			// 2528-2532
} VEHICLE_TYPE;
#pragma pack(pop)

struct BULLET_SYNC
{
    uint8_t hitType;
    uint16_t hitId;
    float origin[3];
    float hitPos[3];
    float offsets[3];
    uint8_t weapId;
};

#pragma pack(push, 1)
typedef struct _REMOVEBUILDING_DATA {
	uint32_t dwModel;
	CVector vecPos;
	float fRange;
} REMOVEBUILDING_DATA;
#pragma pack(pop)

//-----------------------------------------------------------

#define	VEHICLE_SUBTYPE_CAR				1
#define	VEHICLE_SUBTYPE_BIKE			2
#define	VEHICLE_SUBTYPE_HELI			3
#define	VEHICLE_SUBTYPE_BOAT			4
#define	VEHICLE_SUBTYPE_PLANE			5
#define	VEHICLE_SUBTYPE_PUSHBIKE		6
#define	VEHICLE_SUBTYPE_TRAIN			7

//-----------------------------------------------------------

#define TRAIN_PASSENGER_LOCO			538
#define TRAIN_FREIGHT_LOCO				537
#define TRAIN_PASSENGER					570
#define TRAIN_FREIGHT					569
#define TRAIN_TRAM						449
#define HYDRA							520

//-----------------------------------------------------------

#define ACTION_WASTED					55
#define ACTION_DEATH					54
#define ACTION_INCAR					50
#define ACTION_NORMAL					1
#define ACTION_SCOPE					12
#define ACTION_NONE						0 

//-----------------------------------------------------------

#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_GOLFCLUB					2
#define WEAPON_NITESTICK				3
#define WEAPON_KNIFE					4
#define WEAPON_BAT						5
#define WEAPON_SHOVEL					6
#define WEAPON_POOLSTICK				7
#define WEAPON_KATANA					8
#define WEAPON_CHAINSAW					9
#define WEAPON_DILDO					10
#define WEAPON_DILDO2					11
#define WEAPON_VIBRATOR					12
#define WEAPON_VIBRATOR2				13
#define WEAPON_FLOWER					14
#define WEAPON_CANE						15
#define WEAPON_GRENADE					16
#define WEAPON_TEARGAS					17
#define WEAPON_MOLTOV					18
#define WEAPON_ROCKET					19
#define WEAPON_ROCKET_HS				20
#define WEAPON_FREEFALLBOMB				21
#define WEAPON_COLT45					22
#define WEAPON_SILENCED					23
#define WEAPON_DEAGLE					24
#define WEAPON_SHOTGUN					25
#define WEAPON_SAWEDOFF					26
#define WEAPON_SHOTGSPA					27
#define WEAPON_UZI						28
#define WEAPON_MP5						29
#define WEAPON_AK47						30
#define WEAPON_M4						31
#define WEAPON_TEC9						32
#define WEAPON_RIFLE					33
#define WEAPON_SNIPER					34
#define WEAPON_ROCKETLAUNCHER			35
#define WEAPON_HEATSEEKER				36
#define WEAPON_FLAMETHROWER				37
#define WEAPON_MINIGUN					38
#define WEAPON_SATCHEL					39
#define WEAPON_BOMB						40
#define WEAPON_SPRAYCAN					41
#define WEAPON_FIREEXTINGUISHER			42
#define WEAPON_CAMERA					43
#define WEAPON_NIGHTVISION				44
#define WEAPON_INFRARED					45
#define WEAPON_PARACHUTE				46
#define WEAPON_ARMOUR					47
#define WEAPON_VEHICLE					49
#define WEAPON_HELIBLADES				50
#define WEAPON_EXPLOSION				51
#define WEAPON_DROWN					53
#define WEAPON_COLLISION				54

//-----------------------------------------------------------

#define WEAPON_MODEL_BRASSKNUCKLE		331 // was 332
#define WEAPON_MODEL_GOLFCLUB			333
#define WEAPON_MODEL_NITESTICK			334
#define WEAPON_MODEL_KNIFE				335
#define WEAPON_MODEL_BAT				336
#define WEAPON_MODEL_SHOVEL				337
#define WEAPON_MODEL_POOLSTICK			338
#define WEAPON_MODEL_KATANA				339
#define WEAPON_MODEL_CHAINSAW			341
#define WEAPON_MODEL_DILDO				321
#define WEAPON_MODEL_DILDO2				322
#define WEAPON_MODEL_VIBRATOR			323
#define WEAPON_MODEL_VIBRATOR2			324
#define WEAPON_MODEL_FLOWER				325
#define WEAPON_MODEL_CANE				326
#define WEAPON_MODEL_GRENADE			342 // was 327
#define WEAPON_MODEL_TEARGAS			343 // was 328
#define WEAPON_MODEL_MOLOTOV			344 // was 329
#define WEAPON_MODEL_COLT45				346
#define WEAPON_MODEL_SILENCED			347
#define WEAPON_MODEL_DEAGLE				348
#define WEAPON_MODEL_SHOTGUN			349
#define WEAPON_MODEL_SAWEDOFF			350
#define WEAPON_MODEL_SHOTGSPA			351
#define WEAPON_MODEL_UZI				352
#define WEAPON_MODEL_MP5				353
#define WEAPON_MODEL_AK47				355
#define WEAPON_MODEL_M4					356
#define WEAPON_MODEL_TEC9				372
#define WEAPON_MODEL_RIFLE				357
#define WEAPON_MODEL_SNIPER				358
#define WEAPON_MODEL_ROCKETLAUNCHER		359
#define WEAPON_MODEL_HEATSEEKER			360
#define WEAPON_MODEL_FLAMETHROWER		361
#define WEAPON_MODEL_MINIGUN			362
#define WEAPON_MODEL_SATCHEL			363
#define WEAPON_MODEL_BOMB				364
#define WEAPON_MODEL_SPRAYCAN			365
#define WEAPON_MODEL_FIREEXTINGUISHER	366
#define WEAPON_MODEL_CAMERA				367
#define WEAPON_MODEL_NIGHTVISION		368	// newly added
#define WEAPON_MODEL_INFRARED			369	// newly added
#define WEAPON_MODEL_JETPACK			370	// newly added
#define WEAPON_MODEL_PARACHUTE			371

#define OBJECT_PARACHUTE				3131
#define OBJECT_CJ_CIGGY					1485
#define OBJECT_DYN_BEER_1				1486
#define OBJECT_CJ_BEER_B_2				1543
#define OBJECT_CJ_PINT_GLASS			1546