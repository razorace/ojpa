#include "w_force.h"

//[FatigueSys]
extern void G_AddMercBalance(gentity_t *self, int amount);//add balance to flamethrower
extern qboolean BG_SaberAttacking( playerState_t *ps );
extern qboolean BG_SaberInTransitionAny( int move );
extern qboolean GAME_INLINE WalkCheck( gentity_t * self );
//[/FatigueSys]

int rageLoopSound = 0;
int protectLoopSound = 0;
int absorbLoopSound = 0;
int seeLoopSound = 0;
int	ysalamiriLoopSound = 0;

extern vmCvar_t		g_saberRestrictForce;

extern qboolean BG_FullBodyTauntAnim( int anim );

extern bot_state_t *botstates[MAX_CLIENTS];

int ForceShootDrain( gentity_t *self );

qboolean WP_ForcePowerInUse( gentity_t *self, forcePowers_t forcePower ) {
	return (qboolean)(self->client->ps.fd.forcePowersActive & ( 1 << forcePower ));
}

gentity_t *G_PreDefSound(vec3_t org, int pdSound)
{
	gentity_t	*te;

	te = G_TempEntity( org, EV_PREDEFSOUND );
	te->s.eventParm = pdSound;
	VectorCopy(org, te->s.origin);

	return te;
}

const int forcePowerMinRank[NUM_FORCE_POWER_LEVELS][NUM_FORCE_POWERS] = //0 == neutral
{
	{
		999,//FP_HEAL,//instant
		999,//FP_LEVITATION,//hold/duration
		999,//FP_SPEED,//duration
		999,//FP_PUSH,//hold/duration
		999,//FP_PULL,//hold/duration
		999,//FP_MINDTRICK,//instant
		999,//FP_GRIP,//hold/duration
		999,//FP_LIGHTNING,//hold/duration
		999,//FP_RAGE,//duration
		999,//FP_MANIPULATE,//duration
		999,//FP_ABSORB,//duration
		999,//FP_TEAM_HEAL,//instant
		999,//FP_LIFT,//instant
		999,//FP_DRAIN,//hold/duration
		999,//FP_SEE,//duration
		999,//FP_SABER_OFFENSE,
		999,//FP_SABER_DEFENSE,
		999//FP_SABERTHROW,
		//NUM_FORCE_POWERS
	},
	{
		10,//FP_HEAL,//instant
		0,//FP_LEVITATION,//hold/duration
		0,//FP_SPEED,//duration
		0,//FP_PUSH,//hold/duration
		0,//FP_PULL,//hold/duration
		10,//FP_MINDTRICK,//instant
		15,//FP_GRIP,//hold/duration
		10,//FP_LIGHTNING,//hold/duration
		15,//FP_RAGE,//duration
		15,//FP_MANIPULATE,//duration
		15,//FP_ABSORB,//duration
		10,//FP_TEAM_HEAL,//instant
		10,//FP_LIFT,//instant
		10,//FP_DRAIN,//hold/duration
		5,//FP_SEE,//duration
		0,//FP_SABER_OFFENSE,
		0,//FP_SABER_DEFENSE,
		0//FP_SABERTHROW,
		//NUM_FORCE_POWERS
	},
	{
		10,//FP_HEAL,//instant
		0,//FP_LEVITATION,//hold/duration
		0,//FP_SPEED,//duration
		0,//FP_PUSH,//hold/duration
		0,//FP_PULL,//hold/duration
		10,//FP_MINDTRICK,//instant
		15,//FP_GRIP,//hold/duration
		10,//FP_LIGHTNING,//hold/duration
		15,//FP_RAGE,//duration
		15,//FP_MANIPULATE,//duration
		15,//FP_ABSORB,//duration
		10,//FP_TEAM_HEAL,//instant
		10,//FP_LIFT,//instant
		10,//FP_DRAIN,//hold/duration
		5,//FP_SEE,//duration
		5,//FP_SABER_OFFENSE,
		5,//FP_SABER_DEFENSE,
		5//FP_SABERTHROW,
		//NUM_FORCE_POWERS
	},
	{
		10,//FP_HEAL,//instant
		0,//FP_LEVITATION,//hold/duration
		0,//FP_SPEED,//duration
		0,//FP_PUSH,//hold/duration
		0,//FP_PULL,//hold/duration
		10,//FP_MINDTRICK,//instant
		15,//FP_GRIP,//hold/duration
		10,//FP_LIGHTNING,//hold/duration
		15,//FP_RAGE,//duration
		15,//FP_MANIPULATE,//duration
		15,//FP_ABSORB,//duration
		10,//FP_TEAM_HEAL,//instant
		10,//FP_LIFT,//instant
		10,//FP_DRAIN,//hold/duration
		5,//FP_SEE,//duration
		10,//FP_SABER_OFFENSE,
		10,//FP_SABER_DEFENSE,
		10//FP_SABERTHROW,
		//NUM_FORCE_POWERS
	}
};

void FP_InitResources() {
	if (!rageLoopSound) {
		rageLoopSound = G_SoundIndex("sound/weapons/force/rageloop.wav");
	}

	if (!absorbLoopSound) {
		absorbLoopSound = G_SoundIndex("sound/weapons/force/absorbloop.wav");
	}

	if (!protectLoopSound) {
		protectLoopSound = G_SoundIndex("sound/weapons/force/protectloop.wav");
	}

	if (!seeLoopSound) {
		seeLoopSound = G_SoundIndex("sound/weapons/force/seeloop.wav");
	}

	if (!ysalamiriLoopSound) {
		ysalamiriLoopSound = G_SoundIndex("sound/player/nullifyloop.wav");
	}
}

//[CoOp]
extern int SpawnForcePowerLevels[NUM_FORCE_POWERS];
extern qboolean UseSpawnForcePowers;
//[/CoOp]
void WP_InitForcePowers( gentity_t *ent ) {
	int i;
	int i_r;
	//[ExpSys]
	int maxRank = ent->client->sess.skillPoints;
	//[/ExpSys]
	qboolean warnClient = qfalse;
	char userinfo[MAX_INFO_STRING];
	char forcePowers[256];
	char readBuf[256];
	int lastFPKnown = -1;
	qboolean didEvent = qfalse;

	if (!ent || 
		!ent->client || 
		ent->s.eType == ET_NPC) {
		return;
	}
	
	FP_InitResources();

	ent->client->ps.fd.saberAnimLevel = ent->client->sess.saberLevel;

	i = 0;
	while (i < NUM_FORCE_POWERS)
	{
		ent->client->ps.fd.forcePowerLevel[i] = 0;
		ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
		i++;
	}

	//[ExpSys]
	//racc - reset additional skills
	for(i=0; i < NUM_SKILLS; i++) {
		ent->client->skillLevel[i] = 0;
	}
	//[/ExpSys]

	ent->client->ps.fd.forcePowerSelected = -1;

	ent->client->ps.fd.forceSide = 0;

	if (g_gametype.integer == GT_SIEGE &&
		ent->client->siegeClass != -1)
	{ //Then use the powers for this class, and skip all this nonsense.
		i = 0;

		while (i < NUM_FORCE_POWERS)
		{
			ent->client->ps.fd.forcePowerLevel[i] = bgSiegeClasses[ent->client->siegeClass].forcePowerLevels[i];

			if (!ent->client->ps.fd.forcePowerLevel[i]) {
				ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
			}
			else {
				ent->client->ps.fd.forcePowersKnown |= (1 << i);
			}

			i++;
		}

		if (!ent->client->sess.setForce) {
			//bring up the class selection menu
			trap_SendServerCommand(ent-g_entities, "scl");
		}

		ent->client->sess.setForce = qtrue;

		return;
	}

	trap_GetUserinfo( ent->s.number, userinfo, sizeof( userinfo ) );

	Q_strncpyz( forcePowers, Info_ValueForKey (userinfo, "forcepowers"), sizeof( forcePowers ) );

	if ( (ent->r.svFlags & SVF_BOT) && botstates[ent->s.number] )
	{ //if it's a bot just copy the info directly from its personality
		Com_sprintf(forcePowers, sizeof(forcePowers), "%s", botstates[ent->s.number]->forceinfo);
	}

	//rww - parse through the string manually and eat out all the appropriate data
	i = 0;

	if (g_forceBasedTeams.integer)
	{
		if (ent->client->sess.sessionTeam == TEAM_RED)
		{
			warnClient = (qboolean)(!(BG_LegalizedForcePowers(forcePowers, maxRank, HasSetSaberOnly(), FORCE_DARKSIDE, g_gametype.integer, g_forcePowerDisable.integer)));
		}
		else if (ent->client->sess.sessionTeam == TEAM_BLUE)
		{
			warnClient = (qboolean)(!(BG_LegalizedForcePowers(forcePowers, maxRank, HasSetSaberOnly(), FORCE_LIGHTSIDE, g_gametype.integer, g_forcePowerDisable.integer)));
		}
		else
		{
			warnClient = (qboolean)(!(BG_LegalizedForcePowers(forcePowers, maxRank, HasSetSaberOnly(), 0, g_gametype.integer, g_forcePowerDisable.integer)));
		}
	}
	else
	{
		warnClient = (qboolean)(!(BG_LegalizedForcePowers(forcePowers, maxRank, HasSetSaberOnly(), 0, g_gametype.integer, g_forcePowerDisable.integer)));
	}

	i_r = 0;
	while (forcePowers[i] && forcePowers[i] != '-')
	{
		readBuf[i_r] = forcePowers[i];
		i_r++;
		i++;
	}
	readBuf[i_r] = 0;
	//THE RANK
	ent->client->ps.fd.forceRank = atoi(readBuf);
	i++;

	i_r = 0;
	while (forcePowers[i] && forcePowers[i] != '-')
	{
		readBuf[i_r] = forcePowers[i];
		i_r++;
		i++;
	}
	readBuf[i_r] = 0;
	//THE SIDE
	ent->client->ps.fd.forceSide = atoi(readBuf);
	i++;

	i_r = 0;
	while (forcePowers[i] && forcePowers[i] != '\n' &&
		i_r < NUM_FORCE_POWERS)
	{
		readBuf[0] = forcePowers[i];
		readBuf[1] = 0;

		ent->client->ps.fd.forcePowerLevel[i_r] = atoi(readBuf);
		if (ent->client->ps.fd.forcePowerLevel[i_r])
		{
			ent->client->ps.fd.forcePowersKnown |= (1 << i_r);
		}
		else
		{
			ent->client->ps.fd.forcePowersKnown &= ~(1 << i_r);
		}
		i++;
		i_r++;
	}
	//THE POWERS

	//[ExpSys]
	//apply our additional force powers
	for(i_r=0; forcePowers[i] && forcePowers[i] != '\n' && i_r < NUM_SKILLS; i_r++, i++)
	{
		readBuf[0] = forcePowers[i];
		readBuf[1] = 0;

		ent->client->skillLevel[i_r] = atoi(readBuf);
	}
	//[/ExpSys]

	if (HasSetSaberOnly())
	{
		gentity_t *te = G_TempEntity( vec3_origin, EV_SET_FREE_SABER );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = 1;
	}
	else
	{
		gentity_t *te = G_TempEntity( vec3_origin, EV_SET_FREE_SABER );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = 0;
	}

	if (g_forcePowerDisable.integer)
	{
		gentity_t *te = G_TempEntity( vec3_origin, EV_SET_FORCE_DISABLE );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = 1;
	}
	else
	{
		gentity_t *te = G_TempEntity( vec3_origin, EV_SET_FORCE_DISABLE );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = 0;
	}

	//racc - Forces the player to look at their Profiles menu in some situations 
	//(like when the game starts or when the player's force power config string is bad)
	//This also lets the client know the server's maxForceRank is.
	//[BotTweaks]
	if (ent->r.svFlags & SVF_BOT)
	//racc - bots care not about such things 
	//[/BotTweaks]
	{
		ent->client->sess.setForce = qtrue;
	}
	else if (g_gametype.integer == GT_SIEGE)
	{
		if (!ent->client->sess.setForce)
		{
			ent->client->sess.setForce = qtrue;
			//bring up the class selection menu
			trap_SendServerCommand(ent-g_entities, "scl");
		}
	}
	else
	{
		//[ExpSys]
		//since the possibility that a player's force configuration is more likely to be bad
		//with the new experience system, just go ahead and don't force warned clients into the
		//force powers screen.
		if (warnClient || !ent->client->sess.setForce)
		//[/ExpSys]
		{ //the client's rank is too high for the server and has been autocapped, so tell them
			if (g_gametype.integer != GT_HOLOCRON && g_gametype.integer != GT_JEDIMASTER )
			{
				didEvent = qtrue;

				//[BotTweaks]
				//racc - bots skip over this stuff now so this check isn't needed anymore.
				//if (!(ent->r.svFlags & SVF_BOT) && ent->s.eType != ET_NPC)
				//[/BotTweaks]
				{
					if (!g_teamAutoJoin.integer) {
						//Make them a spectator so they can set their powerups up without being bothered.
						ent->client->sess.sessionTeam = TEAM_SPECTATOR;
						ent->client->sess.spectatorState = SPECTATOR_FREE;
						ent->client->sess.spectatorClient = 0;

						ent->client->pers.teamState.state = TEAM_BEGIN;
						trap_SendServerCommand(ent-g_entities, "spc");	// Fire up the profile menu
					}
				}

				//Event isn't very reliable, I made it a string. This way I can send it to just one
				//client also, as opposed to making a broadcast event.
				trap_SendServerCommand(ent->s.number, va("nfr %i %i %i", maxRank, 1, ent->client->sess.sessionTeam));
				//Arg1 is new max rank, arg2 is non-0 if force menu should be shown, arg3 is the current team
			}
			ent->client->sess.setForce = qtrue;
		}

		if (!didEvent ) {
			trap_SendServerCommand(ent->s.number, va("nfr %i %i %i", maxRank, 0, ent->client->sess.sessionTeam));
		}
	}

	i = 0;
	while (i < NUM_FORCE_POWERS)
	{
		if ((ent->client->ps.fd.forcePowersKnown & (1 << i)) &&
			!ent->client->ps.fd.forcePowerLevel[i])
		{ //err..
			ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
		}
		else
		{
			if (i != FP_LEVITATION && i != FP_SABER_OFFENSE && i != FP_SABER_DEFENSE && i != FP_SABERTHROW)
			{
				lastFPKnown = i;
			}
		}

		i++;
	}

	if (ent->client->ps.fd.forcePowersKnown & ent->client->sess.selectedFP) {
		ent->client->ps.fd.forcePowerSelected = ent->client->sess.selectedFP;
	}

	if (!(ent->client->ps.fd.forcePowersKnown & (1 << ent->client->ps.fd.forcePowerSelected))) {
		if (lastFPKnown != -1) {
			ent->client->ps.fd.forcePowerSelected = lastFPKnown;
		}
		else {
			ent->client->ps.fd.forcePowerSelected = 0;
		}
	}

	while (i < NUM_FORCE_POWERS)
	{
		ent->client->ps.fd.forcePowerBaseLevel[i] = ent->client->ps.fd.forcePowerLevel[i];
		i++;
	}

	ent->client->ps.fd.forceUsingAdded = 0;
}

