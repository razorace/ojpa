//[CoOp]
//[SPPortComplete]
// leave this line at the top of all AI_xxxx.cpp files for PCH reasons...
#include "b_local.h"
//#include "Q3_Interface.h"

extern qboolean NPC_CheckSurrender( void );
extern void NPC_BehaviorSet_Default( int bState );

void NPC_BSCivilian_Default( int bState )
{
	if ( NPC->enemy
		&& NPC->s.weapon == WP_NONE 
		&& NPC_CheckSurrender() )
	{//surrendering, do nothing
	}
	else if ( NPC->enemy 
		&& NPC->s.weapon == WP_NONE 
		&& bState != BS_HUNT_AND_KILL 
		&& !trap_ICARUS_TaskIDPending( NPC, TID_MOVE_NAV ) )
	{//if in battle and have no weapon, run away, fixme: when in BS_HUNT_AND_KILL, they just stand there
		if ( !NPCInfo->goalEntity
			|| bState != BS_FLEE //not fleeing
			//[CoOp]
			//RAFIXME - Impliment Nav code in that function
			|| ( NPC->enemy//still have enemy (NPC_BSFlee checks enemy and can clear it)
			//|| ( NPC_BSFlee()//have reached our flee goal
			//	&& NPC->enemy//still have enemy (NPC_BSFlee checks enemy and can clear it)
			//[/CoOp]			
				&& DistanceSquared( NPC->r.currentOrigin, NPC->enemy->r.currentOrigin ) < 16384 )//enemy within 128
			)
		{//run away!
			NPC_StartFlee( NPC->enemy, NPC->enemy->r.currentOrigin, AEL_DANGER_GREAT, 5000, 10000 );
		}
	}
	else
	{//not surrendering
		//FIXME: if unarmed and a jawa/ugnuaght, constantly look for enemies/players to run away from?
		//FIXME: if we have a weapon and an enemy, set out playerTeam to the opposite of our enemy..???
		NPC_BehaviorSet_Default(bState);
	}
	if ( !VectorCompare( NPC->client->ps.moveDir, vec3_origin ) )
	{//moving
		if ( NPC->client->ps.legsAnim == BOTH_COWER1 )
		{//stop cowering anim on legs
			NPC->client->ps.legsTimer = 0;
		}
	}
}
//[/SPPortComplete]
//[/CoOp]
