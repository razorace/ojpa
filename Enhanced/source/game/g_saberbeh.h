//This file contains functions relate to the saber impact behavior of OJP Enhanced's saber system.
#ifndef _SABERBEH_H
#define _SABERBEH_H

#include "q_shared.h"

//[SaberSys]
//This is the attack parry rate for the bots since bots don't block intelligently
//This is multipled by the bot's skill level (which can be 1-5) and is actually a percentage of the total parries as
//set by BOT_PARRYRATE.
#define BOT_ATTACKPARRYRATE			20

//////////////////////////////////////////////////////////////////////////
//Mishap Costs (negative values mean the player recovers points)
#define MPCOST_BLOCKED_SUPERBREAK						2		//superbreak was blocked
#define MPCOST_BLOCKING_SUPERBREAK					-1	//saber blocking an attack
#define MPCOST_BLOCKED_ATTACK								1		//standard attack blocked
#define MPCOST_BLOCKING_ATTACK							-1	//blocking standard attack
#define MPCOST_BLOCKING_TAVION_ATTACK				2		//blocking tavion standard attacks causes MP damage
#define MPCOST_BLOCKED_ATTACK_FAKE					-1	//attack fake was blocked, actually regens mishap because you're keeping your opponent off guard
#define MPCOST_ATTACK_ATTACK								1		//both sabers attacking
#define MPCOST_ATTACK_FAKE_ATTACKED					1		//saber was attacked during an attack fake
#define MPCOST_ATTACKING_ATTACK_FAKE				0	//attacked an attack fake
#define MPCOST_PARRIED											3		//getting parried
#define MPCOST_PARRIED_ATTACKFAKE						4		//attack fake getting parried
#define MPCOST_PARRYING											-3	//parrying in general
#define MPCOST_PARRYING_ATTACKFAKE					-4	//MP you deplete by parrying an attackfake
#define MPCOST_BLOCKING_ATTACK_FAKE					0		//blocking an attack fake
#define MPCOST_BLOCKING_DESANN_ATTACK_FAKE	2		//Desann attack fakes actually deal MP damage to blocker.
#define MPCOST_TAVION_ATTACK_SLOW_BOUNCE		-3	//Tavion attacking a slow bounce recovers MP
#define MPCOST_QUICKPARRY_TAVION_ATTACK			2		//quick parrying a tavion attack (doesn't happen. MPCOST_BLOCKING_TAVION_ATTACK is applied instead)

//Force Power Costs
#define FPCOST_BLOCKING_STRONG_ATTACK				2		//Blocking a strong attack is exhausting
//////////////////////////////////////////////////////////////////////////

//This struct holds all the relevant saber mechanics data
struct sabmech_s {
	//Do Knockaway animation
	qboolean doStun;

	//Do knockdown animation
	qboolean doKnockdown;

	//Do butterFingers (disarment)
	qboolean doDisarm;

	//did a parry (do knockaway animation)
	qboolean doParry;

	qboolean doSlowBounce;

	qboolean doHeavySlowBounce;

	int behaveMode;
};

typedef struct sabmech_s sabmech_t;
char* DebugPrintSabMech(sabmech_t* sabmech, char* output, int bufferSize);

typedef enum
{//used to debug saber behavior.  
//Shows (with behaveMode) which mishap probability set was used for saber behavior calculations.
	SABBEHAVE_NONE = 0,
	SABBEHAVE_ATTACK,
	SABBEHAVE_ATTACKPARRIED,
	SABBEHAVE_ATTACKBLOCKED,
	SABBEHAVE_BLOCK,
	SABBEHAVE_BLOCKFAKED,
	SABBEHAVE_PARRY,
} sabBehave_t;
extern stringID_table_t sabBehaveTable[SABBEHAVE_PARRY+1];


void SabBeh_RunSaberBehavior( gentity_t *self, sabmech_t *mechSelf, 
								gentity_t *otherOwner, sabmech_t *mechOther, vec3_t hitLoc, 
								qboolean *didHit, qboolean otherHitSaberBlade );
void SabBeh_AnimateSlowBounce(gentity_t* self, gentity_t *inflictor);
void SabBeh_AnimateHeavySlowBounce(gentity_t*, gentity_t *inflictor);
#endif