void WP_SpawnInitForcePowers( gentity_t *ent ) {
	//[SaberSys]
	//new balancing system uses 7 as the default.
	//ent->client->ps.saberAttackChainCount = 7;
	ent->client->ps.saberAttackChainCount = BALANCE_MAX;
	//[/SaberSys]
	for(int i = 0; i < NUM_FORCE_POWERS; i++) {
		if (WP_ForcePowerInUse(ent, i)) {
			WP_ForcePowerStop(ent, i);
		}
	}

	ent->client->ps.fd.forceDeactivateAll = 0;

	ent->client->ps.fd.forcePower = ent->client->ps.fd.forcePowerMax = FORCE_POWER_MAX;
	ent->client->ps.fd.forcePowerRegenDebounceTime = 0;
	ent->client->ps.fd.forceGripEntityNum = ENTITYNUM_NONE;
	ent->client->ps.fd.forceMindtrickTargetIndex = 0;
	ent->client->ps.fd.forceMindtrickTargetIndex2 = 0;
	ent->client->ps.fd.forceMindtrickTargetIndex3 = 0;
	ent->client->ps.fd.forceMindtrickTargetIndex4 = 0;

	ent->client->ps.holocronBits = 0;
	for(int i = 0; i < NUM_FORCE_POWERS; i++) {
		ent->client->ps.holocronsCarried[i] = 0;
	}

	if (g_gametype.integer == GT_HOLOCRON) {
		for(int i = 0; i < NUM_FORCE_POWERS; i++) {
			ent->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_0;
		}

		if (HasSetSaberOnly()) {
			if (ent->client->ps.fd.forcePowerLevel[FP_SABER_OFFENSE] < FORCE_LEVEL_1) {
				ent->client->ps.fd.forcePowerLevel[FP_SABER_OFFENSE] = FORCE_LEVEL_1;
			}

			if (ent->client->ps.fd.forcePowerLevel[FP_SABER_DEFENSE] < FORCE_LEVEL_1) {
				ent->client->ps.fd.forcePowerLevel[FP_SABER_DEFENSE] = FORCE_LEVEL_1;
			}
		}
	}

	for(int i = 0; i < NUM_FORCE_POWERS; i++) {
		ent->client->ps.fd.forcePowerDebounce[i] = 0;
		ent->client->ps.fd.forcePowerDuration[i] = 0;
	}

	ent->client->ps.fd.forcePowerRegenDebounceTime = 0;
	ent->client->ps.fd.forceJumpZStart = 0;
	ent->client->ps.fd.forceJumpCharge = 0;
	ent->client->ps.fd.forceJumpSound = 0;
	ent->client->ps.fd.forceGripDamageDebounceTime = 0;
	ent->client->ps.fd.forceGripBeingGripped = 0;
	ent->client->ps.fd.forceGripCripple = 0;
	ent->client->ps.fd.forceGripUseTime = 0;
	ent->client->ps.fd.forceGripSoundTime = 0;
	ent->client->ps.fd.forceGripStarted = 0;
	ent->client->ps.fd.forceHealTime = 0;
	ent->client->ps.fd.forceHealAmount = 0;
	ent->client->ps.fd.forceRageRecoveryTime = 0;
	ent->client->ps.fd.forceDrainEntNum = ENTITYNUM_NONE;
	ent->client->ps.fd.forceDrainTime = 0;

	for(int i = 0; i < NUM_FORCE_POWERS; i++) {
		if ((ent->client->ps.fd.forcePowersKnown & (1 << i)) &&
			!ent->client->ps.fd.forcePowerLevel[i])
		{ //make sure all known powers are cleared if we have level 0 in them
			ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
		}
	}

	if (g_gametype.integer == GT_SIEGE &&
		ent->client->siegeClass != -1)
	{ //Then use the powers for this class.
		for(int i = 0; i < NUM_FORCE_POWERS; i++) {
			ent->client->ps.fd.forcePowerLevel[i] = bgSiegeClasses[ent->client->siegeClass].forcePowerLevels[i];

			if (!ent->client->ps.fd.forcePowerLevel[i]) {
				ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
			}
			else {
				ent->client->ps.fd.forcePowersKnown |= (1 << i);
			}
		}
	}
}

qboolean IsGunSkill(int skill) {
	return (qboolean)(
		skill == SK_PISTOL || 
		skill == SK_BLASTER || 
		skill == SK_THERMAL || 
		skill == SK_ROCKET || 
		skill == SK_BOWCASTER || 
		skill == SK_DETPACK ||
		skill == SK_REPEATER || 
		skill == SK_DISRUPTOR || 
		skill == SK_FLECHETTE || 
		skill == SK_TUSKEN_RIFLE || 
		skill == SK_GRENADE);
}

qboolean IsMerc(gclient_t *client) {
	for(int i = 0;i < NUM_SKILLS; i++) {
		if(IsGunSkill(i) && client->skillLevel[i])
			return qtrue;
	}

	return qfalse;
}

//[ForceSys]
extern qboolean PM_RunningAnim(int anim);
extern qboolean PM_SaberInBrokenParry( int move );
extern qboolean BG_InSlowBounce(playerState_t *ps);
extern qboolean BG_InKnockDownOnGround( playerState_t *ps );
extern qboolean BG_InKnockDown( int anim ); //bg_pmove.c
//[/ForceSys]
int ForcePowerUsableOn(gentity_t *attacker, gentity_t *other, forcePowers_t forcePower) {
	if(other && other->client) {
		if(other->client->ps.inAirAnim || other->client->ps.groundEntityNum == ENTITYNUM_NONE) {
			return 1;
		}

		if (BG_HasYsalamiri(g_gametype.integer, &other->client->ps)) {
			return 0;
		}

		if(IsMerc(other->client) && other->client->ps.fd.forcePowerLevel[FP_SEE] >= FORCE_LEVEL_1) {
			return 1;
		}

		if (other->client->ps.duelInProgress) {
			return 0;
		}

		if(other->client->ps.fd.saberAnimLevel == SS_DESANN
			&& other->client->ps.saberAttackChainCount <= BALANCE_LOW) {
			return 0;
		}

		if(forcePower == FP_MINDTRICK) {
			switch(other->client->ps.fd.forcePowerLevel[FP_ABSORB])
			{
			case FORCE_LEVEL_3:
				return 0;
				break;
			case FORCE_LEVEL_2:
				if(!WalkCheck(other) && PM_RunningAnim(other->client->ps.legsAnim) )
					return 0;
				break;

			case FORCE_LEVEL_1:
				if(!WalkCheck(other))
					return 0;
				break;
			}
		}
		else if(forcePower == FP_GRIP) {
			switch(other->client->ps.fd.forcePowerLevel[FP_ABSORB])
			{
			case FORCE_LEVEL_1://Can only block if walking
				if(!WalkCheck(other))
					return 1;
				break;
			case FORCE_LEVEL_2://Can block if walking or running
				if(other->client->ps.inAirAnim || other->client->ps.groundEntityNum == ENTITYNUM_NONE) {
					return 1;
				}
				break;

			case FORCE_LEVEL_3:
				return 0;

			default:
				return 1;
			}
		}

		if (forcePower == FP_PUSH || forcePower == FP_PULL){
			if (BG_InKnockDown(other->client->ps.legsAnim))	{
				return 0;
			}
		}

		if (other->s.eType == ET_NPC &&
			other->s.NPC_class == CLASS_VEHICLE) { 
			//can't use the force on vehicles.. except lightning
			return forcePower == FP_LIGHTNING ? 1 : 0;
		}

		if (other->s.eType == ET_NPC &&
			g_gametype.integer == GT_SIEGE) {
			return 0;
		}
	}
	
	if(attacker && attacker->client) {
		if (!BG_CanUseFPNow(g_gametype.integer, &attacker->client->ps, level.time, forcePower)) {
			return 0;
		}

		//Dueling fighters cannot use force powers on others, with the exception of force push when locked with each other
		if (attacker->client->ps.duelInProgress) {
			return 0;
		}
	}

	return 1;
}

