//[SPPortComplete]
//[CoOp]
//this entire file is a port of the SP version of this code.
// leave this line at the top of all AI_xxxx.cpp files for PCH reasons...

#include "b_local.h"
#include "g_nav.h"
#include "anims.h"

//extern void G_AddVoiceEvent( gentity_t *self, int event, int speakDebounceTime );
extern void WP_DeactivateSaber( gentity_t *self, qboolean clearLength );
extern int PM_AnimLength( int index, animNumber_t anim );

qboolean NPC_CheckPlayerTeamStealth( void );

static qboolean enemyLOS;
static qboolean enemyCS;
static qboolean faceEnemy;
static qboolean move;
static qboolean shoot;
static float	enemyDist;

/*
-------------------------
ST_Move
-------------------------
*/

static qboolean SaberDroid_Move( void )
{//movement while in attack move.
	qboolean	moved;
	NPCInfo->combatMove = qtrue;//always move straight toward our goal
	UpdateGoal();
	if ( !NPCInfo->goalEntity )
	{
		NPCInfo->goalEntity = NPC->enemy;
	}
	NPCInfo->goalRadius = 30.0f;

	moved = NPC_MoveToGoal( qtrue );
//	navInfo_t	info;
	
	//Get the move info
//	NAV_GetLastMove( info );

	//FIXME: if we bump into another one of our guys and can't get around him, just stop!
	//If we hit our target, then stop and fire!
//	if ( info.flags & NIF_COLLISION ) 
//	{
//		if ( info.blocker == NPC->enemy )
//		{
//			SaberDroid_HoldPosition();
//		}
//	}

	//If our move failed, then reset
	/*
	if ( moved == qfalse )
	{//couldn't get to enemy
		//just hang here
		SaberDroid_HoldPosition();
	}
	*/

	return moved;
}

/*
-------------------------
NPC_BSSaberDroid_Patrol
-------------------------
*/