qboolean WP_ForcePowerAvailable( gentity_t *self, forcePowers_t forcePower, int overrideAmt )
{
	int	drain = overrideAmt ? overrideAmt :
				forcePowerNeeded[self->client->ps.fd.forcePowerLevel[forcePower]][forcePower];

	if (WP_ForcePowerInUse(self, forcePower)) {
		return qtrue;
	}

	if ( forcePower == FP_LEVITATION ) {
		return qtrue;
	}

	if ( !drain ) {
		return qtrue;
	}

	if ( self->client->ps.fd.forcePower < drain ) {
		return qfalse;
	}

	return qtrue;
}

qboolean WP_ForcePowerUsable( gentity_t *self, forcePowers_t forcePower )
{
	if (BG_HasYsalamiri(g_gametype.integer, &self->client->ps))
	{
		return qfalse;
	}

	if (self->health <= 0 || self->client->ps.stats[STAT_HEALTH] <= 0 ||
		(self->client->ps.eFlags & EF_DEAD))
	{
		return qfalse;
	}

	if (self->client->ps.pm_flags & PMF_FOLLOW)
	{ //specs can't use powers through people
		return qfalse;
	}
	if (self->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		return qfalse;
	}
	if (self->client->tempSpectate >= level.time)
	{
		return qfalse;
	}

	if (!BG_CanUseFPNow(g_gametype.integer, &self->client->ps, level.time, forcePower))
	{
		return qfalse;
	}

	if ( !(self->client->ps.fd.forcePowersKnown & ( 1 << forcePower )) )
	{//don't know this power
		return qfalse;
	}
	
	if (WP_ForcePowerInUse(self, forcePower))
	{//already using this power
		if (forcePower != FP_LEVITATION && forcePower != FP_SPEED) {
			return qfalse;
		}
	}

	if (forcePower == FP_LEVITATION && self->client->fjDidJump) {
		return qfalse;
	}

	if (!self->client->ps.fd.forcePowerLevel[forcePower]) {
		return qfalse;
	}

	if ( g_debugMelee.integer ) {
		if ( (self->client->ps.pm_flags&PMF_STUCK_TO_WALL) )
		{//no offensive force powers when stuck to wall
			switch ( forcePower )
			{
			case FP_GRIP:
			case FP_LIGHTNING:
			case FP_DRAIN:
			case FP_SABER_OFFENSE:
			case FP_SABER_DEFENSE:
			case FP_SABERTHROW:
				return qfalse;
				break;
			}
		}
	}

	if ( !self->client->ps.saberHolstered )
	{
		if ( (self->client->saber[0].saberFlags&SFL_TWO_HANDED) )
		{
			if ( g_saberRestrictForce.integer )
			{
				switch ( forcePower )
				{
				case FP_PUSH:
				case FP_PULL:
				case FP_MINDTRICK:
				case FP_GRIP:
				case FP_LIGHTNING:
				case FP_DRAIN:
					return qfalse;
					break;
				}
			}
		}

		if ( (self->client->saber[0].saberFlags&SFL_TWO_HANDED)
			|| (self->client->saber[0].model && self->client->saber[0].model[0]) )
		{//this saber requires the use of two hands OR our other hand is using an active saber too
			if ( (self->client->saber[0].forceRestrictions&(1<<forcePower)) )
			{//this power is verboten when using this saber
				return qfalse;
			}
		}

		if ( self->client->saber[0].model 
			&& self->client->saber[0].model[0] )
		{//both sabers on
			if ( g_saberRestrictForce.integer )
			{
				switch ( forcePower )
				{
				case FP_PUSH:
				case FP_PULL:
				case FP_MINDTRICK:
				case FP_GRIP:
				case FP_LIGHTNING:
				case FP_DRAIN:
					return qfalse;
					break;
				}
			}
			if ( (self->client->saber[1].forceRestrictions&(1<<forcePower)) )
			{//this power is verboten when using this saber
				return qfalse;
			}
		}
	}
	return WP_ForcePowerAvailable( self, forcePower, 0 );	// OVERRIDEFIXME
}

void WP_ForcePowerRegenerate( gclient_t *client, int overrideAmt )
{ //called on a regular interval to regenerate force power.
	if ( overrideAmt ) {
		client->ps.fd.forcePower += overrideAmt;
	}
	else {
		client->ps.fd.forcePower++;
	}

	if ( client->ps.fd.forcePower > client->ps.fd.forcePowerMax ) {
		client->ps.fd.forcePower = client->ps.fd.forcePowerMax;
	}
}

void WP_ForcePowerStart( gentity_t *self, forcePowers_t forcePower, int overrideAmt )
{ //activate the given force power
	int	duration = 0;
	qboolean hearable = qfalse;
	float hearDist = 0;

	if (!WP_ForcePowerAvailable( self, forcePower, overrideAmt )) {
		return;
	}

	if (BG_FullBodyTauntAnim( self->client->ps.legsAnim)) {
		self->client->ps.legsTimer = 0;
	}

	if (BG_FullBodyTauntAnim( self->client->ps.torsoAnim)) {
		self->client->ps.torsoTimer = 0;
	}
	//hearable and hearDist are merely for the benefit of bots, and not related to if a sound is actually played.
	//If duration is set, the force power will assume to be timer-based.
	switch( (int)forcePower )
	{
	case FP_HEAL:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_LEVITATION:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_SPEED:
		hearable = qtrue;
		hearDist = 256;
		duration = 500;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_PUSH:
		hearable = qtrue;
		hearDist = 256;
		break;
	case FP_PULL:
		hearable = qtrue;
		hearDist = 256;
		break;
	case FP_MINDTRICK:
		hearable = qtrue;
		hearDist = 256;

		if (self->client->ps.fd.forcePowerLevel[FP_MINDTRICK] == FORCE_LEVEL_1) {
			duration = 20000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_MINDTRICK] == FORCE_LEVEL_2) {
			duration = 5000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_MINDTRICK] == FORCE_LEVEL_3) {
			duration = 30000;
		}
		else {
			break;
		}

		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_GRIP:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		self->client->ps.powerups[PW_DISINT_4] = level.time + 60000;
		break;
	case FP_LIGHTNING:
		if(self->client->forcePowerStart[FP_LIGHTNING] == -1) {
			hearable = qfalse;
			hearDist = 512;

			if(self->client->ps.fd.forcePowerLevel[FP_LIGHTNING] == FORCE_LEVEL_1) {
				duration = 2200;
			}
			else {
				duration = overrideAmt;
			}
			
			overrideAmt = 0;
			self->client->forcePowerStart[FP_LIGHTNING] = level.time+1200;
		}
		else
			return;
		break;
	case FP_RAGE:
		hearable = qtrue;
		hearDist = 256;
		duration=10000;

		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_MANIPULATE:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.powerups[PW_DISINT_4] = level.time + 60000;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		duration = 500;
		break;
	case FP_ABSORB:
		hearable = qtrue;
		hearDist = 256;
		duration = 20000;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_TEAM_HEAL:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_LIFT:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_DRAIN:
		hearable = qtrue;
		hearDist = 256;
		duration = overrideAmt;
		overrideAmt = 0;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_SEE:
		hearable = qtrue;
		hearDist = 256;
		if (self->client->ps.fd.forcePowerLevel[FP_SEE] == FORCE_LEVEL_1) {
			duration = 10000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_SEE] == FORCE_LEVEL_2) {
			duration = 20000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_SEE] == FORCE_LEVEL_3) {
			duration = 30000;
		}
		else {
			break;
		}

		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_SABER_OFFENSE:
		break;
	case FP_SABER_DEFENSE:
		break;
	case FP_SABERTHROW:
		break;
	default:
		break;
	}

	if(duration) {
		self->client->ps.fd.forcePowerDuration[forcePower] = level.time + duration;
	}
	else {
		self->client->ps.fd.forcePowerDuration[forcePower] = 0;
	}

	if (hearable) {
		self->client->ps.otherSoundLen = hearDist;
		self->client->ps.otherSoundTime = level.time + 100;
	}
	
	self->client->ps.fd.forcePowerDebounce[forcePower] = 0;

	if (forcePower == FP_SPEED && overrideAmt) {
		BG_ForcePowerDrain( &self->client->ps, forcePower, overrideAmt * 0.025 );
	}
	else if (forcePower != FP_GRIP && forcePower != FP_DRAIN && forcePower != FP_MANIPULATE) {
		BG_ForcePowerDrain( &self->client->ps, forcePower, overrideAmt );
	}
}

//[ForceSys]
extern qboolean PM_SaberInParry( int move );
qboolean OJP_CounterForce(gentity_t *attacker, gentity_t *defender, int attackPower);
//[/ForceSys]
extern void BG_ReduceMishapLevel(playerState_t *ps);

qboolean IsHybrid(gentity_t *ent) {
	qboolean isJedi = (qboolean)(ent->client->ps.fd.forcePowersKnown & (1 << FP_SEE));

	return (qboolean)(isJedi && IsMerc(ent->client));
}

qboolean OJP_CounterForce(gentity_t *attacker, gentity_t *defender, int attackPower)
{//generically checks to see if the defender is able to block an attack from this attacker 
	if(BG_IsUsingHeavyWeap(&defender->client->ps)) {
		return qfalse;
	}

	if(IsMerc(defender->client) && defender->client->ps.fd.forcePowerLevel[FP_SEE] >= FORCE_LEVEL_1) {
		return qfalse;
	}

	if( !(defender->client->ps.fd.forcePowersKnown & (1 << attackPower)) 
		&& !(defender->client->ps.fd.forcePowersKnown & (1 << FP_ABSORB)) ) {
		return qfalse;
	}

	//determine ability difference
	int abilityDef = attacker->client->ps.fd.forcePowerLevel[attackPower] - defender->client->ps.fd.forcePowerLevel[attackPower];

	if(abilityDef > attacker->client->ps.fd.forcePowerLevel[attackPower] - defender->client->ps.fd.forcePowerLevel[FP_ABSORB]) {
		//defender's absorb ability is stronger than their attackPower ability, use that instead.
		abilityDef = attacker->client->ps.fd.forcePowerLevel[attackPower] - defender->client->ps.fd.forcePowerLevel[FP_ABSORB];
	}
	
	if(abilityDef >= 2) {
		//defender is largely weaker than the attacker (2 levels)
		if(!WalkCheck(defender) || defender->client->ps.groundEntityNum == ENTITYNUM_NONE) {
			//can't block much stronger Force power while running or in mid-air
			return qfalse;
		}
	}
	else if(abilityDef >= 1) {
		//defender is slightly weaker than their attacker
		if(defender->client->ps.groundEntityNum == ENTITYNUM_NONE) {
			return qfalse;
		}
	}

 	if(PM_SaberInBrokenParry(defender->client->ps.saberMove)) {
		return qfalse;
	}

	if(BG_InSlowBounce(&defender->client->ps) && defender->client->ps.userInt3 & (1 << FLAG_OLDSLOWBOUNCE)) {
		//can't block lightning while in the heavier slow bounces.
		return qfalse;
	}

	if(defender->client->ps.saberAttackChainCount <= BALANCE_LOW) {
		return qfalse;
	}
	
	if( defender->client->ps.saberAttackChainCount <= BALANCE_HIGH
		&& attacker->client->ps.fd.saberAnimLevel == SS_DESANN) {
		return qfalse;
	}

	if (defender->client->ps.forceHandExtend != HANDEXTEND_NONE) {
		return qtrue;
	}

	if(IsHybrid(defender)) {
		defender->client->ps.userInt3 |= (1 << FLAG_BLOCKING);
		defender->client->blockTime = level.time + 1000;
	}
	
	return qtrue;
}

qboolean G_InGetUpAnim(playerState_t *ps)
{
	switch( (ps->legsAnim) )
	{
	case BOTH_GETUP1:
	case BOTH_GETUP2:
	case BOTH_GETUP3:
	case BOTH_GETUP4:
	case BOTH_GETUP5:
	case BOTH_FORCE_GETUP_F1:
	case BOTH_FORCE_GETUP_F2:
	case BOTH_FORCE_GETUP_B1:
	case BOTH_FORCE_GETUP_B2:
	case BOTH_FORCE_GETUP_B3:
	case BOTH_FORCE_GETUP_B4:
	case BOTH_FORCE_GETUP_B5:
	case BOTH_GETUP_BROLL_B:
	case BOTH_GETUP_BROLL_F:
	case BOTH_GETUP_BROLL_L:
	case BOTH_GETUP_BROLL_R:
	case BOTH_GETUP_FROLL_B:
	case BOTH_GETUP_FROLL_F:
	case BOTH_GETUP_FROLL_L:
	case BOTH_GETUP_FROLL_R:
		return qtrue;
	}

	switch( (ps->torsoAnim) )
	{
	case BOTH_GETUP1:
	case BOTH_GETUP2:
	case BOTH_GETUP3:
	case BOTH_GETUP4:
	case BOTH_GETUP5:
	case BOTH_FORCE_GETUP_F1:
	case BOTH_FORCE_GETUP_F2:
	case BOTH_FORCE_GETUP_B1:
	case BOTH_FORCE_GETUP_B2:
	case BOTH_FORCE_GETUP_B3:
	case BOTH_FORCE_GETUP_B4:
	case BOTH_FORCE_GETUP_B5:
	case BOTH_GETUP_BROLL_B:
	case BOTH_GETUP_BROLL_F:
	case BOTH_GETUP_BROLL_L:
	case BOTH_GETUP_BROLL_R:
	case BOTH_GETUP_FROLL_B:
	case BOTH_GETUP_FROLL_F:
	case BOTH_GETUP_FROLL_L:
	case BOTH_GETUP_FROLL_R:
		return qtrue;
	}

	return qfalse;
}

void G_LetGoOfWall( gentity_t *ent ) {
	if ( !ent || !ent->client ) {
		return;
	}

	ent->client->ps.pm_flags &= ~PMF_STUCK_TO_WALL;

	if ( BG_InReboundJump( ent->client->ps.legsAnim ) 
		|| BG_InReboundHold( ent->client->ps.legsAnim ) ) {
		ent->client->ps.legsTimer = 0;
	}

	if ( BG_InReboundJump( ent->client->ps.torsoAnim ) 
		|| BG_InReboundHold( ent->client->ps.torsoAnim)) {
		ent->client->ps.torsoTimer = 0;
	}
}

void WP_ForcePowerStop( gentity_t *self, forcePowers_t forcePower )
{
	int wasActive = self->client->ps.fd.forcePowersActive;

	switch( (int)forcePower )
	{
	case FP_HEAL:
		self->client->ps.fd.forceHealAmount = 0;
		self->client->ps.fd.forceHealTime = 0;
		break;
	case FP_LEVITATION:
		break;
	case FP_SPEED:
		if (wasActive & (1 << FP_SPEED)) {
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_2 - 50], CHAN_VOICE);
		}
		break;
	case FP_PUSH:
		break;
	case FP_PULL:
		break;
	case FP_MINDTRICK:
		if (wasActive & (1 << FP_MINDTRICK)) {
			G_Sound( self, CHAN_AUTO, G_SoundIndex("sound/weapons/force/distractstop.wav") );
		}

		self->client->ps.fd.forceMindtrickTargetIndex = 0;
		self->client->ps.fd.forceMindtrickTargetIndex2 = 0;
		self->client->ps.fd.forceMindtrickTargetIndex3 = 0;
		self->client->ps.fd.forceMindtrickTargetIndex4 = 0;

		break;
	case FP_SEE:
		if (wasActive & (1 << FP_SEE)) {
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_5-50], CHAN_VOICE);
		}
		break;
	case FP_MANIPULATE:
	case FP_GRIP:
		StopGrip(self);
		break;
	case FP_LIGHTNING:
		if ( self->client->ps.fd.forcePowerLevel[FP_LIGHTNING] < FORCE_LEVEL_2 )
		{//don't do it again for 3 seconds, minimum... FIXME: this should be automatic once regeneration is slower (normal)
			self->client->ps.fd.forcePowerDebounce[FP_LIGHTNING] = level.time + 3000;
		}
		else
		{
			self->client->ps.fd.forcePowerDebounce[FP_LIGHTNING] = level.time + 1500;
		}
		
		if (self->client->ps.forceHandExtend == HANDEXTEND_FORCE_HOLD) {
			self->client->ps.forceHandExtendTime = 0; //reset hand position
		}

		self->client->ps.weaponTime=self->client->ps.legsTimer=self->client->ps.torsoTimer=0;
		self->client->ps.activeForcePass = 0;
		break;
	case FP_RAGE:
		self->client->ps.fd.forceRageRecoveryTime = level.time + 10000;
		if (wasActive & (1 << FP_RAGE)) {
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_3-50], CHAN_VOICE);
		}
		break;
	case FP_ABSORB:
		if (wasActive & (1 << FP_ABSORB)) {
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_3-50], CHAN_VOICE);
		}
		break;
	case FP_DRAIN:
		if ( self->client->ps.fd.forcePowerLevel[FP_DRAIN] < FORCE_LEVEL_2 )
		{//don't do it again for 3 seconds, minimum...
			self->client->ps.fd.forcePowerDebounce[FP_DRAIN] = level.time + 3000;
		}
		else {
			self->client->ps.fd.forcePowerDebounce[FP_DRAIN] = level.time + 1500;
		}

		if (self->client->ps.forceHandExtend == HANDEXTEND_FORCE_HOLD) {
			self->client->ps.forceHandExtendTime = 0; //reset hand position
		}

		self->client->ps.activeForcePass = 0;
	default:
		break;
	}
	self->client->ps.fd.forcePowersActive &= ~( 1 << forcePower );
}