void NPC_BSSaberDroid_Patrol( void )
{//FIXME: pick up on bodies of dead buddies?
	if ( NPCInfo->confusionTime < level.time )
	{//not confused by mindtrick
		//Look for any enemies
		if ( NPCInfo->scriptFlags&SCF_LOOK_FOR_ENEMIES )
		{
			if ( NPC_CheckPlayerTeamStealth() )
			{//found an enemy
				//NPCInfo->behaviorState = BS_HUNT_AND_KILL;//should be automatic now
				//NPC_AngerSound();
				NPC_UpdateAngles( qtrue, qtrue );
				return;
			}
		}

		if ( !(NPCInfo->scriptFlags&SCF_IGNORE_ALERTS) )
		{//alert reaction behavior.
			//Is there danger nearby
			//[CoOp]
			int alertEvent = NPC_CheckAlertEvents( qtrue, qtrue, -1, qfalse, AEL_SUSPICIOUS, qfalse );
			//int alertEvent = NPC_CheckAlertEvents( qtrue, qtrue, -1, qfalse, AEL_SUSPICIOUS );
			//[/CoOp]
			//There is an event to look at
			if ( alertEvent >= 0 )//&& level.alertEvents[alertEvent].ID != NPCInfo->lastAlertID )
			{
				//NPCInfo->lastAlertID = level.alertEvents[alertEvent].ID;
				if ( level.alertEvents[alertEvent].level >= AEL_DISCOVERED )
				{
					if ( level.alertEvents[alertEvent].owner && 
						level.alertEvents[alertEvent].owner->client && 
						level.alertEvents[alertEvent].owner->health >= 0 &&
						level.alertEvents[alertEvent].owner->client->playerTeam == NPC->client->enemyTeam )
					{//an enemy
						G_SetEnemy( NPC, level.alertEvents[alertEvent].owner );
						//NPCInfo->enemyLastSeenTime = level.time;
						TIMER_Set( NPC, "attackDelay", Q_irand( 500, 2500 ) );
					}
				}
				else
				{//FIXME: get more suspicious over time?
					//Save the position for movement (if necessary)
					VectorCopy( level.alertEvents[alertEvent].position, NPCInfo->investigateGoal );
					NPCInfo->investigateDebounceTime = level.time + Q_irand( 500, 1000 );
					if ( level.alertEvents[alertEvent].level == AEL_SUSPICIOUS )
					{//suspicious looks longer
						NPCInfo->investigateDebounceTime += Q_irand( 500, 2500 );
					}
				}
			}

			if ( NPCInfo->investigateDebounceTime > level.time )
			{//FIXME: walk over to it, maybe?  Not if not chase enemies
				//NOTE: stops walking or doing anything else below
				vec3_t	dir, angles;
				float	o_yaw, o_pitch;
				
				VectorSubtract( NPCInfo->investigateGoal, NPC->client->renderInfo.eyePoint, dir );
				vectoangles( dir, angles );
				
				o_yaw = NPCInfo->desiredYaw;
				o_pitch = NPCInfo->desiredPitch;
				NPCInfo->desiredYaw = angles[YAW];
				NPCInfo->desiredPitch = angles[PITCH];
				
				NPC_UpdateAngles( qtrue, qtrue );

				NPCInfo->desiredYaw = o_yaw;
				NPCInfo->desiredPitch = o_pitch;
				return;
			}
		}
	}

	//If we have somewhere to go, then do that
	if ( UpdateGoal() )
	{
		ucmd.buttons |= BUTTON_WALKING;
		NPC_MoveToGoal( qtrue );
	}
	else if ( !NPC->client->ps.weaponTime
		&& TIMER_Done( NPC, "attackDelay" )
		&& TIMER_Done( NPC, "inactiveDelay" ) )
	{//we want to turn off our saber if we need to.
		if ( !NPC->client->ps.saberHolstered )
		{//saber is on.
			WP_DeactivateSaber( NPC, qfalse );
			NPC_SetAnim( NPC, SETANIM_BOTH, BOTH_TURNOFF, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
		}
	}

	NPC_UpdateAngles( qtrue, qtrue );
}


/*  I beleive this is used by the saber code for calculating the saber stuff with 
	//saber droids.  Impliment?
int SaberDroid_PowerLevelForSaberAnim( gentity_t *self )
{//determine the strength of the saber attack based on the saber animation.
	switch ( self->client->ps.legsAnim )
	{
	case BOTH_A2_TR_BL:
		if ( self->client->ps.torsoTimer <= 200 )
		{//end of anim
			return FORCE_LEVEL_0;
		}
		else if ( PM_AnimLength( self->localAnimIndex, (animNumber_t)self->client->ps.legsAnim ) - self->client->ps.torsoTimer < 200 )
		{//beginning of anim
			return FORCE_LEVEL_0;
		}
		return FORCE_LEVEL_2;
		break;
	case BOTH_A1_BL_TR:
		if ( self->client->ps.torsoTimer <= 300 )
		{//end of anim
			return FORCE_LEVEL_0;
		}
		else if ( PM_AnimLength( self->localAnimIndex, (animNumber_t)self->client->ps.legsAnim ) - self->client->ps.torsoTimer < 200 )
		{//beginning of anim
			return FORCE_LEVEL_0;
		}
		return FORCE_LEVEL_1;
		break;
	case BOTH_A1__L__R:
		if ( self->client->ps.torsoTimer <= 250 )
		{//end of anim
			return FORCE_LEVEL_0;
		}
		else if ( PM_AnimLength( self->localAnimIndex, (animNumber_t)self->client->ps.legsAnim ) - self->client->ps.torsoTimer < 150 )
		{//beginning of anim
			return FORCE_LEVEL_0;
		}
		return FORCE_LEVEL_1;
		break;
	case BOTH_A3__L__R:
		if ( self->client->ps.torsoTimer <= 200 )
		{//end of anim
			return FORCE_LEVEL_0;
		}
		else if ( PM_AnimLength( self->localAnimIndex, (animNumber_t)self->client->ps.legsAnim ) - self->client->ps.torsoTimer < 300 )
		{//beginning of anim
			return FORCE_LEVEL_0;
		}
		return FORCE_LEVEL_3;
		break;
	}
	return FORCE_LEVEL_0;
}
*/

/*
-------------------------
NPC_BSSaberDroid_Attack
-------------------------
*/

void NPC_SaberDroid_PickAttack( void )
{
	int attackAnim = Q_irand( 0, 3 );
	switch ( attackAnim )
	{
	case 0:
	default:
		attackAnim = BOTH_A2_TR_BL;
		NPC->client->ps.saberMove = LS_A_TR2BL;
		NPC->client->ps.fd.saberAnimLevel = SS_MEDIUM;
		break;
	case 1:
		attackAnim = BOTH_A1_BL_TR;
		NPC->client->ps.saberMove = LS_A_BL2TR;
		NPC->client->ps.fd.saberAnimLevel = SS_FAST;
		break;
	case 2:
		attackAnim = BOTH_A1__L__R;
		NPC->client->ps.saberMove = LS_A_L2R;
		NPC->client->ps.fd.saberAnimLevel = SS_FAST;
		break;
	case 3:
		attackAnim = BOTH_A3__L__R;
		NPC->client->ps.saberMove = LS_A_L2R;
		NPC->client->ps.fd.saberAnimLevel = SS_STRONG;
		break;
	}
	NPC->client->ps.saberBlocking = saberMoveData[NPC->client->ps.saberMove].blocking;
	/* RAFIXME - since this is saber trail code, I think this needs to be ported to 
	cgame.
	if ( saberMoveData[NPC->client->ps.saberMove].trailLength > 0 )
	{
		NPC->client->ps.SaberActivateTrail( saberMoveData[NPC->client->ps.saberMove].trailLength ); // saber trail lasts for 75ms...feel free to change this if you want it longer or shorter
	}
	else
	{
		NPC->client->ps.SaberDeactivateTrail( 0 );
	}
	*/

	NPC_SetAnim( NPC, SETANIM_BOTH, attackAnim, SETANIM_FLAG_HOLD|SETANIM_FLAG_OVERRIDE );
	NPC->client->ps.torsoAnim = NPC->client->ps.legsAnim;//need to do this because we have no anim split but saber code checks torsoAnim
	NPC->client->ps.weaponTime = NPC->client->ps.torsoTimer = NPC->client->ps.legsTimer;
	NPC->client->ps.weaponstate = WEAPON_FIRING;
}

void NPC_BSSaberDroid_Attack( void )
{//attack behavior
	//Don't do anything if we're hurt
	if ( NPC->painDebounceTime > level.time )
	{
		NPC_UpdateAngles( qtrue, qtrue );
		return;
	}

	//NPC_CheckEnemy( qtrue, qfalse );
	//If we don't have an enemy, just idle
	if ( NPC_CheckEnemyExt(qfalse) == qfalse )//!NPC->enemy )//
	{
		NPC->enemy = NULL;
		NPC_BSSaberDroid_Patrol();//FIXME: or patrol?
		return;
	}

	if ( !NPC->enemy )
	{//WTF?  somehow we lost our enemy?
		NPC_BSSaberDroid_Patrol();//FIXME: or patrol?
		return;
	}

	enemyLOS = enemyCS = qfalse;
	move = qtrue;
	faceEnemy = qfalse;
	shoot = qfalse;
	enemyDist = DistanceSquared( NPC->enemy->r.currentOrigin, NPC->r.currentOrigin );

	//can we see our target?
	if ( NPC_ClearLOS4( NPC->enemy ) )
	{
		NPCInfo->enemyLastSeenTime = level.time;
		enemyLOS = qtrue;

		if ( enemyDist <= 4096 && InFOV3( NPC->enemy->r.currentOrigin, NPC->r.currentOrigin, NPC->client->ps.viewangles, 90, 45 ) )//within 64 & infront
		{
			VectorCopy( NPC->enemy->r.currentOrigin, NPCInfo->enemyLastSeenLocation );
			enemyCS = qtrue;
		}
	}
	/*
	else if ( gi.inPVS( NPC->enemy->currentOrigin, NPC->currentOrigin ) )
	{
		NPCInfo->enemyLastSeenTime = level.time;
		faceEnemy = qtrue;
	}
	*/

	if ( enemyLOS )
	{//FIXME: no need to face enemy if we're moving to some other goal and he's too far away to shoot?
		faceEnemy = qtrue;
	}

	if ( !TIMER_Done( NPC, "taunting" ) )
	{
		move = qfalse;
	}
	else if ( enemyCS )
	{
		shoot = qtrue;
		if ( enemyDist < (NPC->r.maxs[0]+NPC->enemy->r.maxs[0]+32)*(NPC->r.maxs[0]+NPC->enemy->r.maxs[0]+32) )
		{//close enough
			move = qfalse;
		}
	}//this should make him chase enemy when out of range...?

	if ( NPC->client->ps.legsTimer 
		&& NPC->client->ps.legsAnim != BOTH_A3__L__R )//this one is a running attack
	{//in the middle of a held, stationary anim, can't move
		move = qfalse;
	}

	if ( move )
	{//move toward goal
		move = SaberDroid_Move();
		if ( move )
		{//if we had to chase him, be sure to attack as soon as possible
			TIMER_Set( NPC, "attackDelay", NPC->client->ps.weaponTime );
		}
	}

	if ( !faceEnemy )
	{//we want to face in the dir we're running
		if ( move )
		{//don't run away and shoot
			NPCInfo->desiredYaw = NPCInfo->lastPathAngles[YAW];
			NPCInfo->desiredPitch = 0;
			shoot = qfalse;
		}
		NPC_UpdateAngles( qtrue, qtrue );
	}
	else// if ( faceEnemy )
	{//face the enemy
		NPC_FaceEnemy(qtrue);
	}

	if ( NPCInfo->scriptFlags&SCF_DONT_FIRE )
	{
		shoot = qfalse;
	}
	
	//FIXME: need predicted blocking?
	//FIXME: don't shoot right away!
	if ( shoot )
	{//try to shoot if it's time
		if ( TIMER_Done( NPC, "attackDelay" ) )
		{	
			if( !(NPCInfo->scriptFlags & SCF_FIRE_WEAPON) ) // we've already fired, no need to do it again here
			{//attack!
				NPC_SaberDroid_PickAttack();
				//set attac delay for next attack.
				if ( NPCInfo->rank > RANK_CREWMAN )
				{
					TIMER_Set( NPC, "attackDelay", NPC->client->ps.weaponTime+Q_irand(0, 1000) );
				}
				else
				{
					TIMER_Set( NPC, "attackDelay", NPC->client->ps.weaponTime+Q_irand( 0, 1000 )+(Q_irand( 0, (3-g_spskill.integer)*2 )*500) );
				}
			}
		}
	}
}


extern void WP_ActivateSaber( gentity_t *self );
void NPC_BSSD_Default( void )
{
	if( !NPC->enemy )
	{//don't have an enemy, look for one
		NPC_BSSaberDroid_Patrol();
	}
	else//if ( NPC->enemy )
	{//have an enemy
		if ( NPC->client->ps.saberHolstered == 2 )
		{//turn saber on
			WP_ActivateSaber( NPC );
			//NPC->client->ps.SaberActivate();
			if ( NPC->client->ps.legsAnim == BOTH_TURNOFF
				|| NPC->client->ps.legsAnim == BOTH_STAND1 )
			{
				NPC_SetAnim( NPC, SETANIM_BOTH, BOTH_TURNON, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
			}
		}

		NPC_BSSaberDroid_Attack();
		TIMER_Set( NPC, "inactiveDelay", Q_irand( 2000, 4000 ) );
	}
	if ( !NPC->client->ps.weaponTime )
	{//we're not attacking.
		NPC->client->ps.saberMove = LS_READY;
		NPC->client->ps.saberBlocking = saberMoveData[LS_READY].blocking;
		//RAFIXME - since this is saber trail code, I think this needs to be ported to cgame.
		//NPC->client->ps.SaberDeactivateTrail( 0 );
		NPC->client->ps.fd.saberAnimLevel = SS_MEDIUM;
		NPC->client->ps.weaponstate = WEAPON_READY;
	}
}
//[/CoOp]
//[/SPPortComplete]