//[BugFix27]
//Debouncer information for the lightning
static int LightningDebounceTime = 0;
//sets the time between lightning hit shots on the server so that we can alter the sv_fps without issues.  
const int LIGHTNINGDEBOUNCE = 200;//Was 50
//[/BugFix27]
static int SpeedDebounceTime = 0;
//sets the time between force speed FP drains.  
static int ProtectDebounceTime=0;
//same as above except for Protect
const int SPEEDDEBOUNCE = 200;
const int PROTECTDEBOUNCE = 200;
static void WP_ForcePowerRun( gentity_t *self, forcePowers_t forcePower, usercmd_t *cmd )
{
	extern usercmd_t	ucmd;

	switch( (int)forcePower )
	{
	case FP_LIFT:
		RunForceLift(self);
		break;
	case FP_HEAL:
		if (self->client->ps.fd.forcePowerLevel[FP_HEAL] == FORCE_LEVEL_1)
		{
			if (self->client->ps.velocity[0] || self->client->ps.velocity[1] || self->client->ps.velocity[2])
			{
				WP_ForcePowerStop( self, forcePower );
				break;
			}
		}

		if (self->health < 1 || self->client->ps.stats[STAT_HEALTH] < 1)
		{
			WP_ForcePowerStop( self, forcePower );
			break;
		}

		if (self->client->ps.fd.forceHealTime > level.time)
		{
			break;
		}
		if ( self->health > self->client->ps.stats[STAT_MAX_HEALTH])
		{ //rww - we might start out over max_health and we don't want force heal taking us down to 100 or whatever max_health is
			WP_ForcePowerStop( self, forcePower );
			break;
		}
		self->client->ps.fd.forceHealTime = level.time + 1000;
		self->health++;
		self->client->ps.fd.forceHealAmount++;

		if ( self->health > self->client->ps.stats[STAT_MAX_HEALTH])	// Past max health
		{
			self->health = self->client->ps.stats[STAT_MAX_HEALTH];
			WP_ForcePowerStop( self, forcePower );
		}

		if ( (self->client->ps.fd.forcePowerLevel[FP_HEAL] == FORCE_LEVEL_1 && self->client->ps.fd.forceHealAmount >= 25) ||
			(self->client->ps.fd.forcePowerLevel[FP_HEAL] == FORCE_LEVEL_2 && self->client->ps.fd.forceHealAmount >= 33))
		{
			WP_ForcePowerStop( self, forcePower );
		}
		break;
		case FP_MANIPULATE:
			{
				if (self->client->ps.forceHandExtend != HANDEXTEND_FORCE_HOLD) {
					WP_ForcePowerStop(self, FP_MANIPULATE);
					break;
				}

				if (self->client->ps.fd.forcePowerDebounce[FP_MANIPULATE] < level.time)
				{ //This is sort of not ideal. Using the debounce value reserved for pull for this because pull doesn't need it.
					BG_ForcePowerDrain( &self->client->ps, forcePower, 4 );
					self->client->ps.fd.forcePowerDebounce[FP_MANIPULATE] = level.time + 1000;
				}

				if (self->client->ps.fd.forcePower < 1) {
					WP_ForcePowerStop(self, FP_MANIPULATE);
					break;
				}

				DoManipulateAction(self);
			break;
			}
	case FP_SPEED:
		//This is handled in PM_WalkMove and PM_StepSlideMove
		if ( self->client->holdingObjectiveItem >= MAX_CLIENTS  
			&& self->client->holdingObjectiveItem < ENTITYNUM_WORLD )
		{
			if ( g_entities[self->client->holdingObjectiveItem].genericValue15 )
			{//disables force powers
				WP_ForcePowerStop( self, forcePower );
			}
		}

		if ( (ucmd.buttons & BUTTON_FORCEPOWER) && self->client->ps.fd.forcePowerSelected == FP_SPEED ) {
			self->client->ps.fd.forcePowerDuration[FP_SPEED] = level.time + 500;
		}
	
		if ( self->client->ps.fd.forcePower < forcePowerNeeded[self->client->ps.fd.forcePowerLevel[FP_SPEED]][FP_SPEED] 
			|| self->client->ps.fd.forcePowerDuration[FP_SPEED] < level.time )
		{
			WP_ForcePowerStop( self, forcePower );
		}
		else if( SpeedDebounceTime == level.time //someone already advanced the timer this frame
			|| (level.time - SpeedDebounceTime >= SPEEDDEBOUNCE) )
		{
			BG_ForcePowerDrain( &self->client->ps, forcePower, 0 );
			SpeedDebounceTime = level.time;
		}

		break;
	case FP_GRIP:
		if (self->client->ps.forceHandExtend != HANDEXTEND_FORCE_HOLD) {
			WP_ForcePowerStop(self, FP_GRIP);
			break;
		}

		if (self->client->ps.fd.forcePowerDebounce[FP_PULL] < level.time)
		{ //This is sort of not ideal. Using the debounce value reserved for pull for this because pull doesn't need it.
			if(self->client->ps.fd.forcePowerLevel[FP_GRIP] == FORCE_LEVEL_3) {
				BG_ForcePowerDrain( &self->client->ps, forcePower, 5 );
			}
			else {
				BG_ForcePowerDrain( &self->client->ps, forcePower, 1 );
			}

			self->client->ps.fd.forcePowerDebounce[FP_PULL] = level.time + 500;
		}

		if (self->client->ps.fd.forcePower < 1) {
			WP_ForcePowerStop(self, FP_GRIP);
			break;
		}

		DoGripAction(self, forcePower);
		break;
	case FP_LEVITATION:
		if ( self->client->ps.groundEntityNum != ENTITYNUM_NONE && !self->client->ps.fd.forceJumpZStart) {
			WP_ForcePowerStop( self, forcePower );
		}
		break;
	case FP_RAGE:
		if (self->health < 1) {
			WP_ForcePowerStop(self, forcePower);
			break;
		}

		if (self->client->ps.forceRageDrainTime < level.time) {
			int addTime = 400;

			if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_1) {
				addTime = 150;
			}
			else if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_2) {
				addTime = 300;
			}
			else if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_3) {
				addTime = 450;
			}

			self->client->ps.forceRageDrainTime = level.time + addTime;
		}
		
		break;
	case FP_DRAIN:
		if (self->client->ps.forceHandExtend != HANDEXTEND_FORCE_HOLD)
		{
			WP_ForcePowerStop(self, forcePower);
			break;
		}

		if ( self->client->ps.fd.forcePowerLevel[FP_DRAIN] > FORCE_LEVEL_1 )
		{//higher than level 1
			if ( (cmd->buttons & BUTTON_FORCE_DRAIN) || ((ucmd.buttons & BUTTON_FORCEPOWER) && self->client->ps.fd.forcePowerSelected == FP_DRAIN) )
			{//holding it keeps it going
				self->client->ps.fd.forcePowerDuration[FP_DRAIN] = level.time + 500;
			}
		}
		// OVERRIDEFIXME
		//[ForceSys]
		if ( !WP_ForcePowerAvailable( self, forcePower, 1 ) || self->client->ps.fd.forcePowerDuration[FP_DRAIN] < level.time 
			|| self->client->ps.groundEntityNum == ENTITYNUM_NONE || self->client->ps.fd.forcePower < 1 )
		//if ( !WP_ForcePowerAvailable( self, forcePower, 0 ) || self->client->ps.fd.forcePowerDuration[FP_DRAIN] < level.time ||
		//	self->client->ps.fd.forcePower < 25)
		//[/ForceSys]
		{
			WP_ForcePowerStop( self, forcePower );
		}
		//[ForceSys]
		//replaced drain with force lightning for the moment because drain is WAY over powered when 
		//used in the new saber system.
		//[BugFix27]
		//added server debouncer to make the lightning damage consistant even with different sv_fps settings.
		else if( LightningDebounceTime == level.time //someone already advanced the timer this frame
			|| (level.time - LightningDebounceTime >= LIGHTNINGDEBOUNCE) )
		//else
		//[/BugFix27]
		{
			ForceShootLightning( self );
			//[ForceSys]
			BG_ForcePowerDrain( &self->client->ps, forcePower, 1 );
			//BG_ForcePowerDrain( &self->client->ps, forcePower, 0 );
			//[/ForceSys]

			//show the drain lightning effect
			self->client->ps.activeForcePass = self->client->ps.fd.forcePowerLevel[FP_DRAIN] + FORCE_LEVEL_3;

			//[BugFix27]
			//update the lightning shot debouncer
			LightningDebounceTime = level.time;
			//[/BugFix27]
		}
		//[/ForceSys]
		break;
	case FP_LIGHTNING:
		if (self->client->ps.forceHandExtend != HANDEXTEND_FORCE_HOLD)
		{ //Animation for hand extend doesn't end with hand out, so we have to limit lightning intervals by animation intervals (once hand starts to go in in animation, lightning should stop)
			if(self->client->ps.torsoAnim == BOTH_FORCE_DRAIN_HOLD)
			{
				self->client->ps.forceHandExtend = HANDEXTEND_FORCE_HOLD;
				self->client->ps.forceHandExtendTime = level.time + 20000;
			}
			else
			{
				WP_ForcePowerStop(self, forcePower);
				break;
			}
		}

		if ( self->client->ps.fd.forcePowerLevel[FP_LIGHTNING] > FORCE_LEVEL_1 )
		{//higher than level 1
			if ( (cmd->buttons & BUTTON_FORCE_LIGHTNING) || ((cmd->buttons & BUTTON_FORCEPOWER) && self->client->ps.fd.forcePowerSelected == FP_LIGHTNING) )
			{//holding it keeps it going
				self->client->ps.fd.forcePowerDuration[FP_LIGHTNING] = level.time + 500;
			}
		}

		// OVERRIDEFIXME
		//[ForceSys]
		if ( !WP_ForcePowerAvailable( self, forcePower, 1 ) || self->client->ps.fd.forcePowerDuration[FP_LIGHTNING] < level.time 
			|| self->client->ps.groundEntityNum == ENTITYNUM_NONE || self->client->ps.fd.forcePower < 1 )
		//if ( !WP_ForcePowerAvailable( self, forcePower, 0 ) || self->client->ps.fd.forcePowerDuration[FP_LIGHTNING] < level.time ||
		//	self->client->ps.fd.forcePower < 25 || self->client->ps.groundEntityNum == ENTITYNUM_NONE)
		//[/ForceSys]
		{
			WP_ForcePowerStop( self, forcePower );
		}
		//[BugFix27]
		//added server debouncer to make the lightning damage consistant even with different sv_fps settings.
		else if( LightningDebounceTime == level.time //someone already advanced the timer this frame
			|| (level.time - LightningDebounceTime >= LIGHTNINGDEBOUNCE) )
		//else
		//[/BugFix27]
		{
			ForceShootLightning( self );
			//[ForceSys]
			BG_ForcePowerDrain( &self->client->ps, forcePower, 1 ); //holding FP cost
			//[/ForceSys]

			//[BugFix27]
			//update the lightning shot debouncer
			LightningDebounceTime = level.time;
			//[/BugFix27]
		}
		break;
	case FP_MINDTRICK:
		if ( self->client->holdingObjectiveItem >= MAX_CLIENTS  
			&& self->client->holdingObjectiveItem < ENTITYNUM_WORLD
			&& g_entities[self->client->holdingObjectiveItem].genericValue15 )
		{ //if force hindered can't mindtrick whilst carrying a siege item
			WP_ForcePowerStop( self, FP_MINDTRICK );
		}
		else {
			WP_UpdateMindtrickEnts(self);
		}
		break;
	case FP_SABER_OFFENSE:
		break;
	case FP_SABER_DEFENSE:
		break;
	case FP_SABERTHROW:
		break;
	case FP_ABSORB:
		if (self->client->ps.fd.forcePowerDebounce[forcePower] < level.time) {
			BG_ForcePowerDrain( &self->client->ps, forcePower, 1 );

			if (self->client->ps.fd.forcePower < 1) {
				WP_ForcePowerStop(self, forcePower);
			}

			self->client->ps.fd.forcePowerDebounce[forcePower] = level.time + 600;
		}
		break;
	default:
		break;
	}
}

void WP_DoSpecificPower( gentity_t *self, usercmd_t *ucmd, forcePowers_t forcepower) {
	if (!WP_ForcePowerAvailable(self, forcepower, 0)) {
		return;
	}

	switch(forcepower)
	{
		case FP_HEAL:
			if (self->client->ps.fd.forceButtonNeedRelease) {
				break;
			}

			ForceHeal(self);
			self->client->ps.fd.forceButtonNeedRelease = 1;
			break;
		case FP_LEVITATION:
			//if leave the ground by some other means, cancel the force jump so we don't suddenly jump when we land.
			
			if ( self->client->ps.groundEntityNum == ENTITYNUM_NONE )
			{
				self->client->ps.fd.forceJumpCharge = 0;
				G_MuteSound( self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE );
				//This only happens if the groundEntityNum == ENTITYNUM_NONE when the button is actually released
			}
			else
			{//still on ground, so jump
				ForceJump( self, ucmd );
			}
			break;
		case FP_SPEED:
			ForceSpeed(self, 0);
			break;
		case FP_GRIP:
			if (self->client->ps.fd.forceGripEntityNum == ENTITYNUM_NONE) {
				ForceGrip( self );
			}

			if (self->client->ps.fd.forceGripEntityNum != ENTITYNUM_NONE) {
				if (!WP_ForcePowerInUse(self, FP_GRIP)) {
					WP_ForcePowerStart( self, FP_GRIP, 0 );
					BG_ForcePowerDrain( &self->client->ps, FP_GRIP, GRIP_DRAIN_AMOUNT );
				}
			}
			break;
		case FP_LIGHTNING:
			ForceLightning(self);
			break;
		case FP_PUSH:
			if (self->client->ps.fd.forceButtonNeedRelease && !(self->r.svFlags & SVF_BOT))
			{ //need to release before we can use nonhold powers again
				break;
			}
			ForceThrow(self, qfalse);
			self->client->ps.fd.forceButtonNeedRelease = 1;
			break;
		case FP_PULL:
			if (self->client->ps.fd.forceButtonNeedRelease)
			{ //need to release before we can use nonhold powers again
				break;
			}
			ForceThrow(self, qtrue);
			self->client->ps.fd.forceButtonNeedRelease = 1;
			break;
		case FP_MINDTRICK:
			if (self->client->ps.fd.forceButtonNeedRelease)
			{ //need to release before we can use nonhold powers again
				break;
			}
			ForceTelepathy(self);
			self->client->ps.fd.forceButtonNeedRelease = 1;
			break;
		case FP_RAGE:
			if (self->client->ps.fd.forceButtonNeedRelease)
			{ //need to release before we can use nonhold powers again
				break;
			}
			ForceRage(self);
			self->client->ps.fd.forceButtonNeedRelease = 1;
			break;
		case FP_MANIPULATE:
			if (self->client->ps.fd.forceGripEntityNum == ENTITYNUM_NONE) {
				ForceManipulate(self);
			}

			if (self->client->ps.fd.forceGripEntityNum != ENTITYNUM_NONE) {
				if (!WP_ForcePowerInUse(self, FP_MANIPULATE)) {
					WP_ForcePowerStart( self, FP_MANIPULATE, 0 );
					BG_ForcePowerDrain( &self->client->ps, FP_MANIPULATE, GRIP_DRAIN_AMOUNT );
				}
			}
			break;
		case FP_TEAM_HEAL:
			if (self->client->ps.fd.forceButtonNeedRelease)
			{ //need to release before we can use nonhold powers again
				break;
			}
			ForceTeamHeal(self);
			self->client->ps.fd.forceButtonNeedRelease = 1;
			break;
		case FP_LIFT:
			if (self->client->ps.fd.forceButtonNeedRelease)
			{ //need to release before we can use nonhold powers again
				break;
			}
			ForceLift(self);
			self->client->ps.fd.forceButtonNeedRelease = 1;
			break;
		case FP_DRAIN:
			ForceDrain(self);
			break;
		case FP_SEE:
			if (self->client->ps.fd.forceButtonNeedRelease)
			{ //need to release before we can use nonhold powers again
				break;
			}
			ForceSeeing(self);
			self->client->ps.fd.forceButtonNeedRelease = 1;
			break;
		case FP_SABER_OFFENSE:
			break;
		case FP_SABER_DEFENSE:
			break;
		case FP_SABERTHROW:
			break;
		default:
			break;
	}
}

void HolocronUpdate(gentity_t *self)
{ //keep holocron status updated in holocron mode
	int noHRank = 0;

	if (noHRank < FORCE_LEVEL_0) {
		noHRank = FORCE_LEVEL_0;
	}

	if (noHRank > FORCE_LEVEL_3) {
		noHRank = FORCE_LEVEL_3;
	}

	trap_Cvar_Update(&g_MaxHolocronCarry);
	for(int i = 0; i < NUM_FORCE_POWERS; i++) {
		if (self->client->ps.holocronsCarried[i])
		{ //carrying it, make sure we have the power
			self->client->ps.holocronBits |= (1 << i);
			self->client->ps.fd.forcePowersKnown |= (1 << i);
			self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;
		}
		else
		{ //otherwise, make sure the power is cleared from us
			self->client->ps.fd.forcePowerLevel[i] = 0;
			if (self->client->ps.holocronBits & (1 << i)) {
				self->client->ps.holocronBits -= (1 << i);
			}

			if ((self->client->ps.fd.forcePowersKnown & (1 << i)) && i != FP_LEVITATION && i != FP_SABER_OFFENSE) {
				self->client->ps.fd.forcePowersKnown -= (1 << i);
			}

			if (WP_ForcePowerInUse(self, i) && i != FP_LEVITATION && i != FP_SABER_OFFENSE) {
				WP_ForcePowerStop(self, i);
			}

			if (i == FP_LEVITATION) {
				if (noHRank >= FORCE_LEVEL_1) {
					self->client->ps.fd.forcePowerLevel[i] = noHRank;
				}
				else {
					self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_1;
				}
			}
			else if (i == FP_SABER_OFFENSE) {
				self->client->ps.fd.forcePowersKnown |= (1 << i);

				if (noHRank >= FORCE_LEVEL_1) {
					self->client->ps.fd.forcePowerLevel[i] = noHRank;
				}
				else {
					self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_1;
				}

				//[BugFix46]
				//make sure that the player's saber stance is reset so they can't continue to use that stance 
				//when they don't have the skill for it anymore.
				if (self->client->saber[0].model[0] &&
					self->client->saber[1].model[0])
				{ //dual
					self->client->ps.fd.saberAnimLevelBase = self->client->ps.fd.saberAnimLevel = self->client->ps.fd.saberDrawAnimLevel = SS_DUAL;
				}
				else if ((self->client->saber[0].saberFlags&SFL_TWO_HANDED))
				{ //staff
					self->client->ps.fd.saberAnimLevel = self->client->ps.fd.saberDrawAnimLevel = SS_STAFF;
				}
				else {
					self->client->ps.fd.saberAnimLevelBase = self->client->ps.fd.saberAnimLevel = self->client->ps.fd.saberDrawAnimLevel = SS_MEDIUM;
				}
				//[/BugFix46]
			}
			else {
				self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_0;
			}
		}
	}

	if (HasSetSaberOnly()) { 
		//if saberonly, we get these powers no matter what (still need the holocrons for level 3)
		if (self->client->ps.fd.forcePowerLevel[FP_SABER_OFFENSE] < FORCE_LEVEL_1) {
			self->client->ps.fd.forcePowerLevel[FP_SABER_OFFENSE] = FORCE_LEVEL_1;
		}

		if (self->client->ps.fd.forcePowerLevel[FP_SABER_DEFENSE] < FORCE_LEVEL_1) {
			self->client->ps.fd.forcePowerLevel[FP_SABER_DEFENSE] = FORCE_LEVEL_1;
		}
	}
}

void JediMasterUpdate(gentity_t *self)
{ //keep jedi master status updated for JM gametype
	trap_Cvar_Update(&g_MaxHolocronCarry);

	for(int i = 0; i < NUM_FORCE_POWERS; i++) {
		if (self->client->ps.isJediMaster) {
			self->client->ps.fd.forcePowersKnown |= (1 << i);
			self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;

			if (i == FP_TEAM_HEAL || i == FP_LIFT ||
				i == FP_DRAIN)
			{ //team powers are useless in JM, absorb is too because no one else has powers to absorb. Drain is just
			  //relatively useless in comparison, because its main intent is not to heal, but rather to cripple others
			  //by draining their force at the same time. And no one needs force in JM except the JM himself.
				self->client->ps.fd.forcePowersKnown &= ~(1 << i);
				self->client->ps.fd.forcePowerLevel[i] = 0;
			}

			if (i == FP_MINDTRICK)
			{ //this decision was made because level 3 mindtrick allows the JM to just hide too much, and no one else has force
			  //sight to counteract it. Since the JM himself is the focus of gameplay in this mode, having him hidden for large
			  //durations is indeed a bad thing.
				self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_2;
			}
		}
		else {
			if ((self->client->ps.fd.forcePowersKnown & (1 << i)) && i != FP_LEVITATION) {
				self->client->ps.fd.forcePowersKnown -= (1 << i);
			}

			if (WP_ForcePowerInUse(self, i) && i != FP_LEVITATION) {
				WP_ForcePowerStop(self, i);
			}

			if (i == FP_LEVITATION) {
				self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_1;
			}
			else {
				self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_0;
			}
		}
	}
}

qboolean WP_HasForcePowers( const playerState_t *ps ) {
	if ( ps ) {
		for (int i = 0; i < NUM_FORCE_POWERS; i++ ){
			if ( i == FP_LEVITATION ) {
				if ( ps->fd.forcePowerLevel[i] > FORCE_LEVEL_1 ) {
					return qtrue;
				}
			}
			else if ( ps->fd.forcePowerLevel[i] > FORCE_LEVEL_0 ) {
				return qtrue;
			}
		}
	}

	return qfalse;
}

//try a special roll getup move
qboolean G_SpecialRollGetup(gentity_t *self)
{ //fixme: currently no knockdown will actually land you on your front... so froll's are pretty useless at the moment.
	qboolean rolled = qfalse;

	/*
	if (self->client->ps.weapon != WP_SABER &&
		self->client->ps.weapon != WP_MELEE)
	{ //can't do acrobatics without saber selected
		return qfalse;
	}
	*/

	if (/*!self->client->pers.cmd.upmove &&*/
		self->client->pers.cmd.rightmove > 0 &&
		!self->client->pers.cmd.forwardmove)
	{
		G_SetAnim(self, &self->client->pers.cmd, SETANIM_BOTH, BOTH_GETUP_BROLL_R, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD, 0);
		rolled = qtrue;
	}
	else if (/*!self->client->pers.cmd.upmove &&*/
		self->client->pers.cmd.rightmove < 0 &&
		!self->client->pers.cmd.forwardmove)
	{
		G_SetAnim(self, &self->client->pers.cmd, SETANIM_BOTH, BOTH_GETUP_BROLL_L, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD, 0);
		rolled = qtrue;
	}
	else if (/*self->client->pers.cmd.upmove > 0 &&*/
		!self->client->pers.cmd.rightmove &&
		self->client->pers.cmd.forwardmove > 0)
	{
		G_SetAnim(self, &self->client->pers.cmd, SETANIM_BOTH, BOTH_GETUP_BROLL_F, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD, 0);
		rolled = qtrue;
	}
	else if (/*self->client->pers.cmd.upmove > 0 &&*/
		!self->client->pers.cmd.rightmove &&
		self->client->pers.cmd.forwardmove < 0)
	{
		G_SetAnim(self, &self->client->pers.cmd, SETANIM_BOTH, BOTH_GETUP_BROLL_B, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD, 0);
		rolled = qtrue;
	}
	else if (self->client->pers.cmd.upmove)
	{
		G_PreDefSound(self->client->ps.origin, PDSOUND_FORCEJUMP);
		self->client->ps.forceDodgeAnim = 2;
		self->client->ps.forceHandExtendTime = level.time + 500;

		//self->client->ps.velocity[2] = 300;
	}

	if (rolled)
	{
		G_EntitySound( self, CHAN_VOICE, G_SoundIndex("*jump1.wav") );
	}

	return rolled;
}

//[CloakingVehicles]
extern qboolean DrivingCloakableVehicle(gentity_t *self);
extern void G_ToggleVehicleCloak(playerState_t *ps);
//[/CloakingVehicles]


//[FatigueSys]
static GAME_INLINE qboolean MeditateCheck( gentity_t * self )
{		
	int	anim;

	if ( self->client->saber[0].meditateAnim != -1 )
	{
		anim = self->client->saber[0].meditateAnim;
	}
	else if ( self->client->saber[1].model 
			&& self->client->saber[1].model[0]
			&& self->client->saber[1].meditateAnim != -1 )
	{
		anim = self->client->saber[1].meditateAnim;
	}
	else
	{
		anim = BOTH_MEDITATE; 
	}

	//we don't use a HANDEXTEND_TAUNT check anymore since 
	//it's not always consistantly on during a meditate taunt.
	if( self->client->ps.torsoAnim == anim && self->client->ps.torsoTimer <= 100)
	{
		return qtrue;
	}
	
	return qfalse;
}
//[/FatigueSys]


//[FatigueSys]
extern qboolean PM_SaberInBrokenParry( int move );
extern qboolean PM_InKnockDown( playerState_t *ps );
//[/FatigueSys]
//[Flamethrower]
void Flamethrower_Fire( gentity_t *self );
//[/Flamethrower]
void WP_ForcePowersUpdate( gentity_t *self, usercmd_t *ucmd )
{
	qboolean	usingForce = qfalse;
	int			prepower = 0;

	//[FatigueSys]
	int			FatigueTime;
	//[/FatigueSys]

	//see if any force powers are running
	if (!self ||
		!self->client ||
		self->client->ps.pm_flags & PMF_FOLLOW ||
		self->client->sess.sessionTeam == TEAM_SPECTATOR) {
		return;
	}

	if (!self->client->ps.fd.saberAnimLevel) {//racc - don't ever have SS_NONE?
		self->client->ps.fd.saberAnimLevel = FORCE_LEVEL_1;
	}

	if (self->client->ps.fd.forcePowerSelected < 0) {
		self->client->ps.fd.forcePowerSelected = 0;
	}

	//Fatigued state check.  This is used because not all current deduction sources of FP check to 
	//see if popping the flag is nessicary or not.
	if(self->client->ps.fd.forcePower <= (self->client->ps.fd.forcePowerMax * FATIGUEDTHRESHHOLD))
	{//Pop the Fatigued flag
		self->client->ps.userInt3 |= ( 1 << FLAG_FATIGUED );
	}
	//[/FatigueSys]


	if ( ((self->client->sess.selectedFP != self->client->ps.fd.forcePowerSelected) ||
		(self->client->sess.saberLevel != self->client->ps.fd.saberAnimLevel)) &&
		!(self->r.svFlags & SVF_BOT) )
	{
		if (self->client->sess.updateUITime < level.time)
		{ //a bit hackish, but we don't want the client to flood with userinfo updates if they rapidly cycle
		  //through their force powers or saber attack levels

			self->client->sess.selectedFP = self->client->ps.fd.forcePowerSelected;
			self->client->sess.saberLevel = self->client->ps.fd.saberAnimLevel;
		}
	}

	if (!g_LastFrameTime)
	{
		g_LastFrameTime = level.time;
	}

	if (self->client->ps.forceHandExtend == HANDEXTEND_KNOCKDOWN)
	{
		self->client->ps.zoomFov = 0;
		self->client->ps.zoomMode = 0;
		self->client->ps.zoomLocked = qfalse;
		self->client->ps.zoomTime = 0;
	}

	if (self->client->ps.forceHandExtend == HANDEXTEND_KNOCKDOWN &&
		self->client->ps.forceHandExtendTime >= level.time)
	{
		self->client->ps.saberMove = 0;
		self->client->ps.saberBlocking = 0;
		self->client->ps.saberBlocked = 0;
		self->client->ps.weaponTime = 0;
		self->client->ps.weaponstate = WEAPON_READY;
	}
	else if (self->client->ps.forceHandExtend != HANDEXTEND_NONE &&
		self->client->ps.forceHandExtendTime < level.time)
	{
		if (self->client->ps.forceHandExtend == HANDEXTEND_KNOCKDOWN && !self->client->ps.forceDodgeAnim)
		{
			if (self->health < 1 || (self->client->ps.eFlags & EF_DEAD) || G_SpecialRollGetup(self)) {
				self->client->ps.forceHandExtend = HANDEXTEND_NONE;
			}
			else if(WP_ForcePowerInUse(self, FP_LEVITATION))
			{
				G_EntitySound( self, CHAN_VOICE, G_SoundIndex("*jump1.wav") );
				self->client->ps.forceDodgeAnim = 3;
				self->client->ps.forceHandExtendTime = level.time + 500;
				self->client->ps.velocity[2] = 300;
			}

			self->client->ps.quickerGetup = qfalse;
		}
		else if (self->client->ps.forceHandExtend == HANDEXTEND_POSTTHROWN)
		{
			if (self->health < 1 || (self->client->ps.eFlags & EF_DEAD))
				self->client->ps.forceHandExtend = HANDEXTEND_NONE;
			else if (self->client->ps.groundEntityNum != ENTITYNUM_NONE && !self->client->ps.forceDodgeAnim)
			{
				self->client->ps.forceDodgeAnim = 1;
				self->client->ps.forceHandExtendTime = level.time + 1000;
				G_EntitySound( self, CHAN_VOICE, G_SoundIndex("*jump1.wav") );
				self->client->ps.velocity[2] = 100;
			}
			else if (!self->client->ps.forceDodgeAnim)
				self->client->ps.forceHandExtendTime = level.time + 100;
			else
				self->client->ps.forceHandExtend = HANDEXTEND_WEAPONREADY;
		}
		//[DodgeSys]
		else if(self->client->ps.forceHandExtend == HANDEXTEND_DODGE)
		{//don't do the HANDEXTEND_WEAPONREADY since it screws up our saber block code.
			self->client->ps.forceHandExtend = HANDEXTEND_NONE;
		}
		//[/DodgeSys]
		else {
			self->client->ps.forceHandExtend = HANDEXTEND_WEAPONREADY;
		}
	}

	if (g_gametype.integer == GT_HOLOCRON) {
		HolocronUpdate(self);
	}

	if (g_gametype.integer == GT_JEDIMASTER) {
		JediMasterUpdate(self);
	}

	if (self->client->ps.powerups[PW_FORCE_BOON]) {
		prepower = self->client->ps.fd.forcePower;
	}

	if (self && self->client && (BG_HasYsalamiri(g_gametype.integer, &self->client->ps) ||
		self->client->ps.fd.forceDeactivateAll || self->client->tempSpectate >= level.time))
	{ //has ysalamiri.. or we want to forcefully stop all his active powers
		for(int i = 0; i < NUM_FORCE_POWERS; i++) {
			if (WP_ForcePowerInUse(self, i) && i != FP_LEVITATION) {
				WP_ForcePowerStop(self, i);
			}
		}

		if (self->client->tempSpectate >= level.time)
		{
			self->client->ps.fd.forcePower = 100;
			self->client->ps.fd.forceRageRecoveryTime = 0;
		}

		self->client->ps.fd.forceDeactivateAll = 0;

		if (self->client->ps.fd.forceJumpCharge)
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE);
			self->client->ps.fd.forceJumpCharge = 0;
		}
	}
	else
	{ //otherwise just do a check through them all to see if they need to be stopped for any reason.
		for(int i = 0; i < NUM_FORCE_POWERS; i++) {
			if (WP_ForcePowerInUse(self, i) && i != FP_LEVITATION &&
				!BG_CanUseFPNow(g_gametype.integer, &self->client->ps, level.time, i))
			{
				WP_ForcePowerStop(self, i);
			}
		}
	}

	if (self->client->ps.powerups[PW_FORCE_ENLIGHTENED_LIGHT] || self->client->ps.powerups[PW_FORCE_ENLIGHTENED_DARK])
	{ //enlightenment
		if (!self->client->ps.fd.forceUsingAdded) {
			for(int i = 0; i < NUM_FORCE_POWERS; i++) {
				self->client->ps.fd.forcePowerBaseLevel[i] = self->client->ps.fd.forcePowerLevel[i];
				
				//[ExpSys]
				if(i == FP_HEAL 
					|| i == FP_RAGE 
					|| i == FP_MANIPULATE 
					|| i == FP_TEAM_HEAL
					|| i == FP_LIFT
					|| i == FP_DRAIN)
				{//don't boost the level of Enhanced's disabled force powers.
					i++;
					continue;
				}
	
				if(!forcePowerDarkLight[i] 
					|| (self->client->ps.powerups[PW_FORCE_ENLIGHTENED_LIGHT] && forcePowerDarkLight[i] == FORCE_LIGHTSIDE)
					|| (self->client->ps.powerups[PW_FORCE_ENLIGHTENED_DARK] && forcePowerDarkLight[i] == FORCE_DARKSIDE))
				//[/ExpSys]
				{
					self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;
					self->client->ps.fd.forcePowersKnown |= (1 << i);
				}
			}

			self->client->ps.fd.forceUsingAdded = 1;
		}
	}
	else if (self->client->ps.fd.forceUsingAdded)
	{ //we don't have enlightenment but we're still using enlightened powers, so clear them back to how they should be.
		for(int i = 0; i < NUM_FORCE_POWERS; i++) {
			self->client->ps.fd.forcePowerLevel[i] = self->client->ps.fd.forcePowerBaseLevel[i];
			if (!self->client->ps.fd.forcePowerLevel[i]) {
				if (WP_ForcePowerInUse(self, i)) {
					WP_ForcePowerStop(self, i);
				}

				self->client->ps.fd.forcePowersKnown &= ~(1 << i);
			}

		}

		self->client->ps.fd.forceUsingAdded = 0;
	}

	if (!WP_ForcePowerInUse(self, FP_MINDTRICK))
	{ //clear the mindtrick index values
		self->client->ps.fd.forceMindtrickTargetIndex = 0;
		self->client->ps.fd.forceMindtrickTargetIndex2 = 0;
		self->client->ps.fd.forceMindtrickTargetIndex3 = 0;
		self->client->ps.fd.forceMindtrickTargetIndex4 = 0;
	}
	
	if (self->health < 1) {
		self->client->ps.fd.forceGripBeingGripped = 0;
	}

	if (self->client->ps.fd.forceGripBeingGripped > level.time) {
		self->client->ps.fd.forceGripCripple = 1;

		//keep the saber off during this period
		if (self->client->ps.weapon == WP_SABER && !self->client->ps.saberHolstered) {
			Cmd_ToggleSaber_f(self);
		}
	}
	else {
		self->client->ps.fd.forceGripCripple = 0;
	}

	if (self->client->ps.fd.forceJumpSound) {
		G_PreDefSound(self->client->ps.origin, PDSOUND_FORCEJUMP);
		self->client->ps.fd.forceJumpSound = 0;
	}

	if (self->client->ps.fd.forceGripCripple) {
		if (self->client->ps.fd.forceGripSoundTime < level.time) {
			G_PreDefSound(self->client->ps.origin, PDSOUND_FORCEGRIP);
			self->client->ps.fd.forceGripSoundTime = level.time + 1000;
		}
	}

	if (WP_ForcePowerInUse(self, FP_SPEED)) {
		self->client->ps.powerups[PW_SPEED] = level.time + 100;
	}

	if ( self->health <= 0 )
	{//if dead, deactivate any active force powers
		for (int i = 0; i < NUM_FORCE_POWERS; i++ ) {
			if ( self->client->ps.fd.forcePowerDuration[i] || WP_ForcePowerInUse(self, i) ) {
				WP_ForcePowerStop( self, (forcePowers_t)i );
				self->client->ps.fd.forcePowerDuration[i] = 0;
			}
		}

		goto powersetcheck;
	}

	if (self->client->ps.groundEntityNum != ENTITYNUM_NONE) {
		self->client->fjDidJump = qfalse;
	}

	if (self->client->ps.fd.forceJumpCharge && self->client->ps.groundEntityNum == ENTITYNUM_NONE && self->client->fjDidJump)
	{ //this was for the "charge" jump method... I guess
		if (ucmd->upmove < 10 && (!(ucmd->buttons & BUTTON_FORCEPOWER) || self->client->ps.fd.forcePowerSelected != FP_LEVITATION))
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE);
			self->client->ps.fd.forceJumpCharge = 0;
		}
	}

	if (!(self->client->ps.pm_flags & PMF_JUMP_HELD) && self->client->ps.fd.forceJumpCharge) {
		if (!(ucmd->buttons & BUTTON_FORCEPOWER) ||
			self->client->ps.fd.forcePowerSelected != FP_LEVITATION) {
			WP_DoSpecificPower( self, ucmd, FP_LEVITATION );
		}
	}

	//[Flamethrower]
	if(self->client->flameTime > level.time)
	{//flamethrower is active, flip active flamethrower flag
		if (self->client->ps.stats[STAT_FUEL] < FLAMETHROWER_FUELCOST)
		{//not enough gas, turn it off.
			self->client->flameTime = 0;
			self->client->ps.userInt3 &= ~(1 << FLAG_FLAMETHROWER);
		}
		else
		{//fire flamethrower
			self->client->ps.userInt3 |= (1 << FLAG_FLAMETHROWER);
			self->client->ps.forceHandExtend = HANDEXTEND_FORCE_HOLD;
			self->client->ps.forceHandExtendTime = level.time + 100;

			if( LightningDebounceTime == level.time //someone already advanced the timer this frame
				|| (level.time - LightningDebounceTime >= LIGHTNINGDEBOUNCE) )
			{
				G_Sound( self, CHAN_WEAPON, G_SoundIndex("sound/effects/fireburst") );
				Flamethrower_Fire(self);
				LightningDebounceTime = level.time;

				self->client->ps.stats[STAT_FUEL] -= FLAMETHROWER_FUELCOST;
			}
		}
	}
	else {
		self->client->ps.userInt3 &= ~(1 << FLAG_FLAMETHROWER);
	}
	//[/Flamethrower]

	if ( ucmd->buttons & BUTTON_FORCEGRIP )
	{ //grip is one of the powers with its own button.. if it's held, call the specific grip power function.
		WP_DoSpecificPower( self, ucmd, FP_GRIP );
		usingForce = qtrue;
	}
	else
	{ //see if we're using it generically.. if not, stop.
		if (WP_ForcePowerInUse(self, FP_GRIP)) {
			if (!(ucmd->buttons & BUTTON_FORCEPOWER) || self->client->ps.fd.forcePowerSelected != FP_GRIP) {
				WP_ForcePowerStop(self, FP_GRIP);
			}
		}
	}

	if ( ucmd->buttons & BUTTON_FORCE_LIGHTNING )
	{ //lightning
		//[Flamethrower]
		//ItemUse_FlameThrower(self);
		//[/Flamethrower]
		WP_DoSpecificPower(self, ucmd, FP_LIGHTNING);
		usingForce = qtrue;
	}
	else
	{ //see if we're using it generically.. if not, stop.
		if (WP_ForcePowerInUse(self, FP_LIGHTNING))
		{
			if (!(ucmd->buttons & BUTTON_FORCEPOWER) || self->client->ps.fd.forcePowerSelected != FP_LIGHTNING)
			{
				WP_ForcePowerStop(self, FP_LIGHTNING);
			}
		}
	}

	if ( ucmd->buttons & BUTTON_FORCE_DRAIN )
	{ //drain
		WP_DoSpecificPower(self, ucmd, FP_DRAIN);
		usingForce = qtrue;
	}
	else
	{ //see if we're using it generically.. if not, stop.
		if (WP_ForcePowerInUse(self, FP_DRAIN))
		{
			if (!(ucmd->buttons & BUTTON_FORCEPOWER) || self->client->ps.fd.forcePowerSelected != FP_DRAIN)
			{
				WP_ForcePowerStop(self, FP_DRAIN);
			}
		}
	}

	//[CloakingVehicles]
	if((ucmd->buttons & BUTTON_FORCEPOWER) && self->client->ps.m_iVehicleNum && DrivingCloakableVehicle(self))
	{//player is on a vehicle that can cloak.
		if(!self->client->ps.fd.forceButtonNeedRelease) {
			G_ToggleVehicleCloak(&g_entities[self->client->ps.m_iVehicleNum].client->ps);
			self->client->ps.fd.forceButtonNeedRelease = 1;
		}
	}
	else if( (ucmd->buttons & BUTTON_FORCEPOWER) &&
	//[/CloakingVehicles]
		BG_CanUseFPNow(g_gametype.integer, &self->client->ps, level.time, self->client->ps.fd.forcePowerSelected))
	{
		if (self->client->ps.fd.forcePowerSelected == FP_LEVITATION) {
			ForceJumpCharge( self, ucmd );
			usingForce = qtrue;
		}
		else {
			WP_DoSpecificPower( self, ucmd, self->client->ps.fd.forcePowerSelected );
			
			if (self->client->ps.fd.forcePowerSelected == FP_GRIP) {
				usingForce = qtrue;
			}
		}
	}
	else
	{
		self->client->ps.fd.forceButtonNeedRelease = 0;
	}

	for (int i = 0; i < NUM_FORCE_POWERS; i++ ) {
		if ( self->client->ps.fd.forcePowerDuration[i] ) {
			if ( self->client->ps.fd.forcePowerDuration[i] < level.time ) {
				if (WP_ForcePowerInUse(self, i)) {
					WP_ForcePowerStop( self, (forcePowers_t)i );
				}

				self->client->ps.fd.forcePowerDuration[i] = 0;
			}
		}

		if (WP_ForcePowerInUse(self, i)) {
			usingForce = qtrue;
			WP_ForcePowerRun( self, (forcePowers_t)i, ucmd );
		}
	}

	if ( self->client->ps.saberInFlight && self->client->ps.saberEntityNum )
	{//don't regen force power while throwing saber
		if ( self->client->ps.saberEntityNum < ENTITYNUM_NONE && self->client->ps.saberEntityNum > 0 ) {
			if ( &g_entities[self->client->ps.saberEntityNum] != NULL && g_entities[self->client->ps.saberEntityNum].s.pos.trType == TR_LINEAR ) {
				//fell to the ground and we're trying to pull it back
				usingForce = qtrue;
			}
		}
	}

	if ( !self->client->ps.fd.forcePowersActive || self->client->ps.fd.forcePowersActive == (1 << FP_DRAIN)
		|| self->client->ps.fd.forcePowersActive == (1 << FP_LIFT))
	{//when not using the force, regenerate at 1 point per half second
		//[SaberThrowSys]
		//Saber is going to be gone alot more, better be able to regen without it.
		if (self->client->ps.fd.forcePowerRegenDebounceTime < level.time &&
		//[/SaberThrowSys]
			//[FatigueSys]
			//Don't regen force while attacking with the saber.
			(self->client->ps.weapon != WP_SABER || !BG_SaberInSpecial(self->client->ps.saberMove)) &&
			!BG_SaberAttacking(&self->client->ps) && !BG_SaberInTransitionAny(self->client->ps.saberMove)
			//Don't regen while running
			&& WalkCheck(self)
			&& self->client->ps.groundEntityNum != ENTITYNUM_NONE)  //can't regen while in the air.
			//[/FatigueSys]
		{
			if (g_gametype.integer != GT_HOLOCRON || g_MaxHolocronCarry.value) {
				//let non-jedi force regen since we're doing a more strict jedi/non-jedi thing... this gives dark jedi something to drain
				{
					if (self->client->ps.powerups[PW_FORCE_BOON]) {
						WP_ForcePowerRegenerate(self->client, 6 );
					}
					else if (self->client->ps.isJediMaster && g_gametype.integer == GT_JEDIMASTER) {
						WP_ForcePowerRegenerate(self->client, 4 ); //jedi master regenerates 4 times as fast
					}
					else {
						WP_ForcePowerRegenerate(self->client, 0 );
					}
				}
			}
			else { //regenerate based on the number of holocrons carried
				int holoregen = 0;
				for(int i = 0; i < NUM_FORCE_POWERS; i++) {
					if (self->client->ps.holocronsCarried[i]) {
						holoregen++;
					}
				}

				WP_ForcePowerRegenerate(self->client, holoregen);
			}
			
			{
				if ( g_gametype.integer == GT_POWERDUEL && self->client->sess.duelTeam == DUELTEAM_LONE ) {
					if ( g_duel_fraglimit.integer ) {
						//[FatigueSys]
						FatigueTime = (g_forceRegenTime.integer*
							(0.6 + (.3 * (float)self->client->sess.wins / (float)g_duel_fraglimit.integer)));
						//[/FatigueSys]
					}
					else {
						//[FatigueSys]
						FatigueTime = (g_forceRegenTime.integer*0.7);
						//self->client->ps.fd.forcePowerRegenDebounceTime = level.time + (g_forceRegenTime.integer*0.7);
						//[/FatigueSys]
					}
				}
				else
				{
					//[FatigueSys]
					FatigueTime = g_forceRegenTime.integer;
					//self->client->ps.fd.forcePowerRegenDebounceTime = level.time + g_forceRegenTime.integer;
					//[/FatigueSys]
				}

				//[FatigueSys]
				if(MeditateCheck(self))
				{//more regen rate while meditating
					self->client->ps.fd.forcePowerRegenDebounceTime = level.time + (FatigueTime/4);

					//[Test]
					//simple debugging messages for determining if the meditation regen is working.
					if(d_test.integer != -1 && self->client->ps.clientNum == d_test.integer)
					{
						trap_SendServerCommand( d_test.integer, va("print \"%i: Meditation FP Regen. Next RegenTime: %i\n\"", d_test.integer, FatigueTime/4 ) );
					}
					//[/Test]
				}	
				
				else if(self->client->saber[0].numBlades == 1 && self->client->ps.fd.saberAnimLevel == SS_STAFF)
				{//faster regen rate for singleblade staff/niman's perk
					self->client->ps.fd.forcePowerRegenDebounceTime = level.time + FatigueTime - 100;

					//[Test]
					//simple debugging messages for determining if the meditation regen is working.
					if(d_test.integer != -1 && self->client->ps.clientNum == d_test.integer) {
						trap_SendServerCommand( d_test.integer, va("print \"%i: Niman FP Regen. Next RegenTime: %i\n\"", d_test.integer, FatigueTime-100 ) );
					}
					//[/Test]
				}	
				else	
				{//standard regen
					self->client->ps.fd.forcePowerRegenDebounceTime = level.time + FatigueTime;
					//[Test]
					//simple debugging messages for determining if the meditation regen is working.
					if(d_test.integer != -1 && self->client->ps.clientNum == d_test.integer) {
						trap_SendServerCommand( d_test.integer, va("print \"%i: Normal FP Regen. Next RegenTime: %i\n\"", d_test.integer, FatigueTime ) );
					}
					//[/Test]
				}
				
				//[/FatigueSys]
			}

			//[FatigueSys]
			if( self->client->ps.fd.forcePower > (self->client->ps.fd.forcePowerMax * FATIGUEDTHRESHHOLD) )
			{//You gained some FP back.  Cancel the Fatigue status.
				self->client->ps.userInt3 &= ~( 1 << FLAG_FATIGUED );
			}
			//[/FatigueSys]
		}
	}

	//[SaberSys]
	if(self->client->MishapDebounce < level.time  
		&& !BG_InSlowBounce(&self->client->ps) && !PM_SaberInBrokenParry(self->client->ps.saberMove)
		&& !PM_InKnockDown(&self->client->ps) && self->client->ps.forceHandExtend != HANDEXTEND_DODGE
		&& self->client->ps.saberLockTime < level.time	//not in a saber lock.
		&& self->client->ps.groundEntityNum != ENTITYNUM_NONE)  //can't regen while in the air.
	{
		if(self->client->ps.saberAttackChainCount < BALANCE_MAX) {
			self->client->ps.saberAttackChainCount++;
		}
		
		if(self->client->ps.weapon == WP_SABER)
		{//saberer regens slower since they use MP differently
			if(self->client->ps.fd.saberAnimLevel == SS_MEDIUM) {
				//yellow style is more "centered" and recovers MP faster.
				self->client->MishapDebounce = level.time + (g_mishapRegenTime.integer*.75);
			}
			else {
				self->client->MishapDebounce = level.time + g_mishapRegenTime.integer;
			}
		}
		else {//gunner regen faster
			self->client->MishapDebounce = level.time + g_mishapRegenTime.integer/5;
		}
	}
	//[/SaberSys]
	
powersetcheck:

	if (prepower && self->client->ps.fd.forcePower < prepower) {
		int dif = max(1, ((prepower - self->client->ps.fd.forcePower)/2));

		self->client->ps.fd.forcePower = (prepower - dif);
	}
}

int Jedi_DodgeAnimForLoc(int hitLoc) {
	int	dodgeAnim = -1;
	
	switch( hitLoc )
	{
	case HL_NONE:
	case HL_FOOT_RT:
	case HL_FOOT_LT:
	case HL_LEG_RT:
	case HL_LEG_LT:
		break;

	case HL_BACK_RT:
		dodgeAnim = BOTH_DODGE_FL;
		break;
	case HL_CHEST_RT:
		dodgeAnim = BOTH_DODGE_FR;
		break;
	case HL_BACK_LT:
		dodgeAnim = BOTH_DODGE_FR;
		break;
	case HL_CHEST_LT:
		dodgeAnim = BOTH_DODGE_FR;
		break;
	case HL_BACK:
	case HL_CHEST:
	case HL_WAIST:
		dodgeAnim = BOTH_DODGE_FL;
		break;
	case HL_ARM_RT:
	case HL_HAND_RT:
		dodgeAnim = BOTH_DODGE_L;
		break;
	case HL_ARM_LT:
	case HL_HAND_LT:
		dodgeAnim = BOTH_DODGE_R;
		break;
	case HL_HEAD:
		dodgeAnim = BOTH_DODGE_FL;
		break;
	default:
		break;
	}

	return dodgeAnim;
}

qboolean Jedi_DodgeEvasion( gentity_t *self, gentity_t *shooter, trace_t *tr, int hitLoc ) {
	if ( !self || 
		!self->client || 
		self->health <= 0 ||
		!g_forceDodge.integer ||
		(g_forceDodge.integer != 2 && !WP_ForcePowerInUse(self, FP_SEE)) ||
		self->client->ps.groundEntityNum == ENTITYNUM_NONE ||
		self->client->ps.weaponTime > 0 || 
		self->client->ps.forceHandExtend != HANDEXTEND_NONE ||
		(g_forceDodge.integer == 2 && self->client->ps.fd.forcePowersActive) ||
		(g_forceDodge.integer == 2 && !WP_ForcePowerUsable(self, FP_SPEED))) {
		return qfalse;
	}

	if (g_forceDodge.integer == 2) {
		if ( Q_irand( 1, 7 ) > self->client->ps.fd.forcePowerLevel[FP_SPEED] ){
			//more likely to fail on lower force speed level
			return qfalse;
		}
	}
	else if (self->client->ps.fd.forcePowerLevel[FP_SEE] < FORCE_LEVEL_3) {
		//more likely to fail on lower force sight level
		return qfalse;
	}

	int	dodgeAnim = Jedi_DodgeAnimForLoc(hitLoc);

	if ( dodgeAnim != -1 ) {
		//Our own happy way of forcing an anim:
		self->client->ps.forceHandExtend = HANDEXTEND_DODGE;
		self->client->ps.forceDodgeAnim = dodgeAnim;
		self->client->ps.forceHandExtendTime = level.time + 300;

		self->client->ps.powerups[PW_SPEEDBURST] = level.time + 100;

		if (g_forceDodge.integer == 2) {
			ForceSpeed( self, 500 );
		}
		else {
			G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/force/speed.wav") );
		}

		return qtrue;
	}

	return qfalse;
}
