// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"
#include "../ghoul2/G2.h"
#include "bg_saga.h"

// g_client.c -- client functions that don't happen every frame

static vec3_t	playerMins = {-15, -15, DEFAULT_MINS_2};
static vec3_t	playerMaxs = {15, 15, DEFAULT_MAXS_2};

extern int g_siegeRespawnCheck;
extern int ojp_ffaRespawnTimerCheck;//[FFARespawnTimer]

void WP_SaberAddG2Model( gentity_t *saberent, const char *saberModel, qhandle_t saberSkin );
void WP_SaberRemoveG2Model( gentity_t *saberent );
//[StanceSelection]
//extern qboolean WP_SaberStyleValidForSaber( saberInfo_t *saber1, saberInfo_t *saber2, int saberHolstered, int saberAnimLevel );
//[/StanceSelection]
extern qboolean WP_UseFirstValidSaberStyle( saberInfo_t *saber1, saberInfo_t *saber2, int saberHolstered, int *saberAnimLevel );

forcedata_t Client_Force[MAX_CLIENTS];

//[LastManStanding]
void OJP_Spectator(gentity_t *ent)
{
	if (ent->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		ent->client->tempSpectate = Q3_INFINITE;
		ent->health = ent->client->ps.stats[STAT_HEALTH] = 1;
		ent->client->ps.weapon = WP_NONE;
		ent->client->ps.stats[STAT_WEAPONS] = 0;
		ent->client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
		ent->client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
		ent->takedamage = qfalse;
		trap_LinkEntity(ent);
	}
}
//[/LastManStanding]


/*QUAKED info_player_duel (1 0 1) (-16 -16 -24) (16 16 32) initial
potential spawning position for duelists in duel.
Targets will be fired when someone spawns in on them.
"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
void SP_info_player_duel( gentity_t *ent ) {
	int		i;

	G_SpawnInt( "nobots", "0", &i);
	if ( i ) {
		ent->flags |= FL_NO_BOTS;
	}

	G_SpawnInt( "nohumans", "0", &i );
	if ( i ) {
		ent->flags |= FL_NO_HUMANS;
	}
}

/*QUAKED info_player_duel1 (1 0 1) (-16 -16 -24) (16 16 32) initial
potential spawning position for lone duelists in powerduel.
Targets will be fired when someone spawns in on them.
"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
void SP_info_player_duel1( gentity_t *ent )
{
	int		i;

	G_SpawnInt( "nobots", "0", &i);
	if ( i ) {
		ent->flags |= FL_NO_BOTS;
	}
	G_SpawnInt( "nohumans", "0", &i );
	if ( i ) {
		ent->flags |= FL_NO_HUMANS;
	}
}

/*QUAKED info_player_duel2 (1 0 1) (-16 -16 -24) (16 16 32) initial
potential spawning position for paired duelists in powerduel.
Targets will be fired when someone spawns in on them.
"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
void SP_info_player_duel2( gentity_t *ent )
{
	int		i;

	G_SpawnInt( "nobots", "0", &i);
	if ( i ) {
		ent->flags |= FL_NO_BOTS;
	}
	G_SpawnInt( "nohumans", "0", &i );
	if ( i ) {
		ent->flags |= FL_NO_HUMANS;
	}
}

/*QUAKED info_player_deathmatch (1 0 1) (-16 -16 -24) (16 16 32) initial
potential spawning position for deathmatch games.
The first time a player enters the game, they will be at an 'initial' spot.
Targets will be fired when someone spawns in on them.
"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
void SP_info_player_deathmatch( gentity_t *ent ) {
	int		i;

	G_SpawnInt( "nobots", "0", &i);
	if ( i ) {
		ent->flags |= FL_NO_BOTS;
	}
	G_SpawnInt( "nohumans", "0", &i );
	if ( i ) {
		ent->flags |= FL_NO_HUMANS;
	}

	//[CoOp]
	//make sure the spawnpoint's genericValue1 is cleared so that our magical waypoint
	//system for CoOp will work.
	ent->genericValue1 = 0;
	//[/CoOp]
}

/*QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
Targets will be fired when someone spawns in on them.
equivelant to info_player_deathmatch
*/
void SP_info_player_start(gentity_t *ent) {
	ent->classname = "info_player_deathmatch";
	SP_info_player_deathmatch( ent );
}

/*QUAKED info_player_start_red (1 0 0) (-16 -16 -24) (16 16 32) INITIAL
For Red Team DM starts

Targets will be fired when someone spawns in on them.
equivalent to info_player_deathmatch

INITIAL - The first time a player enters the game, they will be at an 'initial' spot.

"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
void SP_info_player_start_red(gentity_t *ent) {
	SP_info_player_deathmatch( ent );
}

/*QUAKED info_player_start_blue (1 0 0) (-16 -16 -24) (16 16 32) INITIAL
For Blue Team DM starts

Targets will be fired when someone spawns in on them.
equivalent to info_player_deathmatch

INITIAL - The first time a player enters the game, they will be at an 'initial' spot.

"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
void SP_info_player_start_blue(gentity_t *ent) {
	SP_info_player_deathmatch( ent );
}

void SiegePointUse( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	//Toggle the point on/off
	if (self->genericValue1)
	{
		self->genericValue1 = 0;
	}
	else
	{
		self->genericValue1 = 1;
	}
}

/*QUAKED info_player_siegeteam1 (1 0 0) (-16 -16 -24) (16 16 32)
siege start point - team1
name and behavior of team1 depends on what is defined in the
.siege file for this level

startoff - if non-0 spawn point will be disabled until used
idealclass - if specified, this spawn point will be considered
"ideal" for players of this class name. Corresponds to the name
entry in the .scl (siege class) file.
Targets will be fired when someone spawns in on them.
*/
void SP_info_player_siegeteam1(gentity_t *ent) {
	int soff = 0;

	if (g_gametype.integer != GT_SIEGE)
	{ //turn into a DM spawn if not in siege game mode
		ent->classname = "info_player_deathmatch";
		SP_info_player_deathmatch( ent );

		return;
	}

	G_SpawnInt("startoff", "0", &soff);

	if (soff)
	{ //start disabled
		ent->genericValue1 = 0;
	}
	else
	{
		ent->genericValue1 = 1;
	}

	ent->use = SiegePointUse;
}

/*QUAKED info_player_siegeteam2 (0 0 1) (-16 -16 -24) (16 16 32)
siege start point - team2
name and behavior of team2 depends on what is defined in the
.siege file for this level

startoff - if non-0 spawn point will be disabled until used
idealclass - if specified, this spawn point will be considered
"ideal" for players of this class name. Corresponds to the name
entry in the .scl (siege class) file.
Targets will be fired when someone spawns in on them.
*/
void SP_info_player_siegeteam2(gentity_t *ent) {
	int soff = 0;

	if (g_gametype.integer != GT_SIEGE)
	{ //turn into a DM spawn if not in siege game mode
		ent->classname = "info_player_deathmatch";
		SP_info_player_deathmatch( ent );

		return;
	}

	G_SpawnInt("startoff", "0", &soff);

	if (soff)
	{ //start disabled
		ent->genericValue1 = 0;
	}
	else
	{
		ent->genericValue1 = 1;
	}

	ent->use = SiegePointUse;
}

/*QUAKED info_player_intermission (1 0 1) (-16 -16 -24) (16 16 32) RED BLUE
The intermission will be viewed from this point.  Target an info_notnull for the view direction.
RED - In a Siege game, the intermission will happen here if the Red (attacking) team wins
BLUE - In a Siege game, the intermission will happen here if the Blue (defending) team wins
*/
void SP_info_player_intermission( gentity_t *ent ) {

}

/*QUAKED info_player_intermission_red (1 0 1) (-16 -16 -24) (16 16 32)
The intermission will be viewed from this point.  Target an info_notnull for the view direction.

In a Siege game, the intermission will happen here if the Red (attacking) team wins
target - ent to look at
target2 - ents to use when this intermission point is chosen
*/
void SP_info_player_intermission_red( gentity_t *ent ) {

}

/*QUAKED info_player_intermission_blue (1 0 1) (-16 -16 -24) (16 16 32)
The intermission will be viewed from this point.  Target an info_notnull for the view direction.

In a Siege game, the intermission will happen here if the Blue (defending) team wins
target - ent to look at
target2 - ents to use when this intermission point is chosen
*/
void SP_info_player_intermission_blue( gentity_t *ent ) {

}

#define JMSABER_RESPAWN_TIME 20000 //in case it gets stuck somewhere no one can reach

void ThrowSaberToAttacker(gentity_t *self, gentity_t *attacker)
{
	gentity_t *ent = &g_entities[self->client->ps.saberIndex];
	vec3_t a;
	int altVelocity = 0;

	if (!ent || ent->enemy != self)
	{ //something has gone very wrong (this should never happen)
		//but in case it does.. find the saber manually
#ifdef _DEBUG
		Com_Printf("Lost the saber! Attempting to use global pointer..\n");
#endif
		ent = gJMSaberEnt;

		if (!ent)
		{
#ifdef _DEBUG
			Com_Printf("The global pointer was NULL. This is a bad thing.\n");
#endif
			return;
		}

#ifdef _DEBUG
		Com_Printf("Got it (%i). Setting enemy to client %i.\n", ent->s.number, self->s.number);
#endif

		ent->enemy = self;
		self->client->ps.saberIndex = ent->s.number;
	}

	trap_SetConfigstring ( CS_CLIENT_JEDIMASTER, "-1" );

	if (attacker && attacker->client && self->client->ps.saberInFlight)
	{ //someone killed us and we had the saber thrown, so actually move this saber to the saber location
	  //if we killed ourselves with saber thrown, however, same suicide rules of respawning at spawn spot still
	  //apply.
		gentity_t *flyingsaber = &g_entities[self->client->ps.saberEntityNum];

		if (flyingsaber && flyingsaber->inuse)
		{
			VectorCopy(flyingsaber->s.pos.trBase, ent->s.pos.trBase);
			VectorCopy(flyingsaber->s.pos.trDelta, ent->s.pos.trDelta);
			VectorCopy(flyingsaber->s.apos.trBase, ent->s.apos.trBase);
			VectorCopy(flyingsaber->s.apos.trDelta, ent->s.apos.trDelta);

			VectorCopy(flyingsaber->r.currentOrigin, ent->r.currentOrigin);
			VectorCopy(flyingsaber->r.currentAngles, ent->r.currentAngles);
			altVelocity = 1;
		}
	}

	self->client->ps.saberInFlight = qtrue; //say he threw it anyway in order to properly remove from dead body

	WP_SaberAddG2Model( ent, self->client->saber[0].model, self->client->saber[0].skin );

	ent->s.eFlags &= ~(EF_NODRAW);
	ent->s.modelGhoul2 = 1;
	ent->s.eType = ET_MISSILE;
	ent->enemy = NULL;

	if (!attacker || !attacker->client)
	{
		VectorCopy(ent->s.origin2, ent->s.pos.trBase);
		VectorCopy(ent->s.origin2, ent->s.origin);
		VectorCopy(ent->s.origin2, ent->r.currentOrigin);
		ent->pos2[0] = 0;
		trap_LinkEntity(ent);
		return;
	}

	if (!altVelocity)
	{
		VectorCopy(self->s.pos.trBase, ent->s.pos.trBase);
		VectorCopy(self->s.pos.trBase, ent->s.origin);
		VectorCopy(self->s.pos.trBase, ent->r.currentOrigin);

		VectorSubtract(attacker->client->ps.origin, ent->s.pos.trBase, a);

		VectorNormalize(a);

		ent->s.pos.trDelta[0] = a[0]*256;
		ent->s.pos.trDelta[1] = a[1]*256;
		ent->s.pos.trDelta[2] = 256;
	}

	trap_LinkEntity(ent);
}

void JMSaberThink(gentity_t *ent)
{
	gJMSaberEnt = ent;

	if (ent->enemy)
	{//RACC - someone has the JediMaster saber.
		if (!ent->enemy->client || !ent->enemy->inuse)
		{ //disconnected?
			VectorCopy(ent->enemy->s.pos.trBase, ent->s.pos.trBase);
			VectorCopy(ent->enemy->s.pos.trBase, ent->s.origin);
			VectorCopy(ent->enemy->s.pos.trBase, ent->r.currentOrigin);
			ent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
			ent->s.eFlags &= ~(EF_NODRAW);
			ent->s.modelGhoul2 = 1;
			ent->s.eType = ET_MISSILE;
			ent->enemy = NULL;

			ent->pos2[0] = 1;
			ent->pos2[1] = 0; //respawn next think
			trap_LinkEntity(ent);
		}
		else
		{
			ent->pos2[1] = level.time + JMSABER_RESPAWN_TIME;
		}
	}
	else if (ent->pos2[0] && ent->pos2[1] < level.time)
	{//RACC - the jedimaster saber is loose.
		VectorCopy(ent->s.origin2, ent->s.pos.trBase);
		VectorCopy(ent->s.origin2, ent->s.origin);
		VectorCopy(ent->s.origin2, ent->r.currentOrigin);
		ent->pos2[0] = 0;
		trap_LinkEntity(ent);
	}

	ent->nextthink = level.time + 50;
	G_RunObject(ent);
}


//[ExpSys]
void DetermineDodgeMax(gentity_t *ent);
//[/ExpSys]
void JMSaberTouch(gentity_t *self, gentity_t *other, trace_t *trace) {
	int i;
	if (!other || 
		!other->client || 
		other->health < 1 ||
		other->client->ps.stats[STAT_WEAPONS] & (1 << WP_SABER) ||
		other->client->ps.isJediMaster ||
		self->enemy ||
		!self->s.modelindex) {
		return;
	}

	self->enemy = other;
	other->client->ps.stats[STAT_WEAPONS] = (1 << WP_SABER);
	other->client->ps.weapon = WP_SABER;
	other->s.weapon = WP_SABER;
	G_AddEvent(other, EV_BECOME_JEDIMASTER, 0);

	// Track the jedi master 
	trap_SetConfigstring ( CS_CLIENT_JEDIMASTER, va("%i", other->s.number ) );

	if (g_spawnInvulnerability.integer)
	{
		other->client->ps.eFlags |= EF_INVULNERABLE;
		other->client->invulnerableTimer = level.time + g_spawnInvulnerability.integer;
	}

	trap_SendServerCommand( -1, va("cp \"%s %s\n\"", other->client->pers.netname, G_GetStringEdString("MP_SVGAME", "BECOMEJM")) );

	other->client->ps.isJediMaster = qtrue;
	other->client->ps.saberIndex = self->s.number;

	if (other->health < 200 && other->health > 0)
	{ //full health when you become the Jedi Master
		other->client->ps.stats[STAT_HEALTH] = other->health = 200;
	}

	if (other->client->ps.fd.forcePower < 100)
	{

		other->client->ps.fd.forcePower = 100;
	}

	for(i = 0; i < NUM_FORCE_POWERS; i++ ) {
		other->client->ps.fd.forcePowersKnown |= (1 << i);
		other->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;
	}

	//[ExpSys]
	//recalc DP
	DetermineDodgeMax(other);
	//set dp to max.
	other->client->ps.stats[STAT_DODGE] = other->client->ps.stats[STAT_MAX_DODGE]; 
	//[/ExpSys]

	self->pos2[0] = 1;
	self->pos2[1] = level.time + JMSABER_RESPAWN_TIME;

	self->s.modelindex = 0;
	self->s.eFlags |= EF_NODRAW;
	self->s.modelGhoul2 = 0;
	self->s.eType = ET_GENERAL;

	G_KillG2Queue(self->s.number);

	return;
}

gentity_t *gJMSaberEnt = NULL;

/*QUAKED info_jedimaster_start (1 0 0) (-16 -16 -24) (16 16 32)
"jedi master" saber spawn point
*/
void SP_info_jedimaster_start(gentity_t *ent)
{
	if (g_gametype.integer != GT_JEDIMASTER)
	{
		gJMSaberEnt = NULL;
		G_FreeEntity(ent);
		return;
	}

	ent->enemy = NULL;

	ent->flags = FL_BOUNCE_HALF;

	ent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
	ent->s.modelGhoul2 = 1;
	ent->s.g2radius = 20;
	//ent->s.eType = ET_GENERAL;
	ent->s.eType = ET_MISSILE;
	ent->s.weapon = WP_SABER;
	ent->s.pos.trType = TR_GRAVITY;
	ent->s.pos.trTime = level.time;
	VectorSet( ent->r.maxs, 3, 3, 3 );
	VectorSet( ent->r.mins, -3, -3, -3 );
	ent->r.contents = CONTENTS_TRIGGER;
	ent->clipmask = MASK_SOLID;

	ent->isSaberEntity = qtrue;

	ent->bounceCount = -5;

	ent->physicsObject = qtrue;

	VectorCopy(ent->s.pos.trBase, ent->s.origin2); //remember the spawn spot

	ent->touch = JMSaberTouch;

	trap_LinkEntity(ent);

	ent->think = JMSaberThink;
	ent->nextthink = level.time + 50;
}

/*
=======================================================================

  SelectSpawnPoint

=======================================================================
*/

/*
================
SpotWouldTelefrag

================
*/
qboolean SpotWouldTelefrag( gentity_t *spot ) {
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*hit;
	vec3_t		mins, maxs;

	VectorAdd( spot->s.origin, playerMins, mins );
	VectorAdd( spot->s.origin, playerMaxs, maxs );
	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );

	for (i=0 ; i<num ; i++) {
		hit = &g_entities[touch[i]];
		//if ( hit->client && hit->client->ps.stats[STAT_HEALTH] > 0 ) {
		if ( hit->client) {
			return qtrue;
		}

	}

	return qfalse;
}

qboolean SpotWouldTelefrag2( gentity_t *mover, vec3_t dest ) 
{
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*hit;
	vec3_t		mins, maxs;

	VectorAdd( dest, mover->r.mins, mins );
	VectorAdd( dest, mover->r.maxs, maxs );
	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );

	for (i=0 ; i<num ; i++) 
	{
		hit = &g_entities[touch[i]];
		if ( hit == mover )
		{
			continue;
		}

		if ( hit->r.contents & mover->r.contents )
		{
			return qtrue;
		}
	}

	return qfalse;
}

/*
================
SelectNearestDeathmatchSpawnPoint

Find the spot that we DON'T want to use
================
*/
#define	MAX_SPAWN_POINTS	128
gentity_t *SelectNearestDeathmatchSpawnPoint( vec3_t from ) {
	gentity_t	*spot;
	vec3_t		delta;
	float		dist, nearestDist;
	gentity_t	*nearestSpot;

	nearestDist = 999999;
	nearestSpot = NULL;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {

		VectorSubtract( spot->s.origin, from, delta );
		dist = VectorLength( delta );
		if ( dist < nearestDist ) {
			nearestDist = dist;
			nearestSpot = spot;
		}
	}

	return nearestSpot;
}


/*
================
SelectRandomDeathmatchSpawnPoint

go to a random point that doesn't telefrag
================
*/
#define	MAX_SPAWN_POINTS	128
gentity_t *SelectRandomDeathmatchSpawnPoint( void ) {
	gentity_t	*spot = NULL;
	int			count = 0;
	gentity_t	*spots[MAX_SPAWN_POINTS];

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( SpotWouldTelefrag( spot ) ) {
			continue;
		}

		spots[count++] = spot;
	}

	if ( !count ) {	// no spots that won't telefrag
		return G_Find( NULL, FOFS(classname), "info_player_deathmatch");
	}

	return spots[ rand() % count ];
}

/*
===========
SelectRandomFurthestSpawnPoint

Chooses a player start, deathmatch start, etc
============
*/
gentity_t *SelectRandomFurthestSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles, team_t team ) {
	gentity_t	*spot;
	vec3_t		delta;
	float		dist;
	float		list_dist[64];
	gentity_t	*list_spot[64];
	int			numSpots, rnd, i, j;

	numSpots = 0;
	spot = NULL;

	//in Team DM, look for a team start spot first, if any
	if ( g_gametype.integer == GT_TEAM 
		&& team != TEAM_FREE 
		&& team != TEAM_SPECTATOR )
	{
		char *classname = NULL;
		if ( team == TEAM_RED )
		{
			classname = "info_player_start_red";
		}
		else
		{
			classname = "info_player_start_blue";
		}
		while ((spot = G_Find (spot, FOFS(classname), classname)) != NULL) {
			if ( SpotWouldTelefrag( spot ) ) {
				continue;
			}
			VectorSubtract( spot->s.origin, avoidPoint, delta );
			dist = VectorLength( delta );
			for (i = 0; i < numSpots; i++) {
				if ( dist > list_dist[i] ) {
					if ( numSpots >= 64 )
						numSpots = 64-1;
					for (j = numSpots; j > i; j--) {
						list_dist[j] = list_dist[j-1];
						list_spot[j] = list_spot[j-1];
					}
					list_dist[i] = dist;
					list_spot[i] = spot;
					numSpots++;
					if (numSpots > 64)
						numSpots = 64;
					break;
				}
			}
			if (i >= numSpots && numSpots < 64) {
				list_dist[numSpots] = dist;
				list_spot[numSpots] = spot;
				numSpots++;
			}
		}
	}

	if ( !numSpots )
	{//couldn't find any of the above
		while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
			if ( SpotWouldTelefrag( spot ) ) {
				continue;
			}
			VectorSubtract( spot->s.origin, avoidPoint, delta );
			dist = VectorLength( delta );
			for (i = 0; i < numSpots; i++) {
				if ( dist > list_dist[i] ) {
					if ( numSpots >= 64 )
						numSpots = 64-1;
					for (j = numSpots; j > i; j--) {
						list_dist[j] = list_dist[j-1];
						list_spot[j] = list_spot[j-1];
					}
					list_dist[i] = dist;
					list_spot[i] = spot;
					numSpots++;
					if (numSpots > 64)
						numSpots = 64;
					break;
				}
			}
			if (i >= numSpots && numSpots < 64) {
				list_dist[numSpots] = dist;
				list_spot[numSpots] = spot;
				numSpots++;
			}
		}
		if (!numSpots) {
			spot = G_Find( NULL, FOFS(classname), "info_player_deathmatch");
			if (!spot)
				G_Error( "Couldn't find a spawn point" );
			VectorCopy (spot->s.origin, origin);
			origin[2] += 9;
			VectorCopy (spot->s.angles, angles);
			return spot;
		}
	}

	// select a random spot from the spawn points furthest away
	rnd = random() * (numSpots / 2);

	VectorCopy (list_spot[rnd]->s.origin, origin);
	origin[2] += 9;
	VectorCopy (list_spot[rnd]->s.angles, angles);

	return list_spot[rnd];
}

gentity_t *SelectDuelSpawnPoint( int team, vec3_t avoidPoint, vec3_t origin, vec3_t angles )
{
	gentity_t	*spot;
	vec3_t		delta;
	float		dist;
	float		list_dist[64];
	gentity_t	*list_spot[64];
	int			numSpots, rnd, i, j;
	char		*spotName;

	if (team == DUELTEAM_LONE)
	{
		spotName = "info_player_duel1";
	}
	else if (team == DUELTEAM_DOUBLE)
	{
		spotName = "info_player_duel2";
	}
	else if (team == DUELTEAM_SINGLE)
	{
		spotName = "info_player_duel";
	}
	else
	{
		spotName = "info_player_deathmatch";
	}
tryAgain:

	numSpots = 0;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), spotName)) != NULL) {
		if ( SpotWouldTelefrag( spot ) ) {
			continue;
		}
		VectorSubtract( spot->s.origin, avoidPoint, delta );
		dist = VectorLength( delta );
		for (i = 0; i < numSpots; i++) {
			if ( dist > list_dist[i] ) {
				if ( numSpots >= 64 )
					numSpots = 64-1;
				for (j = numSpots; j > i; j--) {
					list_dist[j] = list_dist[j-1];
					list_spot[j] = list_spot[j-1];
				}
				list_dist[i] = dist;
				list_spot[i] = spot;
				numSpots++;
				if (numSpots > 64)
					numSpots = 64;
				break;
			}
		}
		if (i >= numSpots && numSpots < 64) {
			list_dist[numSpots] = dist;
			list_spot[numSpots] = spot;
			numSpots++;
		}
	}
	if (!numSpots)
	{
		if (Q_stricmp(spotName, "info_player_deathmatch"))
		{ //try the loop again with info_player_deathmatch as the target if we couldn't find a duel spot
			spotName = "info_player_deathmatch";
			goto tryAgain;
		}

		//If we got here we found no free duel or DM spots, just try the first DM spot
		spot = G_Find( NULL, FOFS(classname), "info_player_deathmatch");
		if (!spot)
			G_Error( "Couldn't find a spawn point" );
		VectorCopy (spot->s.origin, origin);
		origin[2] += 9;
		VectorCopy (spot->s.angles, angles);
		return spot;
	}

	// select a random spot from the spawn points furthest away
	rnd = random() * (numSpots / 2);

	VectorCopy (list_spot[rnd]->s.origin, origin);
	origin[2] += 9;
	VectorCopy (list_spot[rnd]->s.angles, angles);

	return list_spot[rnd];
}

/*
===========
SelectSpawnPoint

Chooses a player start, deathmatch start, etc
============
*/
gentity_t *SelectSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles, team_t team ) {
	return SelectRandomFurthestSpawnPoint( avoidPoint, origin, angles, team );

	/*
	gentity_t	*spot;
	gentity_t	*nearestSpot;

	nearestSpot = SelectNearestDeathmatchSpawnPoint( avoidPoint );

	spot = SelectRandomDeathmatchSpawnPoint ( );
	if ( spot == nearestSpot ) {
		// roll again if it would be real close to point of death
		spot = SelectRandomDeathmatchSpawnPoint ( );
		if ( spot == nearestSpot ) {
			// last try
			spot = SelectRandomDeathmatchSpawnPoint ( );
		}		
	}

	// find a single player start spot
	if (!spot) {
		G_Error( "Couldn't find a spawn point" );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
	*/
}

/*
===========
SelectInitialSpawnPoint

Try to find a spawn point marked 'initial', otherwise
use normal spawn selection.
============
*/
gentity_t *SelectInitialSpawnPoint( vec3_t origin, vec3_t angles, team_t team ) {
	gentity_t	*spot;

	spot = NULL;
	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( spot->spawnflags & 1 ) {
			break;
		}
	}

	if ( !spot || SpotWouldTelefrag( spot ) ) {
		return SelectSpawnPoint( vec3_origin, origin, angles, team );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
}

/*
===========
SelectSpectatorSpawnPoint

============
*/
gentity_t *SelectSpectatorSpawnPoint( vec3_t origin, vec3_t angles ) {
	FindIntermissionPoint();

	VectorCopy( level.intermission_origin, origin );
	VectorCopy( level.intermission_angle, angles );

	return NULL;
}

/*
=======================================================================

BODYQUE

=======================================================================
*/

/*
=======================================================================

BODYQUE

=======================================================================
*/

//[NOBODYQUE]
// replaced by g_corpseRemovalTime.
//#define BODY_SINK_TIME		30000//45000
//[/NOBODYQUE]

/*
===============
InitBodyQue
===============
*/
//[NOBODYQUE]
//we don't want to use a reserved body que system anymore.
/*
void InitBodyQue (void) {
	int		i;
	gentity_t	*ent;

	level.bodyQueIndex = 0;
	for (i=0; i<BODY_QUEUE_SIZE ; i++) {
		ent = G_Spawn();
		ent->classname = "bodyque";
		ent->neverFree = qtrue;
		level.bodyQue[i] = ent;
	}
}
*/
//[/NOBODYQUE]


/*
=============
BodySink

After sitting around for five seconds, fall into the ground and dissapear
=============
*/
void BodySink( gentity_t *ent ) {
	//[NOBODYQUE]
	//while we're at it, I'm making the corpse removal time be set by a cvar like in SP.
	if(g_corpseRemovalTime.integer && (level.time - ent->timestamp) > g_corpseRemovalTime.integer*1000 + 2500)
	{
	//if ( level.time - ent->timestamp > BODY_SINK_TIME + 2500 ) {

		//removed body que code.  replaced with dynamic body creation.
		//for now we need to manually call the G_KillG2Queue to remove the client's corpse
		//instance.
		G_KillG2Queue(ent->s.number);
		G_FreeEntity(ent);
		// the body ques are never actually freed, they are just unlinked
		//trap_UnlinkEntity( ent );
		//ent->physicsObject = qfalse;
	//[/NOBODYQUE]
		return;	
	}
//	ent->nextthink = level.time + 100;
//	ent->s.pos.trBase[2] -= 1;

	G_AddEvent(ent, EV_BODYFADE, 0);
	ent->nextthink = level.time + 18000;
	ent->takedamage = qfalse;
}

/*
=============
CopyToBodyQue

A player is respawning, so make an entity that looks
just like the existing corpse to leave behind.
=============
*/
static qboolean CopyToBodyQue( gentity_t *ent ) {
	gentity_t		*body;
	int			contents;
	int			islight = 0;

	if (level.intermissiontime)
	{
		return qfalse;
	}

	trap_UnlinkEntity (ent);

	// if client is in a nodrop area, don't leave the body
	contents = trap_PointContents( ent->s.origin, -1 );
	if ( contents & CONTENTS_NODROP ) {
		return qfalse;
	}

	if (ent->client && (ent->client->ps.eFlags & EF_DISINTEGRATION))
	{ //for now, just don't spawn a body if you got disint'd
		return qfalse;
	}

	//[NOBODYQUE]
	body = G_Spawn();

	body->classname = "body";

	// grab a body que and cycle to the next one
	/*
	body = level.bodyQue[ level.bodyQueIndex ];
	level.bodyQueIndex = (level.bodyQueIndex + 1) % BODY_QUEUE_SIZE;
	*/
	//[/NOBODYQUE]

	trap_UnlinkEntity (body);
	body->s = ent->s;

	//avoid oddly angled corpses floating around
	body->s.angles[PITCH] = body->s.angles[ROLL] = body->s.apos.trBase[PITCH] = body->s.apos.trBase[ROLL] = 0;

	body->s.g2radius = 100;

	body->s.eType = ET_BODY;
	body->s.eFlags = EF_DEAD;		// clear EF_TALK, etc

	if (ent->client && (ent->client->ps.eFlags & EF_DISINTEGRATION))
	{
		body->s.eFlags |= EF_DISINTEGRATION;
	}

	VectorCopy(ent->client->ps.lastHitLoc, body->s.origin2);

	body->s.powerups = 0;	// clear powerups
	body->s.loopSound = 0;	// clear lava burning
	body->s.loopIsSoundset = qfalse;
	body->s.number = body - g_entities;
	body->timestamp = level.time;
	body->physicsObject = qtrue;
	body->physicsBounce = 0;		// don't bounce
	if ( body->s.groundEntityNum == ENTITYNUM_NONE ) {
		body->s.pos.trType = TR_GRAVITY;
		body->s.pos.trTime = level.time;
		VectorCopy( ent->client->ps.velocity, body->s.pos.trDelta );
	} else {
		body->s.pos.trType = TR_STATIONARY;
	}

	body->s.event = 0;

	body->s.weapon = ent->s.bolt2;

	if (body->s.weapon == WP_SABER && ent->client->ps.saberInFlight)
	{
		//[NOBODYQUE]
		//actually we shouldn't use have any weapon at all if we died like this.
		body->s.weapon = WP_MELEE;
		//body->s.weapon = WP_BLASTER; //lie to keep from putting a saber on the corpse, because it was thrown at death
		//[/NOBODYQUE]
	}

	//G_AddEvent(body, EV_BODY_QUEUE_COPY, ent->s.clientNum);
	//Now doing this through a modified version of the rcg reliable command.
	if (ent->client && ent->client->ps.fd.forceSide == FORCE_LIGHTSIDE)
	{
		islight = 1;
	}
	trap_SendServerCommand(-1, va("ircg %i %i %i %i", ent->s.number, body->s.number, body->s.weapon, islight));

	body->r.svFlags = ent->r.svFlags | SVF_BROADCAST;

	VectorCopy (ent->r.mins, body->r.mins);
	VectorCopy (ent->r.maxs, body->r.maxs);
	VectorCopy (ent->r.absmin, body->r.absmin);
	VectorCopy (ent->r.absmax, body->r.absmax);

	body->s.torsoAnim = body->s.legsAnim = ent->client->ps.legsAnim;

	body->s.customRGBA[0] = ent->client->ps.customRGBA[0];
	body->s.customRGBA[1] = ent->client->ps.customRGBA[1];
	body->s.customRGBA[2] = ent->client->ps.customRGBA[2];
	body->s.customRGBA[3] = ent->client->ps.customRGBA[3];

	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
	body->r.contents = CONTENTS_CORPSE;
	body->r.ownerNum = ent->s.number;

	//[NOBODYQUE]
	if(g_corpseRemovalTime.integer)
	{
		body->nextthink = level.time + g_corpseRemovalTime.integer*1000;
		//body->nextthink = level.time + BODY_SINK_TIME;

		body->think = BodySink;
	}
	//[/NOBODYQUE]

	body->die = body_die;

	// don't take more damage if already gibbed
	if ( ent->health <= GIB_HEALTH ) {
		body->takedamage = qfalse;
	} else {
		body->takedamage = qtrue;
	}

	VectorCopy ( body->s.pos.trBase, body->r.currentOrigin );
	trap_LinkEntity (body);

	return qtrue;
}

//======================================================================


/*
==================
SetClientViewAngle

==================
*/
void SetClientViewAngle( gentity_t *ent, vec3_t angle ) {
	int			i;

	// set the delta angle
	for (i=0 ; i<3 ; i++) {
		int		cmdAngle;

		cmdAngle = ANGLE2SHORT(angle[i]);
		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
	}
	VectorCopy( angle, ent->s.angles );
	VectorCopy (ent->s.angles, ent->client->ps.viewangles);
}

void MaintainBodyQueue(gentity_t *ent)
{ //do whatever should be done taking ragdoll and dismemberment states into account.
	qboolean doRCG = qfalse;

	assert(ent && ent->client);
	if (ent->client->tempSpectate > level.time ||
		(ent->client->ps.eFlags2 & EF2_SHIP_DEATH))
	{
		ent->client->noCorpse = qtrue;
	}

	if (!ent->client->noCorpse && !ent->client->ps.fallingToDeath)
	{//racc - we're suppose to have a corpse so attempt to create one.
		if (!CopyToBodyQue (ent))
		{//racc - no luck, but still remember to restore all the limb damage on the player
			doRCG = qtrue;
		}
	}
	else
	{
		ent->client->noCorpse = qfalse; //clear it for next time
		ent->client->ps.fallingToDeath = qfalse;
		doRCG = qtrue;
	}

	if (doRCG)
	{ //bodyque func didn't manage to call ircg so call this to assure our limbs and ragdoll states are proper on the client.
		trap_SendServerCommand(-1, va("rcg %i", ent->s.clientNum));
	}
}

/*
================
respawn
================
*/
void SiegeRespawn(gentity_t *ent);
void respawn( gentity_t *ent ) {
	MaintainBodyQueue(ent);

	if (gEscaping || g_gametype.integer == GT_POWERDUEL)
	{
		ent->client->sess.sessionTeam = TEAM_SPECTATOR;
		ent->client->sess.spectatorState = SPECTATOR_FREE;
		ent->client->sess.spectatorClient = 0;

		ent->client->pers.teamState.state = TEAM_BEGIN;
		ent->client->sess.spectatorTime = level.time;
		ClientSpawn(ent);
		ent->client->iAmALoser = qtrue;
		return;
	}

	trap_UnlinkEntity (ent);

	//[LastManStanding]
	if (ojp_lms.integer > 0 && ent->lives < 1 && BG_IsLMSGametype(g_gametype.integer) && LMS_EnoughPlayers())
	{//playing LMS and we're DEAD!  Just start chillin in tempSpec.
		OJP_Spectator(ent);
	}
	else if(g_gametype.integer == GT_FFA || g_gametype.integer == GT_TEAM
		|| g_gametype.integer == GT_CTF)
	{
		if (ojp_ffaRespawnTimer.integer)
		{
			if (ent->client->tempSpectate <= level.time)
			{
				int minDel = g_siegeRespawn.integer* 2000;
				if (minDel < 20000)
				{
					minDel = 20000;
				}
				OJP_Spectator(ent);
				ent->client->tempSpectate = level.time + minDel;

				// Respawn time.
				if ( ent->s.number < MAX_CLIENTS )
				{
					gentity_t *te = G_TempEntity( ent->client->ps.origin, EV_SIEGESPEC );
					te->s.time = ojp_ffaRespawnTimerCheck;
					te->s.owner = ent->s.number;
				}

				return;
			}
		}
		ClientSpawn(ent);
		//[LastManStanding]
		if ( ojp_lms.integer > 0 && BG_IsLMSGametype(g_gametype.integer) && LMS_EnoughPlayers())
		{//reduce our number of lives since we respawned and we're not the only player.
			ent->lives--;
		}
		//[/LastManStanding]
	}
	else if (g_gametype.integer == GT_SIEGE)
	//if (g_gametype.integer == GT_SIEGE)
	//[/LastManStanding]
	{
		if (g_siegeRespawn.integer)
		{
			if (ent->client->tempSpectate <= level.time)
			{
				int minDel = g_siegeRespawn.integer* 2000;
				if (minDel < 20000)
				{
					minDel = 20000;
				}
				ent->client->tempSpectate = level.time + minDel;
				ent->health = ent->client->ps.stats[STAT_HEALTH] = 1;
				ent->client->ps.weapon = WP_NONE;
				ent->client->ps.stats[STAT_WEAPONS] = 0;
				ent->client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
				ent->client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
				ent->takedamage = qfalse;
				trap_LinkEntity(ent);

				// Respawn time.
				if ( ent->s.number < MAX_CLIENTS )
				{
					gentity_t *te = G_TempEntity( ent->client->ps.origin, EV_SIEGESPEC );
					te->s.time = g_siegeRespawnCheck;
					te->s.owner = ent->s.number;
				}

				return;
			}
		}
		SiegeRespawn(ent);
	}
	else
	{
		gentity_t	*tent;

		ClientSpawn(ent);
		// add a teleportation effect
		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
		tent->s.clientNum = ent->s.clientNum;
	}
}

/*
================
TeamCount

Returns number of players on a team
================
*/
team_t TeamCount( int ignoreClientNum, int team ) {
	int		count = 0;
	int i;

	for (i = 0 ; i < level.maxclients ; i++ ) {
		if ( i == ignoreClientNum ) {
			continue;
		}

		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
			continue;
		}

		if ( level.clients[i].sess.sessionTeam == team ) {
			count++;
		}
		else if (g_gametype.integer == GT_SIEGE &&
            level.clients[i].sess.siegeDesiredTeam == team)
		{
			count++;
		}
	}

	return count;
}

/*
================
TeamLeader

Returns the client number of the team leader
================
*/
int TeamLeader( int team ) {
	int i;
	for (i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
			continue;
		}

		if ( level.clients[i].sess.sessionTeam == team ) {
			if ( level.clients[i].sess.teamLeader ) {
				return i;
			}
		}
	}

	return -1;
}


/*
================
PickTeam

================
*/
//[AdminSys]
int G_CountHumanPlayers( int ignoreClientNum, int team );
team_t PickTeam( int ignoreClientNum, qboolean isBot ) {
//team_t PickTeam( int ignoreClientNum ) {
//[/AdminSys]
	int		counts[TEAM_NUM_TEAMS];

	counts[TEAM_BLUE] = TeamCount( ignoreClientNum, TEAM_BLUE );
	counts[TEAM_RED] = TeamCount( ignoreClientNum, TEAM_RED );

	//[AdminSys]
	if(g_teamForceBalance.integer == 4 && !isBot)
	{//human is autojoining the game, place them on the team with the fewest humans.  
		//This will make things work out in the end. :)
		int humanCount[TEAM_NUM_TEAMS];
		humanCount[TEAM_RED] = G_CountHumanPlayers( ignoreClientNum, TEAM_RED );
		humanCount[TEAM_BLUE] = G_CountHumanPlayers( ignoreClientNum, TEAM_BLUE );
		if( humanCount[TEAM_RED] > humanCount[TEAM_BLUE] )
		{
			return TEAM_BLUE;
		}

		if( humanCount[TEAM_BLUE] > humanCount[TEAM_RED] )
		{
			return TEAM_RED;
		}
	}
	//[/AdminSys]

	if ( counts[TEAM_BLUE] > counts[TEAM_RED] ) {
		return TEAM_RED;
	}
	if ( counts[TEAM_RED] > counts[TEAM_BLUE] ) {
		return TEAM_BLUE;
	}
	// equal team count, so join the team with the lowest score
	if ( level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED] ) {
		return TEAM_RED;
	}
	return TEAM_BLUE;
}

/*
===========
ClientCheckName
============
*/
static void ClientCleanName( const char *in, char *out, int outSize ) {
	int		len, colorlessLen;
	char	ch;
	char	*p;
	int		spaces;

	//save room for trailing null byte
	outSize--;

	len = 0;
	colorlessLen = 0;
	p = out;
	*p = 0;
	spaces = 0;

	while( 1 ) {
		ch = *in++;
		if( !ch ) {
			break;
		}

		// don't allow leading spaces
		if( !*p && ch == ' ' ) {
			continue;
		}

		// check colors
		if( ch == Q_COLOR_ESCAPE ) {
			// solo trailing carat is not a color prefix
			if( !*in ) {
				break;
			}

			// don't allow black in a name, period
			
			if( ColorIndex(*in) == 0 ) {
				in++;
				continue;
			}
			

			// make sure room in dest for both chars
			if( len > outSize - 2 ) {
				break;
			}

			*out++ = ch;
			*out++ = *in++;
			len += 2;
			continue;
		}

		// don't allow too many consecutive spaces
		if( ch == ' ' ) {
			spaces++;
			if( spaces > 3 ) {
				continue;
			}
		}
		else {
			spaces = 0;
		}

		if( len > outSize - 1 ) {
			break;
		}

		*out++ = ch;
		colorlessLen++;
		len++;
	}
	*out = 0;

	// don't allow empty names
	if( *p == 0 || colorlessLen == 0 ) {
		Q_strncpyz( p, "Padawan", outSize );
	}
}

#ifdef _DEBUG
void G_DebugWrite(const char *path, const char *text)
{
	fileHandle_t f;

	trap_FS_FOpenFile( path, &f, FS_APPEND );
	trap_FS_Write(text, strlen(text), f);
	trap_FS_FCloseFile(f);
}
#endif

qboolean G_SaberModelSetup(gentity_t *ent)
{
	int i = 0;
	qboolean fallbackForSaber = qtrue;

	while (i < MAX_SABERS)
	{
		if (ent->client->saber[i].model[0])
		{
			//first kill it off if we've already got it
			if (ent->client->weaponGhoul2[i])
			{
				trap_G2API_CleanGhoul2Models(&(ent->client->weaponGhoul2[i]));
			}
			trap_G2API_InitGhoul2Model(&ent->client->weaponGhoul2[i], ent->client->saber[i].model, 0, 0, -20, 0, 0);

			if (ent->client->weaponGhoul2[i])
			{
				int j = 0;
				char *tagName;
				int tagBolt;

				if (ent->client->saber[i].skin)
				{
					trap_G2API_SetSkin(ent->client->weaponGhoul2[i], 0, ent->client->saber[i].skin, ent->client->saber[i].skin);
				}

				if (ent->client->saber[i].saberFlags & SFL_BOLT_TO_WRIST)
				{
					trap_G2API_SetBoltInfo(ent->client->weaponGhoul2[i], 0, 3+i);
				}
				else
				{ // bolt to right hand for 0, or left hand for 1
					trap_G2API_SetBoltInfo(ent->client->weaponGhoul2[i], 0, i);
				}

				//Add all the bolt points
				while (j < ent->client->saber[i].numBlades)
				{
					tagName = va("*blade%i", j+1);
					tagBolt = trap_G2API_AddBolt(ent->client->weaponGhoul2[i], 0, tagName);

					if (tagBolt == -1)
					{
						if (j == 0)
						{ //guess this is an 0ldsk3wl saber
							tagBolt = trap_G2API_AddBolt(ent->client->weaponGhoul2[i], 0, "*flash");
							fallbackForSaber = qfalse;
							break;
						}

						if (tagBolt == -1)
						{
							assert(0);
							break;

						}
					}
					j++;

					fallbackForSaber = qfalse; //got at least one custom saber so don't need default
				}

				//Copy it into the main instance
				trap_G2API_CopySpecificGhoul2Model(ent->client->weaponGhoul2[i], 0, ent->ghoul2, i+1); 
			}
		}
		else
		{
			break;
		}

		i++;
	}

	return fallbackForSaber;
}

/*
===========
SetupGameGhoul2Model

There are two ghoul2 model instances per player (actually three).  One is on the clientinfo (the base for the client side 
player, and copied for player spawns and for corpses).  One is attached to the centity itself, which is the model acutally 
animated and rendered by the system.  The final is the game ghoul2 model.  This is animated by pmove on the server, and
is used for determining where the lightsaber should be, and for per-poly collision tests.
===========
*/
void *g2SaberInstance = NULL;


qboolean BG_IsValidCharacterModel(const char *modelName, const char *skinName);
qboolean BG_ValidateSkinForTeam( const char *modelName, char *skinName, int team, float *colors );
void BG_GetVehicleModelName(char *modelname);


void SetupGameGhoul2Model(gentity_t *ent, char *modelname, char *skinName)
{
	int handle;
	char		afilename[MAX_QPATH];
	char		GLAName[MAX_QPATH];
	vec3_t	tempVec = {0,0,0};

	// First things first.  If this is a ghoul2 model, then let's make sure we demolish this first.
	if (ent->ghoul2 && trap_G2_HaveWeGhoul2Models(ent->ghoul2))
	{
		trap_G2API_CleanGhoul2Models(&(ent->ghoul2));
	}

	//rww - just load the "standard" model for the server"
	if (!precachedKyle)
	{
		int defSkin;

		Com_sprintf( afilename, sizeof( afilename ), "models/players/kyle/model.glm" );
		handle = trap_G2API_InitGhoul2Model(&precachedKyle, afilename, 0, 0, -20, 0, 0);

		if (handle<0)
		{
			return;
		}

		defSkin = trap_R_RegisterSkin("models/players/kyle/model_default.skin");
		trap_G2API_SetSkin(precachedKyle, 0, defSkin, defSkin);
	}

	if (precachedKyle && trap_G2_HaveWeGhoul2Models(precachedKyle))
	{
		if (d_perPlayerGhoul2.integer || ent->s.number >= MAX_CLIENTS ||
			G_PlayerHasCustomSkeleton(ent))
		{ //rww - allow option for perplayer models on server for collision and bolt stuff.
			char modelFullPath[MAX_QPATH];
			char truncModelName[MAX_QPATH];
			char skin[MAX_QPATH];
			char vehicleName[MAX_QPATH];
			int skinHandle = 0;
			int i = 0;
			char *p;

			// If this is a vehicle, get it's model name.
			if ( ent->client->NPC_class == CLASS_VEHICLE )
			{
				strcpy(vehicleName, modelname);
				BG_GetVehicleModelName(modelname);
				strcpy(truncModelName, modelname);
				skin[0] = 0;
				if ( ent->m_pVehicle
					&& ent->m_pVehicle->m_pVehicleInfo
					&& ent->m_pVehicle->m_pVehicleInfo->skin
					&& ent->m_pVehicle->m_pVehicleInfo->skin[0] )
				{
					skinHandle = trap_R_RegisterSkin(va("models/players/%s/model_%s.skin", modelname, ent->m_pVehicle->m_pVehicleInfo->skin));
				}
				else
				{
					skinHandle = trap_R_RegisterSkin(va("models/players/%s/model_default.skin", modelname));
				}
			}
			else
			{
				if (skinName && skinName[0])
				{
					strcpy(skin, skinName);
					strcpy(truncModelName, modelname);
				}
				else
				{
					strcpy(skin, "default");

					strcpy(truncModelName, modelname);
					p = Q_strrchr(truncModelName, '/');

					if (p)
					{
						*p = 0;
						p++;

						while (p && *p)
						{
							skin[i] = *p;
							i++;
							p++;
						}
						skin[i] = 0;
						i = 0;
					}

					if (!BG_IsValidCharacterModel(truncModelName, skin))
					{
						strcpy(truncModelName, "kyle");
						strcpy(skin, "default");
					}

					if ( g_gametype.integer >= GT_TEAM && g_gametype.integer != GT_SIEGE && !g_trueJedi.integer )
					{
						//[BugFix32]
						//Also adjust customRGBA for team colors.
						float colorOverride[3];

						colorOverride[0] = colorOverride[1] = colorOverride[2] = 0.0f;

						BG_ValidateSkinForTeam( truncModelName, skin, ent->client->sess.sessionTeam, colorOverride);
						if (colorOverride[0] != 0.0f ||
							colorOverride[1] != 0.0f ||
							colorOverride[2] != 0.0f)
						{
							ent->client->ps.customRGBA[0] = colorOverride[0]*255.0f;
							ent->client->ps.customRGBA[1] = colorOverride[1]*255.0f;
							ent->client->ps.customRGBA[2] = colorOverride[2]*255.0f;
						}

						//BG_ValidateSkinForTeam( truncModelName, skin, ent->client->sess.sessionTeam, NULL );
						//[/BugFix32]
					}
					else if (g_gametype.integer == GT_SIEGE)
					{ //force skin for class if appropriate
						if (ent->client->siegeClass != -1)
						{
							siegeClass_t *scl = &bgSiegeClasses[ent->client->siegeClass];
							if (scl->forcedSkin[0])
							{
								strcpy(skin, scl->forcedSkin);
							}
						}
					}
				}
			}

			if (skin[0])
			{
				char *useSkinName;

				if (strchr(skin, '|'))
				{//three part skin
					useSkinName = va("models/players/%s/|%s", truncModelName, skin);
				}
				else
				{
					useSkinName = va("models/players/%s/model_%s.skin", truncModelName, skin);
				}

				skinHandle = trap_R_RegisterSkin(useSkinName);
			}

			strcpy(modelFullPath, va("models/players/%s/model.glm", truncModelName));
			handle = trap_G2API_InitGhoul2Model(&ent->ghoul2, modelFullPath, 0, skinHandle, -20, 0, 0);

			if (handle<0)
			{ //Huh. Guess we don't have this model. Use the default.

				if (ent->ghoul2 && trap_G2_HaveWeGhoul2Models(ent->ghoul2))
				{
					trap_G2API_CleanGhoul2Models(&(ent->ghoul2));
				}
				ent->ghoul2 = NULL;
				trap_G2API_DuplicateGhoul2Instance(precachedKyle, &ent->ghoul2);
			}
			else
			{
				trap_G2API_SetSkin(ent->ghoul2, 0, skinHandle, skinHandle);

				GLAName[0] = 0;
				trap_G2API_GetGLAName( ent->ghoul2, 0, GLAName);

				if (!GLAName[0] || (!strstr(GLAName, "players/_humanoid/") && ent->s.number < MAX_CLIENTS && !G_PlayerHasCustomSkeleton(ent)))
				{ //a bad model
					trap_G2API_CleanGhoul2Models(&(ent->ghoul2));
					ent->ghoul2 = NULL;
					trap_G2API_DuplicateGhoul2Instance(precachedKyle, &ent->ghoul2);
				}

				if (ent->s.number >= MAX_CLIENTS)
				{
					ent->s.modelGhoul2 = 1; //so we know to free it on the client when we're removed.

					if (skin[0])
					{ //append it after a *
						strcat( modelFullPath, va("*%s", skin) );
					}

					if ( ent->client->NPC_class == CLASS_VEHICLE )
					{ //vehicles are tricky and send over their vehicle names as the model (the model is then retrieved based on the vehicle name)
						ent->s.modelindex = G_ModelIndex(vehicleName);
					}
					else
					{
						ent->s.modelindex = G_ModelIndex(modelFullPath);
					}
				}
			}
		}
		else
		{
			trap_G2API_DuplicateGhoul2Instance(precachedKyle, &ent->ghoul2);
		}
	}
	else
	{
		return;
	}

	//Attach the instance to this entity num so we can make use of client-server
	//shared operations if possible.
	trap_G2API_AttachInstanceToEntNum(ent->ghoul2, ent->s.number, qtrue);

	// The model is now loaded.

	GLAName[0] = 0;

	if (!BGPAFtextLoaded)
	{
		if (BG_ParseAnimationFile("models/players/_humanoid/animation.cfg", bgHumanoidAnimations, qtrue) == -1)
		{
			Com_Printf( "Failed to load humanoid animation file\n");
			return;
		}
	}

	if (ent->s.number >= MAX_CLIENTS || G_PlayerHasCustomSkeleton(ent))
	{
		ent->localAnimIndex = -1;

		GLAName[0] = 0;
		trap_G2API_GetGLAName(ent->ghoul2, 0, GLAName);

		if (GLAName[0] &&
			!strstr(GLAName, "players/_humanoid/") /*&&
			!strstr(GLAName, "players/rockettrooper/")*/)
		{ //it doesn't use humanoid anims.
			char *slash = Q_strrchr( GLAName, '/' );
			if ( slash )
			{
				strcpy(slash, "/animation.cfg");

				ent->localAnimIndex = BG_ParseAnimationFile(GLAName, NULL, qfalse);
			}
		}
		else
		{ //humanoid index.
			if (strstr(GLAName, "players/rockettrooper/"))
			{
				ent->localAnimIndex = 1;
			}
			else
			{
				ent->localAnimIndex = 0;
			}
		}

		if (ent->localAnimIndex == -1)
		{
			Com_Error(ERR_DROP, "NPC had an invalid GLA\n");
		}
	}
	else
	{
		GLAName[0] = 0;
		trap_G2API_GetGLAName(ent->ghoul2, 0, GLAName);

		if (strstr(GLAName, "players/rockettrooper/"))
		{
			//assert(!"Should not have gotten in here with rockettrooper skel");
			ent->localAnimIndex = 1;
		}
		else
		{
			ent->localAnimIndex = 0;
		}
	}

	if (ent->s.NPC_class == CLASS_VEHICLE &&
		ent->m_pVehicle)
	{ //do special vehicle stuff
		char strTemp[128];
		int i;

		// Setup the default first bolt
		i = trap_G2API_AddBolt( ent->ghoul2, 0, "model_root" );

		// Setup the droid unit.
		ent->m_pVehicle->m_iDroidUnitTag = trap_G2API_AddBolt( ent->ghoul2, 0, "*droidunit" );

		// Setup the Exhausts.
		for ( i = 0; i < MAX_VEHICLE_EXHAUSTS; i++ )
		{
			Com_sprintf( strTemp, 128, "*exhaust%i", i + 1 );
			ent->m_pVehicle->m_iExhaustTag[i] = trap_G2API_AddBolt( ent->ghoul2, 0, strTemp );
		}

		// Setup the Muzzles.
		for ( i = 0; i < MAX_VEHICLE_MUZZLES; i++ )
		{
			Com_sprintf( strTemp, 128, "*muzzle%i", i + 1 );
			ent->m_pVehicle->m_iMuzzleTag[i] = trap_G2API_AddBolt( ent->ghoul2, 0, strTemp );
			if ( ent->m_pVehicle->m_iMuzzleTag[i] == -1 )
			{//ergh, try *flash?
				Com_sprintf( strTemp, 128, "*flash%i", i + 1 );
				ent->m_pVehicle->m_iMuzzleTag[i] = trap_G2API_AddBolt( ent->ghoul2, 0, strTemp );
			}
		}

		// Setup the Turrets.
		for ( i = 0; i < MAX_VEHICLE_TURRET_MUZZLES; i++ )
		{
			if ( ent->m_pVehicle->m_pVehicleInfo->turret[i].gunnerViewTag )
			{
				ent->m_pVehicle->m_iGunnerViewTag[i] = trap_G2API_AddBolt( ent->ghoul2, 0, ent->m_pVehicle->m_pVehicleInfo->turret[i].gunnerViewTag );
			}
			else
			{
				ent->m_pVehicle->m_iGunnerViewTag[i] = -1;
			}
		}
	}
	
	if (ent->client->ps.weapon == WP_SABER || ent->s.number < MAX_CLIENTS)
	{ //a player or NPC saber user
		trap_G2API_AddBolt(ent->ghoul2, 0, "*r_hand");
		trap_G2API_AddBolt(ent->ghoul2, 0, "*l_hand");

		//rhand must always be first bolt. lhand always second. Whichever you want the
		//jetpack bolted to must always be third.
		trap_G2API_AddBolt(ent->ghoul2, 0, "*chestg");

		//claw bolts
		trap_G2API_AddBolt(ent->ghoul2, 0, "*r_hand_cap_r_arm");
		trap_G2API_AddBolt(ent->ghoul2, 0, "*l_hand_cap_l_arm");

		trap_G2API_SetBoneAnim(ent->ghoul2, 0, "model_root", 0, 12, BONE_ANIM_OVERRIDE_LOOP, 1.0f, level.time, -1, -1);
		trap_G2API_SetBoneAngles(ent->ghoul2, 0, "upper_lumbar", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, level.time);
		trap_G2API_SetBoneAngles(ent->ghoul2, 0, "cranium", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, NULL, 0, level.time);

		if (!g2SaberInstance)
		{
			trap_G2API_InitGhoul2Model(&g2SaberInstance, "models/weapons2/saber/saber_w.glm", 0, 0, -20, 0, 0);

			if (g2SaberInstance)
			{
				// indicate we will be bolted to model 0 (ie the player) on bolt 0 (always the right hand) when we get copied
				trap_G2API_SetBoltInfo(g2SaberInstance, 0, 0);
				// now set up the gun bolt on it
				trap_G2API_AddBolt(g2SaberInstance, 0, "*blade1");
			}
		}

		if (G_SaberModelSetup(ent))
		{
			if (g2SaberInstance)
			{
				trap_G2API_CopySpecificGhoul2Model(g2SaberInstance, 0, ent->ghoul2, 1); 
			}
		}
	}

	if (ent->s.number >= MAX_CLIENTS)
	{ //some extra NPC stuff
		if (trap_G2API_AddBolt(ent->ghoul2, 0, "lower_lumbar") == -1)
		{ //check now to see if we have this bone for setting anims and such
			ent->noLumbar = qtrue;
		}

		//[CoOp]
		//add some extra bolts for some of the NPC classes
		if ( ent->client->NPC_class == CLASS_HOWLER )
		{
			ent->NPC->genericBolt1 = trap_G2API_AddBolt(&ent->ghoul2, 0, "Tongue01" );// tongue base
			ent->NPC->genericBolt2 = trap_G2API_AddBolt(&ent->ghoul2, 0, "Tongue08" );// tongue tip
		}
		//[/CoOp]
	}
}




/*
===========
ClientUserInfoChanged

Called from ClientConnect when the player first connects and
directly by the server system when the player updates a userinfo variable.

The game can override any of the settings and call trap_SetUserinfo
if desired.
============
*/
//[CoOp]
qboolean WinterGear = qfalse;  //sets weither or not the models go for winter gear skins
								//or not.
//[/CoOp]
qboolean G_SetSaber(gentity_t *ent, int saberNum, char *saberName, qboolean siegeOverride);
void G_ValidateSiegeClassForTeam(gentity_t *ent, int team);
void ClientUserinfoChanged( int clientNum ) {
	gentity_t *ent;
	int		teamTask, teamLeader, team, health;
	char	*s;
	char	model[MAX_QPATH];
	char	oldname[MAX_STRING_CHARS];
	gclient_t	*client;
	char	c1[MAX_INFO_STRING];
	char	c2[MAX_INFO_STRING];
	char	userinfo[MAX_INFO_STRING];
	char	className[MAX_QPATH]; //name of class type to use in siege
	char	saberName[MAX_QPATH];
	char	saber2Name[MAX_QPATH];
	char	*value;
	int		maxHealth;

	//[RGBSabers]
	char	rgb1[MAX_INFO_STRING];
	char	rgb2[MAX_INFO_STRING];
	char	script1[MAX_INFO_STRING];
	char	script2[MAX_INFO_STRING];
	//[/RGBSabers]

	qboolean	modelChanged = qfalse;

	ent = g_entities + clientNum;
	client = ent->client;

	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );

	// check for malformed or illegal info strings
	if ( !Info_Validate(userinfo) ) {
		strcpy (userinfo, "\\name\\badinfo");
	}

	// check for local client
	s = Info_ValueForKey( userinfo, "ip" );
	if ( !strcmp( s, "localhost" ) ) {
		client->pers.localClient = qtrue;
	}

	// check the item prediction
	s = Info_ValueForKey( userinfo, "cg_predictItems" );
	if ( !atoi( s ) ) {
		client->pers.predictItemPickup = qfalse;
	} else {
		client->pers.predictItemPickup = qtrue;
	}

	// set name
	Q_strncpyz ( oldname, client->pers.netname, sizeof( oldname ) );
	s = Info_ValueForKey (userinfo, "name");
	ClientCleanName( s, client->pers.netname, sizeof(client->pers.netname) );

	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
			Q_strncpyz( client->pers.netname, "scoreboard", sizeof(client->pers.netname) );
		}
	}

	if ( client->pers.connected == CON_CONNECTED ) {
		if ( strcmp( oldname, client->pers.netname ) ) 
		{
			if ( client->pers.netnameTime > level.time  )
			{
				trap_SendServerCommand( clientNum, va("print \"%s\n\"", G_GetStringEdString("MP_SVGAME", "NONAMECHANGE")) );

				Info_SetValueForKey( userinfo, "name", oldname );
				trap_SetUserinfo( clientNum, userinfo );			
				strcpy ( client->pers.netname, oldname );
			}
			else
			{				
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s %s\n\"", oldname, G_GetStringEdString("MP_SVGAME", "PLRENAME"), client->pers.netname) );
				client->pers.netnameTime = level.time + 5000;
			}
		}
	}

	// set model
	Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );

	//[CoOp]
	if (WinterGear)
	{//use winter gear
		char *skinname = strstr ( model, "|" );
		if(skinname)
		{//we're using a species player model, try to use their hoth clothes.
			skinname++;
			strcpy( skinname, "torso_g1|lower_e1\0" );
		}
	}
	//[/CoOp]

	if (d_perPlayerGhoul2.integer)
	{
		if (Q_stricmp(model, client->modelname))
		{
			strcpy(client->modelname, model);
			modelChanged = qtrue;
		}
	}

	//Get the skin RGB based on his userinfo
	value = Info_ValueForKey (userinfo, "char_color_red");
	if (value)
	{
		client->ps.customRGBA[0] = atoi(value);
	}
	else
	{
		client->ps.customRGBA[0] = 255;
	}

	value = Info_ValueForKey (userinfo, "char_color_green");
	if (value)
	{
		client->ps.customRGBA[1] = atoi(value);
	}
	else
	{
		client->ps.customRGBA[1] = 255;
	}

	value = Info_ValueForKey (userinfo, "char_color_blue");
	if (value)
	{
		client->ps.customRGBA[2] = atoi(value);
	}
	else
	{
		client->ps.customRGBA[2] = 255;
	}

	if ((client->ps.customRGBA[0]+client->ps.customRGBA[1]+client->ps.customRGBA[2]) < 100)
	{ //hmm, too dark!
		client->ps.customRGBA[0] = client->ps.customRGBA[1] = client->ps.customRGBA[2] = 255;
	}

	client->ps.customRGBA[3]=255;

	//[BugFix32]
	//update our customRGBA for team colors. 
	if ( g_gametype.integer >= GT_TEAM && g_gametype.integer != GT_SIEGE && !g_trueJedi.integer )
	{
		char skin[MAX_QPATH];
		float colorOverride[3];

		colorOverride[0] = colorOverride[1] = colorOverride[2] = 0.0f;

		BG_ValidateSkinForTeam( model, skin, client->sess.sessionTeam, colorOverride);
		if (colorOverride[0] != 0.0f ||
			colorOverride[1] != 0.0f ||
			colorOverride[2] != 0.0f)
		{
			client->ps.customRGBA[0] = colorOverride[0]*255.0f;
			client->ps.customRGBA[1] = colorOverride[1]*255.0f;
			client->ps.customRGBA[2] = colorOverride[2]*255.0f;
		}
	}
	//[/BugFix32]

	//[BugFix14]
	//Testing to see if this fixes the problem with a bot's team getting set incorrectly.
	team = client->sess.sessionTeam;
	//[/BugFix14]

	//Set the siege class
	if (g_gametype.integer == GT_SIEGE)
	{
		strcpy(className, client->sess.siegeClass);
		//Now that the team is legal for sure, we'll go ahead and get an index for it.
		client->siegeClass = BG_SiegeFindClassIndexByName(className);
		if (client->siegeClass == -1)
		{ //ok, get the first valid class for the team you're on then, I guess.
			BG_SiegeCheckClassLegality(team, className);
			strcpy(client->sess.siegeClass, className);
			client->siegeClass = BG_SiegeFindClassIndexByName(className);
		}
		else
		{ //otherwise, make sure the class we are using is legal.
			G_ValidateSiegeClassForTeam(ent, team);
			strcpy(className, client->sess.siegeClass);
		}

		//Set the sabers if the class dictates
		if (client->siegeClass != -1)
		{
			siegeClass_t *scl = &bgSiegeClasses[client->siegeClass];

			if (scl->saber1[0])
			{
				G_SetSaber(ent, 0, scl->saber1, qtrue);
			}
			else
			{ //default I guess
				G_SetSaber(ent, 0, "Kyle", qtrue);
			}
			if (scl->saber2[0])
			{
				G_SetSaber(ent, 1, scl->saber2, qtrue);
			}
			else
			{ //no second saber then
				G_SetSaber(ent, 1, "none", qtrue);
			}

			//make sure the saber models are updated
			G_SaberModelSetup(ent);

			if (scl->forcedModel[0])
			{ //be sure to override the model we actually use
				strcpy(model, scl->forcedModel);
				if (d_perPlayerGhoul2.integer)
				{
					if (Q_stricmp(model, client->modelname))
					{
						strcpy(client->modelname, model);
						modelChanged = qtrue;
					}
				}
			}

			//force them to use their class model on the server, if the class dictates
			if (G_PlayerHasCustomSkeleton(ent))
			{
				if (Q_stricmp(model, client->modelname) || ent->localAnimIndex == 0)
				{
					strcpy(client->modelname, model);
					modelChanged = qtrue;
				}
			}
		}
	}
	else
	{
		strcpy(className, "none");
	}

	//Set the saber name
	strcpy(saberName, client->sess.saberType);
	strcpy(saber2Name, client->sess.saber2Type);

	// set max health
	if (g_gametype.integer == GT_SIEGE && client->siegeClass != -1)
	{
		siegeClass_t *scl = &bgSiegeClasses[client->siegeClass];
		maxHealth = 100;

		if (scl->maxhealth)
		{
			maxHealth = scl->maxhealth;
		}

		health = maxHealth;
	}
	else
	{
		maxHealth = 100;
		health = 100; //atoi( Info_ValueForKey( userinfo, "handicap" ) );
	}
	client->pers.maxHealth = health;
	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > maxHealth ) {
		client->pers.maxHealth = 100;
	}
	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;

	if (g_gametype.integer >= GT_TEAM) {
		client->pers.teamInfo = qtrue;
	} else {
		s = Info_ValueForKey( userinfo, "teamoverlay" );
		if ( ! *s || atoi( s ) != 0 ) {
			client->pers.teamInfo = qtrue;
		} else {
			client->pers.teamInfo = qfalse;
		}
	}
	/*
	s = Info_ValueForKey( userinfo, "cg_pmove_fixed" );
	if ( !*s || atoi( s ) == 0 ) {
		client->pers.pmoveFixed = qfalse;
	}
	else {
		client->pers.pmoveFixed = qtrue;
	}
	*/

	// team task (0 = none, 1 = offence, 2 = defence)
	teamTask = atoi(Info_ValueForKey(userinfo, "teamtask"));
	// team Leader (1 = leader, 0 is normal player)
	teamLeader = client->sess.teamLeader;

	// colors
	strcpy(c1, Info_ValueForKey( userinfo, "color1" ));
	strcpy(c2, Info_ValueForKey( userinfo, "color2" ));

//	strcpy(redTeam, Info_ValueForKey( userinfo, "g_redteam" ));
//	strcpy(blueTeam, Info_ValueForKey( userinfo, "g_blueteam" ));

	//[ClientPlugInDetect]
	s = Info_ValueForKey( userinfo, "ojp_clientplugin" );
	if ( !*s  ) 
	{
		client->pers.ojpClientPlugIn = qfalse;
	}
	else if(!strcmp(CURRENT_OJPENHANCED_CLIENTVERSION, s))
	{
		client->pers.ojpClientPlugIn = qtrue;
	}
	//[/ClientPlugInDetect]

	//[RGBSabers]
	Q_strncpyz(rgb1,Info_ValueForKey(userinfo, "rgb_saber1"), sizeof(rgb1));
	Q_strncpyz(rgb2,Info_ValueForKey(userinfo, "rgb_saber2"), sizeof(rgb2));

	Q_strncpyz(script1,Info_ValueForKey(userinfo, "rgb_script1"), sizeof(script1));
	Q_strncpyz(script2,Info_ValueForKey(userinfo, "rgb_script2"), sizeof(script2));


//	Com_Printf("game > newinfo update > sab1 \"%s\" sab2 \"%s\" \n",rgb1,rgb2);

	// send over a subset of the userinfo keys so other clients can
	// print scoreboards, display models, and play custom sounds
	//[RGBSabers]
	if ( ent->r.svFlags & SVF_BOT ) {
		s = va("n\\%s\\t\\%i\\model\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\skill\\%s\\tt\\%d\\tl\\%d\\siegeclass\\%s\\st\\%s\\st2\\%s\\dt\\%i\\sdt\\%i\\tc1\\%s\\tc2\\%s\\ss1\\%s\\ss2\\%s",
			client->pers.netname, team, model,  c1, c2, 
			client->pers.maxHealth, client->sess.wins, client->sess.losses,
			Info_ValueForKey( userinfo, "skill" ), teamTask, teamLeader, className, saberName, saber2Name, client->sess.duelTeam, client->sess.siegeDesiredTeam,
			rgb1,rgb2,script1,script2);
	} else {
		if (g_gametype.integer == GT_SIEGE)
		{ //more crap to send
			s = va("n\\%s\\t\\%i\\model\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\tt\\%d\\tl\\%d\\siegeclass\\%s\\st\\%s\\st2\\%s\\dt\\%i\\sdt\\%i\\tc1\\%s\\tc2\\%s\\ss1\\%s\\ss2\\%s",
				client->pers.netname, client->sess.sessionTeam, model, c1, c2, 
				client->pers.maxHealth, client->sess.wins, client->sess.losses, teamTask, teamLeader, className, saberName, saber2Name, client->sess.duelTeam, client->sess.siegeDesiredTeam,rgb1,rgb2,script1,script2);
		}
		else
		{
			s = va("n\\%s\\t\\%i\\model\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\tt\\%d\\tl\\%d\\st\\%s\\st2\\%s\\dt\\%i\\tc1\\%s\\tc2\\%s\\ss1\\%s\\ss2\\%s",
				client->pers.netname, client->sess.sessionTeam, model, c1, c2, 
				client->pers.maxHealth, client->sess.wins, client->sess.losses, teamTask, teamLeader, saberName, saber2Name, client->sess.duelTeam,rgb1,rgb2,script1,script2);
	//[/RGBSabers]
		}
	}

	trap_SetConfigstring( CS_PLAYERS+clientNum, s );

	if (modelChanged) //only going to be true for allowable server-side custom skeleton cases
	{ //update the server g2 instance if appropriate
		char *modelname = Info_ValueForKey (userinfo, "model");
		SetupGameGhoul2Model(ent, modelname, NULL);

		if (ent->ghoul2 && ent->client)
		{
			ent->client->renderInfo.lastG2 = NULL; //update the renderinfo bolts next update.
		}

		client->torsoAnimExecute = client->legsAnimExecute = -1;
		client->torsoLastFlip = client->legsLastFlip = qfalse;
	}

	if (g_logClientInfo.integer)
	{
		G_LogPrintf( "ClientUserinfoChanged: %i %s\n", clientNum, s );
	}
}


/*
===========
ClientConnect

Called when a player begins connecting to the server.
Called again for every map change or tournement restart.

The session information will be valid after exit.

Return NULL if the client should be allowed, otherwise return
a string with the reason for denial.

Otherwise, the client will be sent the current gamestate
and will eventually get to ClientBegin.

firstTime will be qtrue the very first time a client connects
to the server machine, but qfalse on map changes and tournement
restarts.
============
*/
char *ClientConnect( int clientNum, qboolean firstTime, qboolean isBot ) {
	char		*value;
//	char		*areabits;
	gclient_t	*client;
	char		userinfo[MAX_INFO_STRING];
	//[AdminSys]
	char		IPstring[32]={0};
	//[/AdminSys]
	gentity_t	*ent;
	gentity_t	*te;

	ent = &g_entities[ clientNum ];

	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
	//[LastManStanding]
	if(ojp_lms.integer > 0 && BG_IsLMSGametype(g_gametype.integer) )
	{//LMS mode, set up lives.
		ent->lives = (ojp_lmslives.integer >= 1) ? ojp_lmslives.integer : 1; 
		//[Coop]
		if (g_gametype.integer == GT_SINGLE_PLAYER)
		{//LMS mode, playing Coop, setup liveExp
			ent->liveExp = 0;
		}
		//[/Coop]
	}
	//[/LastManStanding]

	// check to see if they are on the banned IP list
	value = Info_ValueForKey (userinfo, "ip");
	//[AdminSys]
	Q_strncpyz(IPstring, value, sizeof(IPstring) );
	//[/AdminSys]

	if ( G_FilterPacket( value ) ) {
		return "Banned.";
	}

	if ( !isBot && g_needpass.integer ) {
		value = Info_ValueForKey (userinfo, "password");

		if ( g_password.string[0] && Q_stricmp( g_password.string, "none" ) && strcmp( g_password.string, value) != 0) {
			if( !sv_privatepassword.string[0] || strcmp( sv_privatepassword.string, value ) ) {
				static char sTemp[1024];
				Q_strncpyz(sTemp, G_GetStringEdString("MP_SVGAME","INVALID_ESCAPE_TO_MAIN"), sizeof (sTemp) );
				return sTemp;// return "Invalid password";
			}
		}
	}

	// they can connect
	ent->client = level.clients + clientNum;
	client = ent->client;

	//assign the pointer for bg entity access
	ent->playerState = &ent->client->ps;

	//[ExpSys]
	//initialize the player's entity now so that ent->s.number will be valid.  Otherwise, poor clientNum 0 will always get 
	//spammed whenever someone joins the game.
	G_InitGentity(ent);
	//[/ExpSys]

//	areabits = client->areabits;

	memset( client, 0, sizeof(*client) );

	client->pers.connected = CON_CONNECTING;

	// read or initialize the session data
	if ( firstTime || level.newSession ) {
		G_InitSessionData( client, userinfo, isBot, firstTime );
	}

	G_ReadSessionData( client );

	//[AdminSys]
	client->sess.IPstring[0] = 0;
	Q_strncpyz(client->sess.IPstring, IPstring, sizeof(client->sess.IPstring) );
	//[/AdminSys]

	if (g_gametype.integer == GT_SIEGE &&
		(firstTime || level.newSession))
	{ //if this is the first time then auto-assign a desired siege team and show briefing for that team
		client->sess.siegeDesiredTeam = 0;//PickTeam(ent->s.number);
		//don't just show it - they'll see it if they switch to a team on purpose.
	}

	if (g_gametype.integer == GT_SIEGE && client->sess.sessionTeam != TEAM_SPECTATOR) {
		if (firstTime || level.newSession) {
			client->sess.siegeDesiredTeam = client->sess.sessionTeam;
			client->sess.sessionTeam = TEAM_SPECTATOR;
		}
	}
	//[BugFix22]
	//this was boning over all the careful player selection stuff that happened before rounds in powerduel.
	/* basejka code
	else if (g_gametype.integer == GT_POWERDUEL && client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		client->sess.sessionTeam = TEAM_SPECTATOR;
	}
	*/
	//[/BugFix22]

	if( isBot ) {
		ent->r.svFlags |= SVF_BOT;
		ent->inuse = qtrue;
		//[Linux]
		//if( !G_BotConnect( clientNum, !firstTime ) ) {
		if( !G_BotConnect( clientNum, (qboolean) (!firstTime) ) ) {
		//[/Linux]
			return "BotConnectfailed";
		}
	}

	// get and distribute relevent paramters
	G_LogPrintf( "ClientConnect: %i\n", clientNum );
	ClientUserinfoChanged( clientNum );
	//[AdminSys]
	if( !isBot ){// MJN - bots don't have IP's ;)
		G_LogPrintf( "%s" S_COLOR_WHITE " connected with IP: %s\n", client->pers.netname, client->sess.IPstring );
	}
	else{// MJN - We'll say this instead.
		G_LogPrintf( "*****Spawning Bot %s" S_COLOR_WHITE "***** \n", client->pers.netname );
	}
	//G_LogPrintf(  "%s" S_COLOR_WHITE " connected with IP: %s\n", client->pers.netname, client->sess.IPstring );
	//[/AdminSys]

	// don't do the "xxx connected" messages if they were caried over from previous level
	if ( firstTime ) {
		trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStringEdString("MP_SVGAME", "PLCONNECT")) );
	}

	if ( g_gametype.integer >= GT_TEAM &&
		client->sess.sessionTeam != TEAM_SPECTATOR ) {
		BroadcastTeamChange( client, -1 );
	}

	// count current clients and rank for scoreboard
	CalculateRanks();


	te = G_TempEntity( vec3_origin, EV_CLIENTJOIN );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = clientNum;

	return NULL;
}

void G_WriteClientSessionData( gclient_t *client );


void WP_SetSaber( int entNum, saberInfo_t *sabers, int saberNum, const char *saberName );


/*
===========
ClientBegin

called when a client has finished connecting, and is ready
to be placed into the level.  This will happen every level load,
and on transition between teams, but doesn't happen on respawns
============
*/
extern qboolean	gSiegeRoundBegun;
extern qboolean	gSiegeRoundEnded;
void SetTeamQuick(gentity_t *ent, int team, qboolean doBegin);
void ClientBegin( int clientNum, qboolean allowTeamReset ) {
	gentity_t	*ent;
	gclient_t	*client;
	gentity_t	*tent;
	int			flags, i;
	//[BugFix48]
	int			spawnCount;
	//[/BugFix48]
	char		userinfo[MAX_INFO_VALUE], *modelname;

	//[ExpandedMOTD]
	//contains the message of the day that is sent to new players.
	char		motd[1024];  

	ent = g_entities + clientNum;

	if ((ent->r.svFlags & SVF_BOT) && g_gametype.integer >= GT_TEAM)
	{
		if (allowTeamReset)
		{
			const char *team = "Red";
			int preSess;

			//SetTeam(ent, "");
			//[AdminSys]
			ent->client->sess.sessionTeam = PickTeam(-1, qtrue);
			//ent->client->sess.sessionTeam = PickTeam(-1);
			//[/AdminSys]
			trap_GetUserinfo(clientNum, userinfo, MAX_INFO_STRING);

			if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
			{
				ent->client->sess.sessionTeam = TEAM_RED;
			}

			if (ent->client->sess.sessionTeam == TEAM_RED)
			{
				team = "Red";
			}
			else
			{
				team = "Blue";
			}

			Info_SetValueForKey( userinfo, "team", team );

			trap_SetUserinfo( clientNum, userinfo );

			ent->client->ps.persistant[ PERS_TEAM ] = ent->client->sess.sessionTeam;

			preSess = ent->client->sess.sessionTeam;
			G_ReadSessionData( ent->client );
			ent->client->sess.sessionTeam = preSess;
			G_WriteClientSessionData(ent->client);
			ClientUserinfoChanged( clientNum );
			ClientBegin(clientNum, qfalse);
			return;
		}
	}

	client = level.clients + clientNum;

	if ( ent->r.linked ) {
		trap_UnlinkEntity( ent );
	}
	G_InitGentity( ent );
	ent->touch = 0;
	ent->pain = 0;
	ent->client = client;

	//assign the pointer for bg entity access
	ent->playerState = &ent->client->ps;

	client->pers.connected = CON_CONNECTED;
	client->pers.enterTime = level.time;
	client->pers.teamState.state = TEAM_BEGIN;

	// save eflags around this, because changing teams will
	// cause this to happen with a valid entity, and we
	// want to make sure the teleport bit is set right
	// so the viewpoint doesn't interpolate through the
	// world to the new position
	flags = client->ps.eFlags;
	//[BugFix48]
	spawnCount = client->ps.persistant[PERS_SPAWN_COUNT];
	//[/BugFix48]

	i = 0;

	while (i < NUM_FORCE_POWERS)
	{
		if (ent->client->ps.fd.forcePowersActive & (1 << i))
		{
			WP_ForcePowerStop(ent, i);
		}
		i++;
	}

	i = TRACK_CHANNEL_1;

	while (i < NUM_TRACK_CHANNELS)
	{
		if (ent->client->ps.fd.killSoundEntIndex[i-50] && ent->client->ps.fd.killSoundEntIndex[i-50] < MAX_GENTITIES && ent->client->ps.fd.killSoundEntIndex[i-50] > 0)
		{
			G_MuteSound(ent->client->ps.fd.killSoundEntIndex[i-50], CHAN_VOICE);
		}
		i++;
	}
	i = 0;

	memset( &client->ps, 0, sizeof( client->ps ) );
	client->ps.eFlags = flags;

	//[BugFix48]
	client->ps.persistant[PERS_SPAWN_COUNT] = spawnCount;
	//[/BugFix48]

	client->ps.hasDetPackPlanted = qfalse;

	//first-time force power initialization
	WP_InitForcePowers( ent );

	//init saber ent
	WP_SaberInitBladeData( ent );

	// First time model setup for that player.
	trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
	modelname = Info_ValueForKey (userinfo, "model");
	SetupGameGhoul2Model(ent, modelname, NULL);

	if (ent->ghoul2 && ent->client)
	{
		ent->client->renderInfo.lastG2 = NULL; //update the renderinfo bolts next update.
	}

	if (g_gametype.integer == GT_POWERDUEL && client->sess.sessionTeam != TEAM_SPECTATOR &&
		client->sess.duelTeam == DUELTEAM_FREE)
	{
		SetTeam(ent, "s");
	}
	else
	{
		if (g_gametype.integer == GT_SIEGE && (!gSiegeRoundBegun || gSiegeRoundEnded))
		{
			SetTeamQuick(ent, TEAM_SPECTATOR, qfalse);
		}
        
		if ((ent->r.svFlags & SVF_BOT) &&
			g_gametype.integer != GT_SIEGE)
		{
			char *saberVal = Info_ValueForKey(userinfo, "saber1");
			char *saber2Val = Info_ValueForKey(userinfo, "saber2");

			if (!saberVal || !saberVal[0])
			{ //blah, set em up with a random saber
				int r = rand()%50;
				char sab1[1024];
				char sab2[1024];

				if (r <= 17)
				{
					strcpy(sab1, "Katarn");
					strcpy(sab2, "none");
				}
				else if (r <= 34)
				{
					strcpy(sab1, "Katarn");
					strcpy(sab2, "Katarn");
				}
				else
				{
					strcpy(sab1, "dual_1");
					strcpy(sab2, "none");
				}
				G_SetSaber(ent, 0, sab1, qfalse);
				G_SetSaber(ent, 0, sab2, qfalse);
				Info_SetValueForKey( userinfo, "saber1", sab1 );
				Info_SetValueForKey( userinfo, "saber2", sab2 );
				trap_SetUserinfo( clientNum, userinfo );
			}
			else
			{
				G_SetSaber(ent, 0, saberVal, qfalse);
			}

			if (saberVal && saberVal[0] &&
				(!saber2Val || !saber2Val[0]))
			{
				G_SetSaber(ent, 0, "none", qfalse);
				Info_SetValueForKey( userinfo, "saber2", "none" );
				trap_SetUserinfo( clientNum, userinfo );
			}
			else
			{
				G_SetSaber(ent, 0, saber2Val, qfalse);
			}
		}

		// locate ent at a spawn point
		//[LastManStanding]
		if (ojp_lms.integer > 0 && ent->lives < 1 && BG_IsLMSGametype(g_gametype.integer) && LMS_EnoughPlayers() && client->sess.sessionTeam != TEAM_SPECTATOR)
		{//don't allow players to respawn in LMS by switching teams.
			OJP_Spectator(ent);
		}
		else
		{
			ClientSpawn(ent);
			if(client->sess.sessionTeam != TEAM_SPECTATOR && LMS_EnoughPlayers())
			{//costs a life to switch teams to something other than spectator.
				ent->lives--;
			}
		}
		//[/LastManStanding]
	}

	if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
		// send event
		//[LastManStanding]
		if (ojp_lms.integer <= 0 || ent->lives >= 1 || !BG_IsLMSGametype(g_gametype.integer))
		{//don't do the "teleport in" effect if we're playing LMS and we're "out"
			tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
			tent->s.clientNum = ent->s.clientNum;
		}
		//tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
		//tent->s.clientNum = ent->s.clientNum;
		//[/LastManStanding]

		if ( g_gametype.integer != GT_DUEL || g_gametype.integer == GT_POWERDUEL ) {
			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStringEdString("MP_SVGAME", "PLENTER")) );
		}
	}
	G_LogPrintf( "ClientBegin: %i\n", clientNum );

	//[ExpandedMOTD]
	//prepare and send MOTD message to new client.
	if(client->pers.ojpClientPlugIn)
	{//send this client the MOTD for clients using the right version of OJP.
		TextWrapCenterPrint(ojp_clientMOTD.string, motd);
	}
	else
	{//send this client the MOTD for clients aren't running OJP or just not the right version.
		TextWrapCenterPrint(ojp_MOTD.string, motd);
	}

	trap_SendServerCommand( clientNum, va("cp \"%s\n\"", motd ) );
	//[/ExpandedMOTD]

	// count current clients and rank for scoreboard
	CalculateRanks();

	G_ClearClientLog(clientNum);
}

qboolean AllForceDisabled() {
	int force = g_forcePowerDisable.integer;
	if (force) {
		int i;
		for (i = 0; i < NUM_FORCE_POWERS; i++) {
			if (!(force & (1 << i ))) {
				return qfalse;
			}
		}

		return qtrue;
	}

	return qfalse;
}

//Convenient interface to set all my limb breakage stuff up -rww
void G_BreakArm(gentity_t *ent, int arm)
{
	int anim = -1;

	assert(ent && ent->client);

	if (ent->s.NPC_class == CLASS_VEHICLE || ent->localAnimIndex > 1)
	{ //no broken limbs for vehicles and non-humanoids
		return;
	}

	if (!arm)
	{ //repair him
		ent->client->ps.brokenLimbs = 0;
		return;
	}

	if (ent->client->ps.fd.saberAnimLevel == SS_STAFF)
	{ //I'm too lazy to deal with this as well for now.
		return;
	}

	if (arm == BROKENLIMB_LARM)
	{
		if (ent->client->saber[1].model[0] &&
			ent->client->ps.weapon == WP_SABER &&
			!ent->client->ps.saberHolstered &&
			ent->client->saber[1].soundOff)
		{ //the left arm shuts off its saber upon being broken
			G_Sound(ent, CHAN_AUTO, ent->client->saber[1].soundOff);
		}
	}

	ent->client->ps.brokenLimbs = 0; //make sure it's cleared out
	ent->client->ps.brokenLimbs |= (1 << arm); //this arm is now marked as broken

	//Do a pain anim based on the side. Since getting your arm broken does tend to hurt.
	if (arm == BROKENLIMB_LARM)
	{
		anim = BOTH_PAIN2;
	}
	else if (arm == BROKENLIMB_RARM)
	{
		anim = BOTH_PAIN3;
	}

	if (anim == -1)
	{
		return;
	}

	G_SetAnim(ent, &ent->client->pers.cmd, SETANIM_BOTH, anim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, 0);

	//This could be combined into a single event. But I guess limbs don't break often enough to
	//worry about it.
	G_EntitySound( ent, CHAN_VOICE, G_SoundIndex("*pain25.wav") );
	//FIXME: A nice bone snapping sound instead if possible
	G_Sound(ent, CHAN_AUTO, G_SoundIndex( va("sound/player/bodyfall_human%i.wav", Q_irand(1, 3)) ));
}

//Update the ghoul2 instance anims based on the playerstate values

qboolean BG_SaberStanceAnim( int anim );
qboolean PM_RunningAnim( int anim );

//[SaberLockSys]
extern stringID_table_t animTable [MAX_ANIMATIONS+1];
//[/SaberLockSys]
void G_UpdateClientAnims(gentity_t *self, float animSpeedScale)
{
	static int f;
	static int torsoAnim;
	static int legsAnim;
	static int firstFrame, lastFrame;
	static int aFlags;
	static float animSpeed, lAnimSpeedScale;
	qboolean setTorso = qfalse;

	torsoAnim = (self->client->ps.torsoAnim);
	legsAnim = (self->client->ps.legsAnim);

	if (self->client->ps.saberLockFrame)
	{
		trap_G2API_SetBoneAnim(self->ghoul2, 0, "model_root", self->client->ps.saberLockFrame, self->client->ps.saberLockFrame+1, BONE_ANIM_OVERRIDE_FREEZE|BONE_ANIM_BLEND, animSpeedScale, level.time, -1, 150);
		trap_G2API_SetBoneAnim(self->ghoul2, 0, "lower_lumbar", self->client->ps.saberLockFrame, self->client->ps.saberLockFrame+1, BONE_ANIM_OVERRIDE_FREEZE|BONE_ANIM_BLEND, animSpeedScale, level.time, -1, 150);
		trap_G2API_SetBoneAnim(self->ghoul2, 0, "Motion", self->client->ps.saberLockFrame, self->client->ps.saberLockFrame+1, BONE_ANIM_OVERRIDE_FREEZE|BONE_ANIM_BLEND, animSpeedScale, level.time, -1, 150);
		return;
	}

	if (self->localAnimIndex > 1 &&
		bgAllAnims[self->localAnimIndex].anims[legsAnim].firstFrame == 0 &&
		bgAllAnims[self->localAnimIndex].anims[legsAnim].numFrames == 0)
	{ //We'll allow this for non-humanoids.
		goto tryTorso;
	}

	if (self->client->legsAnimExecute != legsAnim || self->client->legsLastFlip != self->client->ps.legsFlip)
	{
		animSpeed = 50.0f / bgAllAnims[self->localAnimIndex].anims[legsAnim].frameLerp;
		lAnimSpeedScale = (animSpeed *= animSpeedScale);

		if (bgAllAnims[self->localAnimIndex].anims[legsAnim].loopFrames != -1)
		{
			aFlags = BONE_ANIM_OVERRIDE_LOOP;
		}
		else
		{
			aFlags = BONE_ANIM_OVERRIDE_FREEZE;
		}

		if (animSpeed < 0)
		{
			lastFrame = bgAllAnims[self->localAnimIndex].anims[legsAnim].firstFrame;
			firstFrame = bgAllAnims[self->localAnimIndex].anims[legsAnim].firstFrame + bgAllAnims[self->localAnimIndex].anims[legsAnim].numFrames;
		}
		else
		{
			firstFrame = bgAllAnims[self->localAnimIndex].anims[legsAnim].firstFrame;
			lastFrame = bgAllAnims[self->localAnimIndex].anims[legsAnim].firstFrame + bgAllAnims[self->localAnimIndex].anims[legsAnim].numFrames;
		}

		aFlags |= BONE_ANIM_BLEND; //since client defaults to blend. Not sure if this will make much difference if any on server position, but it's here just for the sake of matching them.

		//[NewGLA]
		//G2API_SetAnimIndex(self->ghoul2, bgAllAnims[self->localAnimIndex].anims[legsAnim].glaIndex);
		//[/NewGLA]
		trap_G2API_SetBoneAnim(self->ghoul2, 0, "model_root", firstFrame, lastFrame, aFlags, lAnimSpeedScale, level.time, -1, 150);
		self->client->legsAnimExecute = legsAnim;
		self->client->legsLastFlip = self->client->ps.legsFlip;
	}

tryTorso:
	if (self->localAnimIndex > 1 &&
		bgAllAnims[self->localAnimIndex].anims[torsoAnim].firstFrame == 0 &&
		bgAllAnims[self->localAnimIndex].anims[torsoAnim].numFrames == 0)

	{ //If this fails as well just return.
		return;
	}
	else if (self->s.number >= MAX_CLIENTS &&
		self->s.NPC_class == CLASS_VEHICLE)
	{ //we only want to set the root bone for vehicles
		return;
	}

	if ((self->client->torsoAnimExecute != torsoAnim || self->client->torsoLastFlip != self->client->ps.torsoFlip) &&
		!self->noLumbar)
	{
		aFlags = 0;
		animSpeed = 0;

		f = torsoAnim;

		//[FatigueSys]
		BG_SaberStartTransAnim(self->s.number, self->client->ps.fd.saberAnimLevel, self->client->ps.weapon, f, &animSpeedScale, self->client->ps.brokenLimbs, self->client->ps.userInt3);
		//BG_SaberStartTransAnim(self->s.number, self->client->ps.fd.saberAnimLevel, self->client->ps.weapon, f, &animSpeedScale, self->client->ps.brokenLimbs);
		//[/FatigueSys]

		animSpeed = 50.0f / bgAllAnims[self->localAnimIndex].anims[f].frameLerp;
		lAnimSpeedScale = (animSpeed *= animSpeedScale);

		if (bgAllAnims[self->localAnimIndex].anims[f].loopFrames != -1)
		{
			aFlags = BONE_ANIM_OVERRIDE_LOOP;
		}
		else
		{
			aFlags = BONE_ANIM_OVERRIDE_FREEZE;
		}

		aFlags |= BONE_ANIM_BLEND; //since client defaults to blend. Not sure if this will make much difference if any on client position, but it's here just for the sake of matching them.

		if (animSpeed < 0)
		{
			lastFrame = bgAllAnims[self->localAnimIndex].anims[f].firstFrame;
			firstFrame = bgAllAnims[self->localAnimIndex].anims[f].firstFrame + bgAllAnims[self->localAnimIndex].anims[f].numFrames;
		}
		else
		{
			firstFrame = bgAllAnims[self->localAnimIndex].anims[f].firstFrame;
			lastFrame = bgAllAnims[self->localAnimIndex].anims[f].firstFrame + bgAllAnims[self->localAnimIndex].anims[f].numFrames;
		}

		trap_G2API_SetBoneAnim(self->ghoul2, 0, "lower_lumbar", firstFrame, lastFrame, aFlags, lAnimSpeedScale, level.time, /*firstFrame why was it this before?*/-1, 150);

		self->client->torsoAnimExecute = torsoAnim;
		self->client->torsoLastFlip = self->client->ps.torsoFlip;
		
		setTorso = qtrue;
		
		//[SaberLockSys]
		//G_Printf("%i: %i: Server Started Torso Animation %s\n", level.time, self->s.number, GetStringForID(animTable, torsoAnim) );
		//[/SaberLockSys]
	}

	if (setTorso &&
		self->localAnimIndex <= 1)
	{ //only set the motion bone for humanoids.
		trap_G2API_SetBoneAnim(self->ghoul2, 0, "Motion", firstFrame, lastFrame, aFlags, lAnimSpeedScale, level.time, -1, 150);
	}
}


//[ExpSys]
int TotalAllociatedSkillPoints(gentity_t *ent)
{//returns the total number of points the player has allociated to various skills.
	int i, countDown;
	int usedPoints = 0;
	qboolean freeSaber = HasSetSaberOnly();

	for(i = 0; i < NUM_TOTAL_SKILLS; i++)
	{
		if(i < NUM_FORCE_POWERS)
		{//force power
			countDown = ent->client->ps.fd.forcePowerLevel[i];
		}
		else
		{
			countDown = ent->client->skillLevel[i-NUM_FORCE_POWERS];
		}

		while (countDown > 0)
		{
			usedPoints += bgForcePowerCost[i][countDown];
			if ( countDown == 1 &&
				((i == FP_SABER_OFFENSE && freeSaber) ||
				 (i == FP_SABER_DEFENSE && freeSaber)) )
			{
				usedPoints -= bgForcePowerCost[i][countDown];
			}
			countDown--;
		}
	}

	return usedPoints;
}
//[/ExpSys]

#define LEVEL_1_LIGHTSIDE 6
#define LEVEL_1_DARKSIDE 10
#define LEVEL_1_LIGHTSABER 8
#define LEVEL_1_NEUTRAL 10
#define LEVEL_2_LIGHTSIDE 20
#define LEVEL_2_DARKSIDE 29
#define LEVEL_2_LIGHTSABER 24
#define LEVEL_2_NEUTRAL 30

int GetTotalCostForSkill(int skill) {
	return bgForcePowerCost[skill][1] + bgForcePowerCost[skill][2] + bgForcePowerCost[skill][3];
}

int GetPointsSpentInSkill(gclient_t *client,int skill) {
	if(client->skillLevel[skill] == FORCE_LEVEL_1) {
		return bgForcePowerCost[skill][1];
	}
	else if(client->skillLevel[skill] == FORCE_LEVEL_2) {
		return bgForcePowerCost[skill][1] + bgForcePowerCost[skill][2];
	}
	else if(client->skillLevel[skill] == FORCE_LEVEL_3) {
		return bgForcePowerCost[skill][1] + bgForcePowerCost[skill][2] + bgForcePowerCost[skill][3];
	}

	return 0;
}

int SkillSide(int forcePower)
{
	switch(forcePower)
	{
		case FP_GRIP:
		case FP_LIGHTNING:
		case FP_RAGE:
		case FP_MANIPULATE:
			return FORCE_DARKSIDE;
		case FP_LEVITATION:
		case FP_SPEED:
		case FP_PUSH:
		case FP_PULL:
		case FP_SEE:
			return FORCE_NEUTRAL;
		case FP_ABSORB:
		case FP_MINDTRICK:
		case FP_LIFT:
			return FORCE_LIGHTSIDE;
		case NUM_FORCE_POWERS+SK_BLUESTYLE:
		case NUM_FORCE_POWERS+SK_REDSTYLE:
		case NUM_FORCE_POWERS+SK_PURPLESTYLE:
		case NUM_FORCE_POWERS+SK_GREENSTYLE:
		case NUM_FORCE_POWERS+SK_DUALSTYLE:
		case NUM_FORCE_POWERS+SK_STAFFSTYLE:
			return FORCE_LIGHTSABER;
	}
	return 0;
}

int Client_GetPointsInDarkSide(gclient_t *client) {
	return 
		GetPointsSpentInSkill(client, FP_GRIP) +
		GetPointsSpentInSkill(client, FP_LIGHTNING) +
		GetPointsSpentInSkill(client, FP_RAGE) +
		GetPointsSpentInSkill(client, FP_MANIPULATE);
}

int Client_GetPointsInNeutral(gclient_t *client) {
	return 
		GetPointsSpentInSkill(client, FP_LEVITATION) +
		GetPointsSpentInSkill(client, FP_SPEED) +
		GetPointsSpentInSkill(client, FP_PUSH) +
		GetPointsSpentInSkill(client, FP_PULL) +
		GetPointsSpentInSkill(client, FP_SEE);
}

int Client_GetPointsInLightSide(gclient_t *client) {
	return 
		GetPointsSpentInSkill(client, FP_ABSORB) +
		GetPointsSpentInSkill(client, FP_MINDTRICK) +
		GetPointsSpentInSkill(client, FP_LIFT);
}

int Client_GetPointsInLightsaber(gclient_t *client) {
	return
		GetPointsSpentInSkill(client, NUM_FORCE_POWERS+SK_BLUESTYLE) +
		GetPointsSpentInSkill(client, NUM_FORCE_POWERS+SK_REDSTYLE) +
		GetPointsSpentInSkill(client, NUM_FORCE_POWERS+SK_PURPLESTYLE) +
		GetPointsSpentInSkill(client, NUM_FORCE_POWERS+SK_GREENSTYLE) +
		GetPointsSpentInSkill(client, NUM_FORCE_POWERS+SK_DUALSTYLE) +
		GetPointsSpentInSkill(client, NUM_FORCE_POWERS+SK_STAFFSTYLE);
}

int GetPointsSpentInInfantry(gclient_t *client) {
	return 
		GetPointsSpentInSkill(client, SK_BLASTER) + 
		GetPointsSpentInSkill(client, SK_PISTOL) + 
		GetPointsSpentInSkill(client, SK_GRENADE) +
		GetPointsSpentInSkill(client, SK_DISRUPTOR);
}

int GetPointsSpentInTech(gclient_t *client) {
	return 
		GetPointsSpentInSkill(client, SK_SHIELD) + 
		GetPointsSpentInSkill(client, SK_SENTRY) + 
		GetPointsSpentInSkill(client, SK_FORCEFIELD) + 
		GetPointsSpentInSkill(client, SK_BACTA) + 
		GetPointsSpentInSkill(client, SK_CLOAK);
}

int GetPointsSpentInBounty(gclient_t *client) {
	return 
		GetPointsSpentInSkill(client, SK_JETPACK) + 
		GetPointsSpentInSkill(client, SK_SEEKER) + 
		GetPointsSpentInSkill(client, SK_FLAMETHROWER) + 
		GetPointsSpentInSkill(client, SK_THERMAL);
}

int GetPointsSpentInAssault(gclient_t *client) {
	return 
		GetPointsSpentInSkill(client, SK_BOWCASTER) + 
		GetPointsSpentInSkill(client, SK_REPEATER) + 
		GetPointsSpentInSkill(client, SK_DETPACK) + 
		GetPointsSpentInSkill(client, SK_ROCKET);
}

OJPRank GetLevelJedi3Rank(gclient_t *client) {
	OJPRank rank = client->playerRank;
	int totalSpent = 
		Client_GetPointsInNeutral(client) +
		Client_GetPointsInLightSide(client) +
		Client_GetPointsInDarkSide(client) +
		Client_GetPointsInLightsaber(client);

	if(totalSpent >= 100) {
		rank = RANK_JEDI_MASTER;
	}

	return rank;
}

OJPRank GetLevelJedi2Rank(gclient_t *client) {
	OJPRank rank = client->playerRank;
	int neutral = Client_GetPointsInNeutral(client);
	int light = Client_GetPointsInLightSide(client);
	int dark = Client_GetPointsInDarkSide(client);
	int saber = Client_GetPointsInLightsaber(client);

	if(dark >= LEVEL_2_DARKSIDE && saber >= LEVEL_1_LIGHTSABER && saber < LEVEL_2_LIGHTSABER) {	
		if(dark > saber) {
			rank = RANK_SITH_LIEUTENANT;
		}
		else {
			rank = RANK_SITH_KNIGHT;
		}
	}

	if(dark >= LEVEL_2_DARKSIDE && light >= LEVEL_1_LIGHTSIDE && light <= LEVEL_2_LIGHTSIDE) {
		if(dark > light) {
			rank = RANK_EX_LUMINI;
		}
		else {
			rank = RANK_SCELEQUILIBRI;
		}
	}


	if(dark >= LEVEL_2_DARKSIDE && neutral >= LEVEL_1_NEUTRAL && neutral < LEVEL_2_NEUTRAL) {	
		if(dark > neutral) {
			rank = RANK_DARK_INFLUENCE;
		}
		else {
			rank = RANK_SITH_MANIPULATOR;
		}
	}

	if(dark >= LEVEL_2_DARKSIDE && saber >= LEVEL_2_LIGHTSABER) {	
		if(dark > saber) {
			rank = RANK_SITH_COMMANDER;
		}
		else {
			rank = RANK_SITH_CAPTAIN;
		}
	}


	if(dark >= LEVEL_2_DARKSIDE && light >= LEVEL_2_LIGHTSIDE) {	
		if(dark > light) {
			rank = RANK_FALLEN_JEDI;
		}
		else {
			rank = RANK_GREY_MASTER;
		}
	}

	if(dark >= LEVEL_2_DARKSIDE && neutral >= LEVEL_2_NEUTRAL) {	
		if(dark > neutral) {
			rank = RANK_SITH_HERO;
		}
		else {
			rank = RANK_BALANCE_MANIPULATOR;
		}
	}

	if(saber >= LEVEL_2_LIGHTSABER && light >= LEVEL_1_LIGHTSIDE && light < LEVEL_2_LIGHTSIDE) {	
		if(saber > light) {
			rank = RANK_JEDI_WARRIOR;
		}
		else {
			rank = RANK_JEDI_INSTRUCTOR;
		}
	}

	if(saber >= LEVEL_2_LIGHTSABER && neutral >= LEVEL_1_NEUTRAL && neutral < LEVEL_2_NEUTRAL) {	
		if(saber > neutral) {
			rank = RANK_SABER_VETERAN;
		}
		else {
			rank = RANK_JEDI_HERO;
		}
	}

	if(saber >= LEVEL_2_LIGHTSABER && dark >= LEVEL_1_DARKSIDE && dark < LEVEL_2_DARKSIDE) {	
		if(saber > dark) {
			rank = RANK_CRUEL_SABERIST;
		}
		else {
			rank = RANK_SITH_SABERIST;
		}
	}

	if(saber >= LEVEL_2_LIGHTSABER && light >= LEVEL_2_LIGHTSIDE) {
		if(saber > light) {
			rank = RANK_JEDI_SABERMASTER;
		}
		else {
			rank = RANK_SABER_OF_BALANCE;
		}
	}

	if(saber >= LEVEL_2_LIGHTSABER && neutral >= LEVEL_2_NEUTRAL) {
		if(saber > neutral) {
			rank = RANK_FORCE_PEACEKEEPER;
		}
		else {
			rank = RANK_FORCE_SOLDIER;
		}
	}

	if(saber >= LEVEL_2_LIGHTSABER && dark >= LEVEL_2_DARKSIDE && saber > dark) {
		if(neutral > saber) {
			rank = RANK_GREY_HAND;
		}
		else {
			rank = RANK_DARK_FLAYER;
		}
	}

	if(neutral >= LEVEL_2_NEUTRAL && light >= LEVEL_1_LIGHTSIDE && light < LEVEL_2_LIGHTSIDE) {
		if(neutral > light) {
			rank = RANK_GREY_TALON;
		}
		else {
			rank = RANK_JEDI_OF_BALANCE;
		}
	}

	if(neutral >= LEVEL_2_NEUTRAL && light >= LEVEL_2_LIGHTSIDE) {
		if(neutral > light) {
			rank = RANK_FORCE_WANDERER;
		}
		else {
			rank = RANK_LIGHT_JEDI;
		}
	}

	if(light >= LEVEL_2_LIGHTSIDE && dark == LEVEL_1_DARKSIDE) {
		if(light > dark) {
			rank = RANK_FORCE_USER;
		}
		else {
			rank = RANK_FORCE_DEBUTANTE;
		}
	}

	if(neutral >= LEVEL_2_NEUTRAL && saber >= LEVEL_2_LIGHTSABER && neutral > saber) {
		rank = RANK_ARBITER;
	}

	if(neutral >= LEVEL_2_NEUTRAL && dark == LEVEL_1_DARKSIDE && neutral > dark) {
		rank = RANK_STRAYING_WARRIOR;
	}

	if(neutral >= LEVEL_2_NEUTRAL && dark >= LEVEL_2_DARKSIDE && neutral > dark) {
		rank = RANK_RENEGADE;
	}

	if(light >= LEVEL_2_LIGHTSIDE && dark >= LEVEL_2_DARKSIDE && light > dark) {
		rank = RANK_FORCE_OVERSEER;
	}

	if(light >= LEVEL_2_LIGHTSIDE && saber == LEVEL_1_LIGHTSIDE && light > saber) {
		rank = RANK_ENLIGHTENED_JEDI;
	}

	if(light >= LEVEL_2_LIGHTSIDE && neutral >= LEVEL_2_NEUTRAL && light > neutral) {
		rank = RANK_TRIUMVIRATI;
	}

	return rank;
}


OJPRank GetLevelJedi1Rank(gclient_t *client) {
	OJPRank rank = client->playerRank;
	int neutral = Client_GetPointsInNeutral(client);
	int light = Client_GetPointsInLightSide(client);
	int dark = Client_GetPointsInDarkSide(client);
	int saber = Client_GetPointsInLightsaber(client);

	if(dark >= LEVEL_1_DARKSIDE && saber >= LEVEL_1_LIGHTSABER) {
		if(dark > saber) {
			rank = RANK_DARK_TEMPLAR;
		}
		else {
			rank = RANK_DARK_KNIGHT;
		}
	}

	if(dark >= LEVEL_1_DARKSIDE && light >= LEVEL_1_LIGHTSIDE) {
		if(dark > light) {
			rank = RANK_SHADOW_JEDI;
		}
		else {
			rank = RANK_GREY_JEDI;
		}
	}

	if(dark >= LEVEL_1_DARKSIDE && neutral >= LEVEL_1_NEUTRAL) {	
		if(dark > neutral) {
			rank = RANK_FORCE_MARAUDER;
		}
		else {
			rank = RANK_FORCE_MANIPULATOR;
		}
	}

	if(saber >= LEVEL_1_LIGHTSABER && neutral >= LEVEL_1_NEUTRAL) {	
		if(saber > neutral) {
			rank = RANK_JEDI_GUARDIAN;
		}
		else {
			rank = RANK_JEDI_KNIGHT;
		}
	}

	if(saber >= LEVEL_1_LIGHTSABER && light >= LEVEL_1_LIGHTSIDE) {	
		if(saber > light) {
			rank = RANK_JEDI_TEMPLAR;
		}
		else {
			rank = RANK_JEDI_CRUSADER;
		}
	}

	if(neutral >= LEVEL_1_NEUTRAL && light >= LEVEL_1_LIGHTSIDE) {	
		if(neutral > light) {
			rank = RANK_JEDI_CONSULAR;
		}
		else {
			rank = RANK_JEDI_HOSPITALIER;
		}
	}

	return rank;
}

OJPRank Client_CalculateJediRank(gclient_t *client) {
	OJPRank rank = RANK_JEDI_PADAWAN;

	int spentInNeutral = Client_GetPointsInNeutral(client);
	int spentInLight = Client_GetPointsInLightSide(client);
	int spentInDark = Client_GetPointsInDarkSide(client);
	int spentInSaber = Client_GetPointsInLightsaber(client);

	rank = GetLevelJedi3Rank(client);
	if(rank == RANK_JEDI_PADAWAN) {
		rank = GetLevelJedi2Rank(client);
	}

	if(rank == RANK_JEDI_PADAWAN) {
		rank = GetLevelJedi1Rank(client);
	}

	if(rank == RANK_JEDI_PADAWAN) {
		if(spentInDark >= LEVEL_2_DARKSIDE) {
			rank = RANK_SITH;
		}

		if(spentInLight >= LEVEL_2_LIGHTSIDE) {
			rank = RANK_JEDI_MEDICI;
		}

		if(spentInSaber >= LEVEL_2_LIGHTSABER) {
			rank = RANK_JEDI_DUELIST;
		}

		if(spentInNeutral >= LEVEL_2_NEUTRAL) {
			rank = RANK_JEDI_SENTINEL;
		}
	}

	return rank;
}

int GetLevel3InfantryCost() {
	return (GetTotalCostForSkill(SK_BLASTER) + GetTotalCostForSkill(SK_PISTOL) + GetTotalCostForSkill(SK_GRENADE) + GetTotalCostForSkill(SK_DISRUPTOR)) / 3;
}

int GetLevel3TechCost() {
	return (GetTotalCostForSkill(SK_SHIELD) + GetTotalCostForSkill(SK_FORCEFIELD) + GetTotalCostForSkill(SK_SENTRY) + GetTotalCostForSkill(SK_BACTA) + GetTotalCostForSkill(SK_CLOAK)) / 3;
}

int GetLevel3BountyCost() {
	return (GetTotalCostForSkill(SK_JETPACK) + GetTotalCostForSkill(SK_SEEKER) + GetTotalCostForSkill(SK_FLAMETHROWER) + GetTotalCostForSkill(SK_THERMAL)) / 3;
}

int GetLevel3AssaultCost() {
	return (GetTotalCostForSkill(SK_BOWCASTER) + GetTotalCostForSkill(SK_REPEATER) + GetTotalCostForSkill(SK_DETPACK) + GetTotalCostForSkill(SK_ROCKET)) / 3;
}

OJPRank GetLevel1GunnerRank(gclient_t *client) {
	OJPRank rank = RANK_NEUTRAL_CIVILIAN;
	const int infantry = GetPointsSpentInInfantry(client);
	const int tech = GetPointsSpentInTech(client);
	const int bounty = GetPointsSpentInBounty(client);
	const int assault = GetPointsSpentInAssault(client);
	const int Level3Infantry = GetLevel3InfantryCost();
	const int Level1Infantry = Level3Infantry / 3;
	const int Level3Tech = GetLevel3TechCost();
	const int Level1Tech = Level3Tech / 3;
	const int Level3Bounty = GetLevel3BountyCost();
	const int Level1Bounty = Level3Bounty / 3;
	const int Level3Assault = GetLevel3AssaultCost();
	const int Level1Assault = Level3Assault / 3;

	if(assault >= Level1Assault) {
		rank = RANK_ASSAULT_ENSIGN;
	}

	if(tech >= Level1Tech) {
		rank = RANK_MECHANIC;
	}

	if(infantry >= Level1Infantry) {
		rank = RANK_PRIVATE;
	}

	if(bounty >= Level1Bounty) {
		rank = RANK_OUTLAW;
	}

	if(assault >= Level1Assault && tech >= Level1Tech) {
		rank = RANK_MECHENSIGN;
	}

	if(assault >= Level1Assault && infantry >= Level1Infantry) {
		rank = RANK_DEMOPRIVATE;
	}

	if(assault >= Level1Assault && bounty >= Level1Bounty) {
		rank = RANK_ROUGHNECK;
	}

	if(assault >= Level1Assault && tech >= Level1Tech && assault > tech) {
		rank = RANK_MECHENSIGN2;
	}

	if(assault >= Level1Assault && infantry >= Level1Infantry && assault > infantry) {
		rank = RANK_SHOCKTROOP;
	}

	if(assault >= Level1Assault && bounty >= Level1Bounty && assault > bounty) {
		rank = RANK_MERCTRAINEE;
	}

	if(tech >= Level1Tech && infantry >= Level1Infantry) {
		rank = RANK_TECHPRIVATE;
	}

	if(tech >= Level1Tech && bounty >= Level1Bounty) {
		rank = RANK_TECHTRAINEE;
	}

	if(tech >= Level1Tech && infantry >= Level1Infantry && tech > infantry) {
		rank = RANK_FIXER;
	}

	if(tech >= Level1Tech && bounty >= Level1Bounty && tech > bounty) {
		rank = RANK_HACKER;
	}

	if(tech >= Level1Tech && assault >= Level1Assault && tech > assault) {
		rank = RANK_HEAVYTECH;
	}

	if(infantry >= Level1Infantry && bounty >= Level1Bounty) {
		rank = RANK_PRIVATEER;
	}

	if(infantry >= Level1Infantry && bounty >= Level1Bounty && infantry > bounty) {
		rank = RANK_ARMSDEALER;
	}

	if(bounty >= Level1Bounty && tech >= Level1Tech && bounty > tech) {
		rank = RANK_PRIVATETECH;
	}
	
	if(bounty >= Level1Bounty && infantry >= Level1Infantry && bounty > infantry) {
		rank = RANK_RENEGADECOMBATANT;
	}

	return rank;
}

OJPRank GetLevel2GunnerRank(gclient_t *client) {
	OJPRank rank = RANK_NEUTRAL_CIVILIAN;
	const int infantry = GetPointsSpentInInfantry(client);
	const int tech = GetPointsSpentInTech(client);
	const int bounty = GetPointsSpentInBounty(client);
	const int assault = GetPointsSpentInAssault(client);
	const int Level3Infantry = GetLevel3InfantryCost();
	const int Level2Infantry = Level3Infantry / 2;
	const int Level1Infantry = Level3Infantry / 3;
	const int Level3Tech = GetLevel3TechCost();
	const int Level2Tech = Level3Tech / 2;
	const int Level1Tech = Level3Tech / 3;
	const int Level3Bounty = GetLevel3BountyCost();
	const int Level2Bounty = Level3Bounty / 2;
	const int Level1Bounty = Level3Bounty / 3;
	const int Level3Assault = GetLevel3AssaultCost();
	const int Level2Assault = Level3Assault / 2;
	const int Level1Assault = Level3Assault / 3;

	if(assault >= Level2Assault && tech >= Level1Tech)
		rank = RANK_ARMOREDLT;

	if(assault >= Level2Assault && infantry >= Level1Infantry)
		rank = RANK_HEAVYGUNNER;

	if(assault >= Level2Assault && bounty >= Level1Bounty)
		rank = RANK_MERCENARY;

	if(assault >= Level2Assault && tech >= Level1Tech && assault > tech)
		rank = RANK_ASSAULTTECH;

	if(assault >= Level2Assault && infantry >= Level1Infantry && assault > infantry)
		rank = RANK_HEAVYTROOPER;

	if(assault >= Level2Assault && bounty >= Level1Bounty && assault > bounty)
		rank = RANK_HEAVYMERCENARY;

	if(assault >= Level2Assault && tech >= Level2Tech)
		rank = RANK_ASSAULTENGINEER;

	if(assault >= Level2Assault && infantry >= Level2Infantry)
		rank = RANK_WEAPONSPECALIST;

	if(assault >= Level2Assault && bounty >= Level2Bounty)
		rank = RANK_LIGHTMANDALORIN;

	if(tech >= Level2Tech && infantry >= Level1Infantry)
		rank = RANK_INFOSOLDIER;

	if(tech >= Level2Tech && bounty >= Level1Bounty)
		rank = RANK_INFOMERC;

	if(tech >= Level2Tech && infantry >= Level1Infantry && tech > infantry)
		rank = RANK_TECHLT;

	if(tech >= Level2Tech && bounty >= Level1Bounty && tech > bounty)
		rank = RANK_TECHSURVIVOR;

	if(tech >= Level2Tech && assault >= Level1Assault && tech > assault)
		rank = RANK_ARMEDTECH;

	if(tech >= Level2Tech && infantry >= Level2Infantry)
		rank = RANK_VANGUARD;

	if(tech >= Level2Tech && bounty >= Level2Bounty)
		rank = RANK_PRIVATEER;

	if(tech >= Level2Tech && infantry >= Level2Infantry && tech > infantry)
		rank = RANK_ADVVANGUARD;

	if(tech >= Level2Tech && bounty >= Level2Bounty && tech > bounty)
		rank = RANK_SMUGGLER;

	if(tech >= Level2Tech && assault >= Level2Assault && tech > assault)
		rank = RANK_ADVASSAULTTECH;

	if(infantry >= Level2Infantry && assault >= Level1Assault && infantry > assault)
		rank = RANK_DEMOSOLDIER;

	if(infantry >= Level2Infantry && tech >= Level1Tech && infantry > tech)
		rank = RANK_ARCTRAINEE;

	if(infantry >= Level2Infantry && bounty >= Level1Bounty)
		rank = RANK_MERCLEADER;

	if(infantry >= Level2Infantry && bounty >= Level1Bounty && infantry > bounty)
		rank = RANK_MERCOFFICER;

	if(infantry >= Level2Infantry && assault >= Level2Assault && infantry > assault)
		rank = RANK_ASSAULTOFFICER;

	if(infantry >= Level2Infantry && tech >= Level2Tech && infantry > tech)
		rank = RANK_ARCTROOPER;

	if(bounty >= Level2Bounty && infantry >= Level1Infantry && bounty > infantry)
		rank = RANK_RENEGADETROOP;

	if(bounty >= Level2Bounty && infantry >= Level2Infantry && bounty > infantry)
		rank = RANK_RENEGADEOFFICER;

	if(bounty >= Level2Bounty && tech >= Level1Tech && bounty > tech)
		rank = RANK_MERCTECH;

	if(bounty >= Level2Bounty && tech >= Level2Tech && bounty > tech)
		rank = RANK_ELECTRONICSOUTLAW;

	if(bounty >= Level1Bounty && assault >= Level1Assault && bounty > assault)
		rank = RANK_SPACEMILITIA;

	if(bounty >= Level2Bounty && assault >= Level1Assault && bounty > assault)
		rank = RANK_OUTLAWJUGGERNAUT;

	if(bounty >= Level2Bounty && assault >= Level2Assault && bounty > assault)
		rank = RANK_OUTLAWDREADNAUGHT;

	return rank;
}

OJPRank GetLevel3GunnerRank(gclient_t *client) {
	OJPRank rank = RANK_NEUTRAL_CIVILIAN;

	const int Level3Infantry = GetLevel3InfantryCost();
	const int Level3Tech = GetLevel3TechCost();
	const int Level3Bounty = GetLevel3BountyCost();
	const int Level3Assault = GetLevel3AssaultCost();
	const int infantry = GetPointsSpentInInfantry(client);
	const int tech = GetPointsSpentInTech(client);
	const int bounty = GetPointsSpentInBounty(client);
	const int assault = GetPointsSpentInAssault(client);

	if(assault >= Level3Assault) {
		rank = RANK_ASSAULTGENERAL;
	}

	if(tech >= Level3Tech) {
		rank = RANK_ENGINEERGENERAL;
	}

	if(infantry >= Level3Infantry) {
		rank = RANK_INFCOMMANDER;
	}

	if(bounty >= Level3Bounty) {
		rank = RANK_BOUNTYHUNTER;
	}

	return rank;
}

OJPRank Client_CalculateGunnerRank(gclient_t *client) {
	OJPRank rank = GetLevel3GunnerRank(client);

	if(rank == RANK_NEUTRAL_CIVILIAN) {
		rank = GetLevel2GunnerRank(client);
	}

	if(rank == RANK_NEUTRAL_CIVILIAN) {
		rank = GetLevel1GunnerRank(client);
	}

	return rank;
}

void CalculateAndSetRank(gentity_t *ent) {
	if(ent->client->ps.fd.forcePowerLevel[FP_SEE]) {
		ent->client->playerRank = Client_CalculateJediRank(ent->client);
	}
	else {
		ent->client->playerRank = Client_CalculateGunnerRank(ent->client);
	}
}

int Client_CalculateStartMaxHealth(gclient_t *client) {
	int	maxHealth = 100;

	if (g_gametype.integer == GT_SIEGE && client->siegeClass != -1) {
		siegeClass_t *scl = &bgSiegeClasses[client->siegeClass];

		if (scl->maxhealth) {
			maxHealth = scl->maxhealth;
		}

		if ( maxHealth < 1 ) {
			maxHealth = 100;
		}
	}

	return maxHealth;
}

int Client_CalculateStartArmor(gclient_t *client) {
	int armor = 0;

	if (g_gametype.integer == GT_SIEGE && client->siegeClass != -1) {
		armor = bgSiegeClasses[client->siegeClass].startarmor;
	}
	else {
		if(client->skillLevel[SK_SHIELD] == FORCE_LEVEL_3) {
			armor = 100;
		}
		else if(client->skillLevel[SK_SHIELD] == FORCE_LEVEL_2) {
			armor = 75;
		}
		else if(client->skillLevel[SK_SHIELD] == FORCE_LEVEL_1) {
			armor = 50;
		}
		else {
			armor = 0;
		}
	}

	return armor;
}

gentity_t *Client_FindSpawnPoint(gentity_t *ent, vec_t *spawn_origin, vec_t *spawn_angles) {
	gentity_t *spawnPoint;
	gclient_t *client = ent->client;

	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
		spawnPoint = SelectSpectatorSpawnPoint ( 
						spawn_origin, spawn_angles);
	} else if (g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY) {
		// all base oriented team games use the CTF spawn points
		spawnPoint = SelectCTFSpawnPoint ( 
						client->sess.sessionTeam, 
						client->pers.teamState.state, 
						spawn_origin, spawn_angles);
	}
	else if (g_gametype.integer == GT_SIEGE)
	{
		spawnPoint = SelectSiegeSpawnPoint (
						client->siegeClass,
						client->sess.sessionTeam, 
						client->pers.teamState.state, 
						spawn_origin, spawn_angles);
	}
	//[CoOp]
	else if (g_gametype.integer == GT_SINGLE_PLAYER)
	{
		spawnPoint = SelectSPSpawnPoint(spawn_origin, spawn_angles);
	}
	//[/CoOp]
	else {
		do {
			if (g_gametype.integer == GT_POWERDUEL)
			{
				spawnPoint = SelectDuelSpawnPoint(client->sess.duelTeam, client->ps.origin, spawn_origin, spawn_angles);
			}
			else if (g_gametype.integer == GT_DUEL)
			{	// duel 
				spawnPoint = SelectDuelSpawnPoint(DUELTEAM_SINGLE, client->ps.origin, spawn_origin, spawn_angles);
			}
			else
			{
				// the first spawn should be at a good looking spot
				if ( !client->pers.initialSpawn && client->pers.localClient ) {
					client->pers.initialSpawn = qtrue;
					spawnPoint = SelectInitialSpawnPoint( spawn_origin, spawn_angles, client->sess.sessionTeam );
				} else {
					// don't spawn near existing origin if possible
					spawnPoint = SelectSpawnPoint ( 
						client->ps.origin, 
						spawn_origin, spawn_angles, client->sess.sessionTeam );
				}
			}

			// Tim needs to prevent bots from spawning at the initial point
			// on q3dm0...
			if ( ( spawnPoint->flags & FL_NO_BOTS ) && ( ent->r.svFlags & SVF_BOT ) ) {
				continue;	// try again
			}
			// just to be symetric, we have a nohumans option...
			if ( ( spawnPoint->flags & FL_NO_HUMANS ) && !( ent->r.svFlags & SVF_BOT ) ) {
				continue;	// try again
			}

			break;

		} while ( 1 );
	}

	return spawnPoint;
}

int GetWeaponsDisableSetting() { 
	if (g_gametype.integer == GT_DUEL || g_gametype.integer == GT_POWERDUEL) {
		return g_duelWeaponDisable.integer;
	}

	return g_weaponDisable.integer;
}

qboolean IsWeaponEnabled(int weapon) {
	int disable = GetWeaponsDisableSetting();

	return (qboolean)(!disable || !(disable & (1 << weapon)));
}

qboolean AreAllWeaponsEnabled() {
	int i;
	for(i = 0; i < WP_NUM_WEAPONS; i++) {
		if(!IsWeaponEnabled(i)) {
			return qfalse;
		}
	}

	return qtrue;
}

void Client_LoadAmmoLevels(gclient_t *client) {
	client->ps.ammo[AMMO_POWERCELL] = ammoData[AMMO_POWERCELL].max * (float) (client->skillLevel[SK_BOWCASTER] < client->skillLevel[SK_DISRUPTOR] ? client->skillLevel[SK_DISRUPTOR] : client->skillLevel[SK_BOWCASTER])/FORCE_LEVEL_3;

	client->ps.ammo[AMMO_METAL_BOLTS] = ammoData[AMMO_METAL_BOLTS].max * (float) client->skillLevel[SK_REPEATER]/FORCE_LEVEL_3;

	client->ps.ammo[AMMO_BLASTER] = ammoData[AMMO_BLASTER].max * (float) client->skillLevel[SK_BLASTER]/FORCE_LEVEL_3;


	client->ps.ammo[AMMO_ROCKETS] = ammoData[AMMO_ROCKETS].max;
	client->ps.ammo[AMMO_THERMAL] = ammoData[AMMO_THERMAL].max * (float) client->skillLevel[SK_THERMAL]/FORCE_LEVEL_3;

	client->ps.ammo[AMMO_DETPACK] = ammoData[AMMO_DETPACK].max * (float) client->skillLevel[SK_DETPACK]/FORCE_LEVEL_2;
	client->ps.ammo[AMMO_TUSKEN_RIFLE] = 100;
}

void Client_UpdateSabers(gentity_t *ent) {
	char userinfo[MAX_INFO_STRING];
	qboolean changedSaber = qfalse;
	char *saber;
	int index = ent - g_entities;
	int i;

	trap_GetUserinfo( index, userinfo, sizeof(userinfo) );

	for(i = 0; i < MAX_SABERS; i++) {
		char *value;
		switch (i) {
		case 0:
			saber = &ent->client->sess.saberType[0];
			break;
		case 1:
			saber = &ent->client->sess.saber2Type[0];
			break;
		default:
			saber = NULL;
			break;
		}
		
		value = Info_ValueForKey (userinfo, va("saber%i", i + 1));
		if (saber &&
			value &&
			(Q_stricmp(value, saber) || !saber[0] || !ent->client->saber[0].model[0]))
		{ //doesn't match up (or our session saber is BS), we want to try setting it
			if (G_SetSaber(ent, i, value, qfalse)) {
				changedSaber = qtrue;
			}
			else if (!saber[0] || !ent->client->saber[0].model[0]) {
				changedSaber = qtrue;
			}
		}
	}

	if (changedSaber) {
		int i;
		
		ClientUserinfoChanged( ent->s.number );
		G_SaberModelSetup(ent);

		for(i = 0; i < MAX_SABERS; i++) {
			char *value;
			switch (i) {
			case 0:
				saber = &ent->client->sess.saberType[0];
				break;
			case 1:
				saber = &ent->client->sess.saber2Type[0];
				break;
			default:
				saber = NULL;
				break;
			}

			value = Info_ValueForKey (userinfo, va("saber%i", i+1));

			if (Q_stricmp(value, saber)) {
				Info_SetValueForKey(userinfo, va("saber%i", i+1), saber);
				trap_SetUserinfo( ent->s.number, userinfo );
			}
		}	
	}
}

/*
===========
ClientSpawn

Called every time a client is placed fresh in the world:
after the first ClientBegin, and after each respawn
Initializes all non-persistant parts of playerState
============
*/
//[CoOp]
extern void UpdatePlayerScriptTarget(void);
extern qboolean UseSpawnWeapons;
extern int SpawnWeapons;
//[/CoOp]
//[VisualWeapons]
//prototype
qboolean OJP_AllPlayersHaveClientPlugin(void);
//[/VisualWeapons]
//[TABBots]
extern int FindBotType(int clientNum);
//[/TABBots]
extern qboolean WP_HasForcePowers( const playerState_t *ps );
//[StanceSelection]
extern qboolean G_ValidSaberStyle(gentity_t *ent, int saberStyle);
extern qboolean WP_SaberCanTurnOffSomeBlades( saberInfo_t *saber );
//[StanceSelection]
void ClientSpawn(gentity_t *ent) {
	int	persistant[MAX_PERSISTANT];
	gclient_t *client = ent->client;
	int index = ent - g_entities;
	int i;
	vec3_t	spawn_origin, spawn_angles;
	gentity_t *spawnPoint;
	clientPersistant_t saved;
	clientSession_t savedSess;
	int eventSequence;
	forcedata_t	savedForce;
	int saveSaberNum;
	int savedSiegeIndex;
	int	savedSkill[NUM_SKILLS];
	//[DodgeSys]
	int					savedDodgeMax;
	//[/DodgeSys]
	saberInfo_t	saberSaved[MAX_SABERS];
	void *g2WeaponPtrs[MAX_SABERS];

	int flags;
	int gameFlags;
	int savedPing;

	char userinfo[MAX_INFO_STRING];
	trap_GetUserinfo(index, userinfo, sizeof(userinfo));

	if(ent->client->sess.skillPoints < g_minForceRank.value) {//the minForceRank was changed to a higher value than the player has
		ent->client->sess.skillPoints = g_minForceRank.value;
		ent->client->skillUpdated = qtrue;

		trap_SendServerCommand(ent->s.number, va("nsp %i %s", (int) ent->client->sess.skillPoints, "Cvar Changed"));
	}

	Client_UpdateSabers(ent);

	if (client->ps.fd.forceDoInit || ent->r.svFlags & SVF_BOT) {
		WP_InitForcePowers( ent );
		client->ps.fd.forceDoInit = 0;
	}

	//[TABBots]
	if ( ent->r.svFlags & SVF_BOT && FindBotType(ent->s.number) == BOT_TAB
		&& ent->client->ps.fd.saberAnimLevel != SS_STAFF 
		&& ent->client->ps.fd.saberAnimLevel != SS_DUAL) 
	{//TABBots randomly switch styles on respawn if not using a staff or dual
		//[StanceSelection]
		int newLevel = Q_irand( SS_FAST, SS_TAVION );

		//new validation technique.
		if ( !G_ValidSaberStyle(ent, newLevel) )
		{//had an illegal style, revert to a valid one
			int count;
			for(count = SS_FAST; count < SS_TAVION; count++) {
				newLevel++;
				if(newLevel > SS_TAVION) {
					newLevel = SS_FAST;
				}

				if(G_ValidSaberStyle(ent, newLevel)) {
					break;
				}
			}
		}

		ent->client->sess.saberLevel = newLevel;
		//[/StanceSelection]
	}
	//[/TABBots]

	ent->client->ps.fd.saberAnimLevel = ent->client->ps.fd.saberDrawAnimLevel = ent->client->sess.saberLevel;
	if ( g_gametype.integer != GT_SIEGE )
	{
		//let's just make sure the styles we chose are cool
		if ( !G_ValidSaberStyle(ent, ent->client->ps.fd.saberAnimLevel) )
		{//had an illegal style, revert to default
			ent->client->ps.fd.saberAnimLevel = SS_MEDIUM;
			ent->client->saberCycleQueue = ent->client->ps.fd.saberAnimLevel;
		}
	}
	//[/StanceSelection]

	spawnPoint = Client_FindSpawnPoint(ent, spawn_origin, spawn_angles);

	client->pers.teamState.state = TEAM_ACTIVE;

	// toggle the teleport bit so the client knows to not lerp
	// and never clear the voted flag
	flags = ent->client->ps.eFlags & (EF_TELEPORT_BIT );
	flags ^= EF_TELEPORT_BIT;
	gameFlags = ent->client->mGameFlags & ( PSG_VOTED | PSG_TEAMVOTED);

	// clear everything but the persistant data

	saved = client->pers;
	savedSess = client->sess;

	savedPing = client->ps.ping;

	for (i = 0 ; i < MAX_PERSISTANT ; i++ ) {
		persistant[i] = client->ps.persistant[i];
	}

	eventSequence = client->ps.eventSequence;
	savedForce = client->ps.fd;
	saveSaberNum = client->ps.saberEntityNum;
	savedSiegeIndex = client->siegeClass;

	for(i = 0; i < NUM_SKILLS; i++) {
		savedSkill[i] = client->skillLevel[i];
	}

	//[DodgeSys]
	savedDodgeMax = client->ps.stats[STAT_MAX_DODGE];
	//[/DodgeSys]

	for(i = 0; i < MAX_SABERS; i++) {
		saberSaved[i] = client->saber[i];
		g2WeaponPtrs[i] = client->weaponGhoul2[i];
	}

	for(i = 0; i < HL_MAX; i++) {
		ent->locationDamage[i] = 0;
	}

	memset (client, 0, sizeof(*client)); // bk FIXME: Com_Memset?
	client->bodyGrabIndex = ENTITYNUM_NONE;

	//Get the skin RGB based on his userinfo
	client->ps.customRGBA[0] = Info_ColorForKey(userinfo, "char_color_red");
	client->ps.customRGBA[1] = Info_ColorForKey(userinfo, "char_color_green");
	client->ps.customRGBA[2] = Info_ColorForKey(userinfo, "char_color_blue");
	client->ps.customRGBA[3] = 255;

	if ((client->ps.customRGBA[0] + client->ps.customRGBA[1] + client->ps.customRGBA[2]) < 100)
	{ //hmm, too dark!
		client->ps.customRGBA[0] = client->ps.customRGBA[1] = client->ps.customRGBA[2] = 255;
	}

	//[BugFix32]
	//update our customRGBA for team colors. 
	if ( g_gametype.integer >= GT_TEAM && g_gametype.integer != GT_SIEGE && !g_trueJedi.integer )
	{
		char skin[MAX_QPATH];
		char model[MAX_QPATH];
		float colorOverride[3];

		colorOverride[0] = colorOverride[1] = colorOverride[2] = 0.0f;
		Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );

		BG_ValidateSkinForTeam( model, skin, savedSess.sessionTeam, colorOverride);
		if (colorOverride[0] != 0.0f ||
			colorOverride[1] != 0.0f ||
			colorOverride[2] != 0.0f)
		{
			client->ps.customRGBA[0] = colorOverride[0]*255.0f;
			client->ps.customRGBA[1] = colorOverride[1]*255.0f;
			client->ps.customRGBA[2] = colorOverride[2]*255.0f;
		}
	}
	//[/BugFix32]

	client->siegeClass = savedSiegeIndex;

	for(i = 0; i < NUM_SKILLS; i++) {
		client->skillLevel[i] = savedSkill[i];
	}

	//[DodgeSys]
	client->ps.stats[STAT_MAX_DODGE] = savedDodgeMax;
	//[/DodgeSys]

	for(i = 0; i < MAX_SABERS; i++) {
		client->saber[i] = saberSaved[i];
		client->weaponGhoul2[i] = g2WeaponPtrs[i];
	}

	for(i = 0; i < NUM_FORCE_POWERS; i++) {
		client->forcePowerStart[i]=-1;
	}
	//or the saber ent num
	client->ps.saberEntityNum = saveSaberNum;
	client->saberStoredIndex = saveSaberNum;

	client->ps.fd = savedForce;

	client->ps.duelIndex = ENTITYNUM_NONE;

	client->ps.stats[STAT_FUEL] = JETPACK_MAXFUEL;
	client->ps.cloakFuel = CLOAK_MAXFUEL;

	client->pers = saved;
	client->sess = savedSess;
	client->ps.ping = savedPing;
	client->lastkilled_client = -1;

	for (i = 0 ; i < MAX_PERSISTANT ; i++ ) {
		client->ps.persistant[i] = persistant[i];
	}

	client->ps.eventSequence = eventSequence;
	// increment the spawncount so the client will detect the respawn
	client->ps.persistant[PERS_SPAWN_COUNT]++;
	client->ps.persistant[PERS_TEAM] = client->sess.sessionTeam;

	client->airOutTime = level.time + 12000;

	// clear entity values
	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth = Client_CalculateStartMaxHealth(client);
	client->ps.eFlags = flags;
	client->mGameFlags = gameFlags;

	ent->s.groundEntityNum = ENTITYNUM_NONE;
	ent->client = &level.clients[index];
	ent->playerState = &ent->client->ps;
	ent->takedamage = qtrue;
	ent->inuse = qtrue;
	ent->classname = "player";
	ent->r.contents = CONTENTS_BODY;
	ent->clipmask = MASK_PLAYERSOLID;
	ent->die = player_die;
	ent->waterlevel = 0;
	ent->watertype = 0;
	ent->flags = 0;
	
	VectorCopy (playerMins, ent->r.mins);
	VectorCopy (playerMaxs, ent->r.maxs);
	client->ps.crouchheight = CROUCH_MAXS_2;
	client->ps.standheight = DEFAULT_MAXS_2;

	client->ps.clientNum = index;
	//give default weapons
	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_NONE );

	//[MELEE]
	//Give everyone fists as long as they aren't disabled.
	if (IsWeaponEnabled(WP_MELEE)) {
		client->ps.stats[STAT_WEAPONS] |= (1 << WP_MELEE);
	}
	//[/MELEE]

	if ( g_gametype.integer != GT_HOLOCRON 
	//[/ExpSys]
	//[/CoOp]
		&& g_gametype.integer != GT_JEDIMASTER 
		&& !HasSetSaberOnly()
		&& !AllForceDisabled()
		&& g_trueJedi.integer )
	{
		if ( g_gametype.integer >= GT_TEAM && (client->sess.sessionTeam == TEAM_BLUE || client->sess.sessionTeam == TEAM_RED) )
		{//In Team games, force one side to be merc and other to be jedi
			if ( level.numPlayingClients > 0 )
			{//already someone in the game
				int		forceTeam = TEAM_SPECTATOR, i;
				for (i = 0 ; i < level.maxclients ; i++ ) 
				{
					if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
						continue;
					}
					if ( level.clients[i].sess.sessionTeam == TEAM_BLUE || level.clients[i].sess.sessionTeam == TEAM_RED ) 
					{//in-game
						if ( WP_HasForcePowers( &level.clients[i].ps ) )
						{//this side is using force
							forceTeam = level.clients[i].sess.sessionTeam;
						}
						else
						{//other team is using force
							if ( level.clients[i].sess.sessionTeam == TEAM_BLUE )
							{
								forceTeam = TEAM_RED;
							}
							else
							{
								forceTeam = TEAM_BLUE;
							}
						}
						break;
					}
				}

				if ( WP_HasForcePowers( &client->ps ) && client->sess.sessionTeam != forceTeam )
				{//using force but not on right team, switch him over
					const char *teamName = TeamName( forceTeam );
					//client->sess.sessionTeam = forceTeam;
					SetTeam( ent, (char *)teamName );
					return;
				}
			}
		}

		if ( WP_HasForcePowers( &client->ps ) ) {
			client->ps.trueNonJedi = qfalse;
			client->ps.trueJedi = qtrue;
			//make sure they only use the saber
			client->ps.weapon = WP_SABER;
			client->ps.stats[STAT_WEAPONS] = (1 << WP_SABER);
		}
		else
		{//no force powers set
			client->ps.trueNonJedi = qtrue;
			client->ps.trueJedi = qfalse;
			if (IsWeaponEnabled(WP_BRYAR_PISTOL)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
			}

			if (IsWeaponEnabled(WP_BLASTER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BLASTER );
			}

			if (IsWeaponEnabled(WP_BOWCASTER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BOWCASTER );
			}

			if (IsWeaponEnabled(WP_REPEATER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_REPEATER );
			}

			if (IsWeaponEnabled(WP_DISRUPTOR)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DISRUPTOR );
			}

			client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_SABER);
			client->ps.stats[STAT_WEAPONS] |= (1 << WP_MELEE);
			client->ps.ammo[AMMO_POWERCELL] = ammoData[AMMO_POWERCELL].max;
			client->ps.weapon = WP_BRYAR_PISTOL;
		}
	}
	else
	{//jediVmerc is incompatible with this gametype, turn it off!
		trap_Cvar_Set( "g_jediVmerc", "0" );
		if (g_gametype.integer == GT_HOLOCRON) {
			if (IsWeaponEnabled(WP_SABER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SABER );
			}
		}
		//[ExpSys]
		else if(g_gametype.integer == GT_JEDIMASTER || g_gametype.integer == GT_HOLOCRON)
		{//don't have selectable starting weapons, but we do have max max gun skills so 
			//people can actually fight well, when they get a gun.
				client->skillLevel[SK_PISTOL] = FORCE_LEVEL_3;
				client->skillLevel[SK_BLASTER] = FORCE_LEVEL_3;
				client->skillLevel[SK_THERMAL] = FORCE_LEVEL_3;
				client->skillLevel[SK_BOWCASTER] = FORCE_LEVEL_3;
				client->skillLevel[SK_DETPACK] = FORCE_LEVEL_3;
				client->skillLevel[SK_REPEATER] = FORCE_LEVEL_3;
				client->skillLevel[SK_DISRUPTOR] = FORCE_LEVEL_3;
		}
		//[/ExpSys]
		else
		{
			if (client->ps.fd.forcePowerLevel[FP_SABER_OFFENSE] && IsWeaponEnabled(WP_SABER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SABER );	
			}

			if(client->skillLevel[SK_PISTOL] && IsWeaponEnabled(WP_BRYAR_PISTOL)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
			}

			if(client->skillLevel[SK_BLASTER] && IsWeaponEnabled(WP_BLASTER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BLASTER );
			}

			if(client->skillLevel[SK_BOWCASTER] && IsWeaponEnabled(WP_BOWCASTER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BOWCASTER );

				if(client->skillLevel[SK_BOWCASTER] > FORCE_LEVEL_2) {
					client->ps.eFlags2 |= EF2_BOWCASTERSCOPE;
				}
			}

			if(client->skillLevel[SK_TUSKEN_RIFLE] && IsWeaponEnabled(WP_TUSKEN_RIFLE)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_TUSKEN_RIFLE );
			}

			if(client->skillLevel[SK_GRENADE] && IsWeaponEnabled(WP_GRENADE)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_GRENADE );
			}

			if(client->skillLevel[SK_THERMAL] && IsWeaponEnabled(WP_THERMAL)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_THERMAL );
			}

			if(client->skillLevel[SK_ROCKET] && IsWeaponEnabled(WP_ROCKET_LAUNCHER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_ROCKET_LAUNCHER );
			}

			if(client->skillLevel[SK_DETPACK] && IsWeaponEnabled(WP_DET_PACK)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DET_PACK );
			}

			if(client->skillLevel[SK_REPEATER] && IsWeaponEnabled(WP_REPEATER)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_REPEATER );
			}

			if(client->skillLevel[SK_DISRUPTOR] && IsWeaponEnabled(WP_DISRUPTOR)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_DISRUPTOR );
			}

			if(client->skillLevel[SK_FLECHETTE] && IsWeaponEnabled(WP_FLECHETTE)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_FLECHETTE );
			}
		}

		if (g_gametype.integer == GT_JEDIMASTER) {
			if (IsWeaponEnabled(WP_BRYAR_PISTOL)) {
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
			}
		}
		
		if(!AreAllWeaponsEnabled()) {
			client->ps.stats[STAT_WEAPONS] |= (1 << WP_MELEE);
			G_Printf( "ERROR:  The game doesn't like it when you disable ALL the weapons.\nReenabling Melee.\n");

			if (g_gametype.integer == GT_DUEL || g_gametype.integer == GT_POWERDUEL) {
				trap_Cvar_Set( "g_duelWeaponDisable", va("%i", WP_MELEEONLY) );
			}
			else {
				trap_Cvar_Set( "g_weaponDisable", va("%i", WP_MELEEONLY) );
			}
		}
	
		if (g_gametype.integer == GT_JEDIMASTER) {
			client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_SABER);

			if (IsWeaponEnabled(WP_BRYAR_PISTOL)) {
				client->ps.stats[STAT_WEAPONS] |= (1 << WP_MELEE);
			}
		}

		//[ExpSys]
		//racc - set initial weapon selected.
		if (client->ps.stats[STAT_WEAPONS] & (1 << WP_SABER)) {
			client->ps.weapon = WP_SABER;
		}
		else {
			for (i = LAST_USEABLE_WEAPON ; i > WP_NONE ; i-- ) {
				if(client->ps.stats[STAT_WEAPONS] & (1 << i) ) {
					client->ps.weapon = i;
					break;
				}
			}
		}
		//[/ExpSys]
	}

	if (g_gametype.integer == GT_SIEGE && client->siegeClass != -1 &&
		client->sess.sessionTeam != TEAM_SPECTATOR)
	{ //well then, we will use a custom weaponset for our class
		int m;
		client->ps.stats[STAT_WEAPONS] = bgSiegeClasses[client->siegeClass].weapons;

		if (client->ps.stats[STAT_WEAPONS] & (1 << WP_SABER)) {
			client->ps.weapon = WP_SABER;
		}
		else if (client->ps.stats[STAT_WEAPONS] & (1 << WP_BRYAR_PISTOL)) {
			client->ps.weapon = WP_BRYAR_PISTOL;
		}
		else {
			client->ps.weapon = WP_MELEE;
		}

		for(m = 0; m < WP_NUM_WEAPONS; m++) {
			if (client->ps.stats[STAT_WEAPONS] & (1 << m))
			{
				if (client->ps.weapon != WP_SABER)
				{ //try to find the highest ranking weapon we have
					if (m > client->ps.weapon)
					{
						client->ps.weapon = m;
					}
				}

				if (m >= WP_BRYAR_PISTOL)
				{ //Max his ammo out for all the weapons he has.
					if ( g_gametype.integer == GT_SIEGE 
						&& m == WP_ROCKET_LAUNCHER )
					{//don't give full ammo!
						//FIXME: extern this and check it when getting ammo from supplier, pickups or ammo stations!
						if ( client->siegeClass != -1 &&
							(bgSiegeClasses[client->siegeClass].classflags & (1<<CFL_SINGLE_ROCKET)) )
						{
							client->ps.ammo[weaponData[m].ammoIndex] = 1;
						}
						else
						{
							client->ps.ammo[weaponData[m].ammoIndex] = 10;
						}
					}
					else
					{
						if ( g_gametype.integer == GT_SIEGE 
							&& client->siegeClass != -1
							&& (bgSiegeClasses[client->siegeClass].classflags & (1<<CFL_EXTRA_AMMO)) )
						{//double ammo
							client->ps.ammo[weaponData[m].ammoIndex] = ammoData[weaponData[m].ammoIndex].max*2;
							client->ps.eFlags |= EF_DOUBLE_AMMO;
						}
						else
						{
							client->ps.ammo[weaponData[m].ammoIndex] = ammoData[weaponData[m].ammoIndex].max;
						}
					}
				}
			}
		}
	}
	else {
		Client_LoadAmmoLevels(client);
	}

	if (g_gametype.integer == GT_SIEGE &&
		client->siegeClass != -1 &&
		client->sess.sessionTeam != TEAM_SPECTATOR)
	{ //use class-specified inventory
		client->ps.stats[STAT_HOLDABLE_ITEMS] = bgSiegeClasses[client->siegeClass].invenItems;
		client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
	}
	else
	{
		client->itemSlot1 = 0;
		client->itemSlot2 = 0; //Reset it to 0

		//[ExpSys]
		client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
		client->ps.stats[STAT_GADGET_1] = client->ps.stats[STAT_GADGET_2] = 999;
		
		if(client->skillLevel[SK_JETPACK])
		{//player has jetpack
			client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_JETPACK);
			client->ps.eFlags |= EF_JETPACK;
		}

		if(client->skillLevel[SK_FORCEFIELD])
		{//give the player the force field item

			//If we can't find any available slot it means they're already taken
			//and so don't allow any more items
			if(client->itemSlot1 == 0)
			{
				client->ps.stats[STAT_GADGET_1] = HI_SHIELD;
				client->itemSlot1 = SK_FORCEFIELD;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_SHIELD);
			}
			else if(client->itemSlot2 == 0)
			{
				client->ps.stats[STAT_GADGET_2] = HI_SHIELD;
				client->itemSlot2 = SK_FORCEFIELD;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_SHIELD);
			}

		}

		if(client->skillLevel[SK_CLOAK])
		{//give the player the cloaking device

			if(client->itemSlot1 == 0)
			{
				client->ps.stats[STAT_GADGET_1] = HI_CLOAK;
				client->itemSlot1 = SK_CLOAK;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_CLOAK);
			}
			else if(client->itemSlot2 == 0)
			{
				client->ps.stats[STAT_GADGET_2] = HI_CLOAK;
				client->itemSlot2 = SK_CLOAK;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_CLOAK);
			}
		}

		if(client->skillLevel[SK_BACTA] == FORCE_LEVEL_2)
		{
			if(client->itemSlot1 == 0)
			{
				client->ps.stats[STAT_GADGET_1] = HI_MEDPAC_BIG;
				client->itemSlot1 = SK_BACTA;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_MEDPAC_BIG);
			}
			else if(client->itemSlot2 == 0)
			{
				client->ps.stats[STAT_GADGET_2] = HI_MEDPAC_BIG;
				client->itemSlot2 = SK_BACTA;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_MEDPAC_BIG);
			}
		}
		else if(client->skillLevel[SK_BACTA])
		{
			if(client->itemSlot1 == 0)
			{
				client->ps.stats[STAT_GADGET_1] = HI_MEDPAC;
				client->itemSlot1 = SK_BACTA;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_MEDPAC);
			}
			else if(client->itemSlot2 == 0)
			{
				client->ps.stats[STAT_GADGET_2] = HI_MEDPAC;
				client->itemSlot2 = SK_BACTA;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_MEDPAC);
			}
		}

		//gain flamethrower
		if(client->skillLevel[SK_FLAMETHROWER])
		{
			if(client->itemSlot1 == 0)
			{
				client->ps.stats[STAT_GADGET_1] = HI_FLAMETHROWER;
				client->itemSlot1 = SK_FLAMETHROWER;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_FLAMETHROWER);
			}
			else if(client->itemSlot2 == 0)
			{
				client->ps.stats[STAT_GADGET_2] = HI_FLAMETHROWER;
				client->itemSlot2 = SK_FLAMETHROWER;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_FLAMETHROWER);
			}
		}

		if(client->skillLevel[SK_SEEKER])
		{
			if(client->itemSlot1 == 0)
			{
				client->ps.stats[STAT_GADGET_1] = HI_SEEKER;
				client->itemSlot1 = SK_SEEKER;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_SEEKER);
			}
			else if(client->itemSlot2 == 0)
			{
				client->ps.stats[STAT_GADGET_2] = HI_SEEKER;
				client->itemSlot2 = SK_SEEKER;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_SEEKER);
			}
		}

		if(client->skillLevel[SK_SENTRY])
		{
			if(client->itemSlot1 == 0)
			{
				client->ps.stats[STAT_GADGET_1] = HI_SENTRY_GUN;
				client->itemSlot1 = SK_SENTRY;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_SENTRY_GUN);
			}
			else if(client->itemSlot2 == 0)
			{
				client->ps.stats[STAT_GADGET_2] = HI_SENTRY_GUN;
				client->itemSlot2 = SK_SENTRY;
				client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_SENTRY_GUN);
			}
		}

		//[/ExpSys]
		client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
	}

	if (g_gametype.integer == GT_SIEGE &&
		client->siegeClass != -1 &&
		bgSiegeClasses[client->siegeClass].powerups &&
		client->sess.sessionTeam != TEAM_SPECTATOR)
	{ //this class has some start powerups	
		for(i = 0; i < PW_NUM_POWERUPS; i++) {
			if (bgSiegeClasses[client->siegeClass].powerups & (1 << i)) {
				client->ps.powerups[i] = Q3_INFINITE;
			}
		}
	}

	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
		client->ps.stats[STAT_WEAPONS] = 0;
		client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
		client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
	}

	client->ps.rocketLockIndex = ENTITYNUM_NONE;
	client->ps.rocketLockTime = 0;

	//rww - Set here to initialize the circling seeker drone to off.
	//A quick note about this so I don't forget how it works again:
	//ps.genericEnemyIndex is kept in sync between the server and client.
	//When it gets set then an entitystate value of the same name gets
	//set along with an entitystate flag in the shared bg code. Which
	//is why a value needs to be both on the player state and entity state.
	//(it doesn't seem to just carry over the entitystate value automatically
	//because entity state value is derived from player state data or some
	//such)
	client->ps.genericEnemyIndex = -1;

	if(OJP_AllPlayersHaveClientPlugin()) {
		G_AddEvent(ent, EV_WEAPINVCHANGE, client->ps.stats[STAT_WEAPONS]);
	}

	client->ps.isJediMaster = qfalse;

	if (client->ps.fallingToDeath) {
		client->ps.fallingToDeath = 0;
		client->noCorpse = qtrue;
	}

	//Do per-spawn force power initialization
	WP_SpawnInitForcePowers( ent );

	//[StanceSelection]
	//set saberAnimLevelBase
	if(ent->client->saber[0].model[0] && ent->client->saber[1].model[0]) {
		ent->client->ps.fd.saberAnimLevelBase = SS_DUAL;
	}
	else if (ent->client->saber[0].numBlades > 1
		&& WP_SaberCanTurnOffSomeBlades( &ent->client->saber[0] )) {
		ent->client->ps.fd.saberAnimLevelBase = SS_STAFF;
	}
	else {
		ent->client->ps.fd.saberAnimLevelBase = SS_MEDIUM;
	}

	//set initial saberholstered mode
	if (ent->client->saber[0].model[0] && ent->client->saber[1].model[0]
			&& WP_SaberCanTurnOffSomeBlades( &ent->client->saber[1] ) 
			&& ent->client->ps.fd.saberAnimLevel != SS_DUAL)
	{//using dual sabers, but not the dual style, turn off blade
		ent->client->ps.saberHolstered = 1;
	}
	else if (ent->client->saber[0].numBlades > 1
		&& WP_SaberCanTurnOffSomeBlades( &ent->client->saber[0] ) 
		&& ent->client->ps.fd.saberAnimLevel != SS_STAFF)
	{//using staff saber, but not the staff style, turn off blade
		ent->client->ps.saberHolstered = 1;
	}
	else
	{
		ent->client->ps.saberHolstered = 0;
	}
	//[/StanceSelection]

	// health will count down towards max_health
	if (g_gametype.integer == GT_SIEGE &&
		client->siegeClass != -1 &&
		bgSiegeClasses[client->siegeClass].starthealth)
	{ //class specifies a start health, so use it
		ent->health = client->ps.stats[STAT_HEALTH] = bgSiegeClasses[client->siegeClass].starthealth;
	}
	else if ( g_gametype.integer == GT_DUEL || g_gametype.integer == GT_POWERDUEL )
	{//only start with 100 health in Duel
		if ( g_gametype.integer == GT_POWERDUEL && client->sess.duelTeam == DUELTEAM_LONE )
		{
			if ( g_duel_fraglimit.integer )
			{			
				ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] =
					g_powerDuelStartHealth.integer - ((g_powerDuelStartHealth.integer - g_powerDuelEndHealth.integer) * (float)client->sess.wins / (float)g_duel_fraglimit.integer);
			}
			else
			{
				ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] = 150;
			}
		}
		else
		{
			ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] = 100;
		}
	}
	else
	{
		ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH];
	}

	client->ps.stats[STAT_ARMOR] = Client_CalculateStartArmor(client);
	//[/ExpSys]

	//[DodgeSys]
	//Init dodge stat.
	client->ps.stats[STAT_DODGE] = client->ps.stats[STAT_MAX_DODGE];
	//[/DodgeSys]

	G_SetOrigin( ent, spawn_origin );
	VectorCopy( spawn_origin, client->ps.origin );

	// the respawned flag will be cleared after the attack and jump keys come up
	client->ps.pm_flags |= PMF_RESPAWNED;

	trap_GetUsercmd( client - level.clients, &ent->client->pers.cmd );
	SetClientViewAngle( ent, spawn_angles );

	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		G_KillBox( ent );
		trap_LinkEntity (ent);

		// force the base weapon up
		if (client->ps.weapon <= WP_NONE) {
			client->ps.weapon = WP_BRYAR_PISTOL;
		}

		client->ps.torsoTimer = client->ps.legsTimer = 0;

		if (client->ps.weapon == WP_SABER) {
			G_SetAnim(ent, NULL, SETANIM_BOTH, BOTH_STAND1TO2, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD|SETANIM_FLAG_HOLDLESS, 0);
		}
		else {
			G_SetAnim(ent, NULL, SETANIM_TORSO, TORSO_RAISEWEAP1, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD|SETANIM_FLAG_HOLDLESS, 0);
			client->ps.legsAnim = WeaponReadyAnim[client->ps.weapon];
		}

		client->ps.weaponstate = WEAPON_RAISING;
		client->ps.weaponTime = client->ps.torsoTimer;
	}

	// don't allow full run speed for a bit
	client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	client->ps.pm_time = 100;

	client->respawnTime = level.time;
	client->inactivityTime = level.time + g_inactivity.integer * 1000;
	client->latched_buttons = 0;

	if ( level.intermissiontime ) {
		MoveClientToIntermission( ent );
	} else {
		UpdatePlayerScriptTarget();
	}

	//set teams for NPCs to recognize
	if (g_gametype.integer == GT_SIEGE)
	{ //Imperial (team1) team is allied with "enemy" NPCs in this mode
		if (client->sess.sessionTeam == SIEGETEAM_TEAM1) {
			client->playerTeam = ent->s.teamowner = NPCTEAM_ENEMY;
			client->enemyTeam = NPCTEAM_PLAYER;
		}
		else {
			client->playerTeam = ent->s.teamowner = NPCTEAM_PLAYER;
			client->enemyTeam = NPCTEAM_ENEMY;
		}
	}
	else
	{
		client->playerTeam = ent->s.teamowner = NPCTEAM_PLAYER;
		client->enemyTeam = NPCTEAM_ENEMY;
	}

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	client->ps.commandTime = level.time - 100;
	ent->client->pers.cmd.serverTime = level.time;
	ClientThink( ent-g_entities, NULL );

	// positively link the client, even if the command times are weird
	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
		VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
		trap_LinkEntity( ent );
	}

	if (g_spawnInvulnerability.integer) {
		ent->client->ps.eFlags |= EF_INVULNERABLE;
		ent->client->invulnerableTimer = level.time + g_spawnInvulnerability.integer;
	}

	// run the presend to set anything else
	ClientEndFrame( ent );

	// clear entity state values
	BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );

	//rww - make sure client has a valid icarus instance
	trap_ICARUS_FreeEnt( ent );
	trap_ICARUS_InitEnt( ent );

	//[CoOp]
	if(!level.intermissiontime)
	{//we want to use all the spawnpoint's triggers if we're not in intermission.
		G_UseTargets( spawnPoint, ent );
		if (g_gametype.integer == GT_SINGLE_PLAYER && spawnPoint)
		{//remove the target of the spawnpoint to prevent multiple target firings
			spawnPoint->target = NULL;
		}
	}
	//[/CoOp]
	CalculateAndSetRank(ent);
}


/*
===========
ClientDisconnect

Called when a player drops from the server.
Will not be called between levels.

This should NOT be called directly by any game logic,
call trap_DropClient(), which will call this and do
server system housekeeping.
============
*/
//[BugFix38]
extern void G_LeaveVehicle( gentity_t* ent, qboolean ConCheck );
//[/BugFix38]
void ClientDisconnect( int clientNum ) {
	// cleanup if we are kicking a bot that
	// hasn't spawned yet
	int i;
	gentity_t	*ent;
	G_RemoveQueuedBotBegin( clientNum );

	ent = g_entities + clientNum;
	if ( !ent->client ) {
		return;
	}

	for(i = 0; i < NUM_FORCE_POWERS; i++) {
		if (ent->client->ps.fd.forcePowersActive & (1 << i)) {
			WP_ForcePowerStop(ent, i);
		}
	}

	for(i = TRACK_CHANNEL_1; i < NUM_TRACK_CHANNELS; i++) {
		if (ent->client->ps.fd.killSoundEntIndex[i-50] && ent->client->ps.fd.killSoundEntIndex[i-50] < MAX_GENTITIES && ent->client->ps.fd.killSoundEntIndex[i-50] > 0) {
			G_MuteSound(ent->client->ps.fd.killSoundEntIndex[i-50], CHAN_VOICE);
		}
	}
	
	G_LeaveVehicle( ent, qtrue );

	// stop any following clients
	for (i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].sess.sessionTeam == TEAM_SPECTATOR
			&& level.clients[i].sess.spectatorState == SPECTATOR_FOLLOW
			&& level.clients[i].sess.spectatorClient == clientNum ) {
			StopFollowing( &g_entities[i] );
		}
	}

	// send effect if they were completely connected
	if ( ent->client->pers.connected == CON_CONNECTED 
		&& ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		gentity_t	*tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
		tent->s.clientNum = ent->s.clientNum;

		// They don't get to take powerups with them!
		// Especially important for stuff like CTF flags
		TossClientItems( ent );
	}

	G_LogPrintf( "ClientDisconnect: %i\n", clientNum );
	//[AdminSys]
	G_LogPrintf( "%s" S_COLOR_WHITE " disconnected with IP: %s\n", ent->client->pers.netname, ent->client->sess.IPstring );
	//[/AdminSys]

	// if we are playing in tourney mode, give a win to the other player and clear his frags for this round
	if ( (g_gametype.integer == GT_DUEL )
		&& !level.intermissiontime
		&& !level.warmupTime ) 
	{
		if ( level.sortedClients[1] == clientNum ) 
		{
			level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] = 0;
			level.clients[ level.sortedClients[0] ].sess.wins++;
			ClientUserinfoChanged( level.sortedClients[0] );
		}
		else if ( level.sortedClients[0] == clientNum ) {
			level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE] = 0;
			level.clients[ level.sortedClients[1] ].sess.wins++;
			ClientUserinfoChanged( level.sortedClients[1] );
		}
	}

	if (ent->ghoul2 && trap_G2_HaveWeGhoul2Models(ent->ghoul2))
	{
		trap_G2API_CleanGhoul2Models(&ent->ghoul2);
	}
	
	for(i = 0; i < MAX_SABERS; i++) {
		if (ent->client->weaponGhoul2[i] && trap_G2_HaveWeGhoul2Models(ent->client->weaponGhoul2[i])) {
			trap_G2API_CleanGhoul2Models(&ent->client->weaponGhoul2[i]);
		}
	}

	trap_UnlinkEntity (ent);
	ent->s.modelindex = 0;
	ent->inuse = qfalse;
	ent->classname = "disconnected";
	ent->client->pers.connected = CON_DISCONNECTED;
	ent->client->ps.persistant[PERS_TEAM] = TEAM_FREE;
	ent->client->sess.sessionTeam = TEAM_FREE;
	ent->r.contents = 0;
	
	//[BugFix39]
	// we call this after all the clearing because the objectiveItem's
	// think checks if the client entity is still inuse
	// (which it is not anymore.) << ent->inuse >>
	if (ent->client->holdingObjectiveItem > 0)
	{ //carrying a siege objective item - make sure it updates and removes itself from us now in case this is a reconnecting client to make the ent remove
		gentity_t *objectiveItem = &g_entities[ent->client->holdingObjectiveItem];

		if (objectiveItem->inuse && objectiveItem->think)
		{
            objectiveItem->think(objectiveItem);
		}
	}
	//[/BugFix39]

	trap_SetConfigstring( CS_PLAYERS + clientNum, "");

	CalculateRanks();

	if ( ent->r.svFlags & SVF_BOT ) {
		BotAIShutdownClient( clientNum, qfalse );
	}

	G_ClearClientLog(clientNum);
}


//[AdminSys]
//stuff needed for new automatic team balancing system

//find the player that has been on the server the least amount of time for this team
//HumanOnly  = sets a scan for human only bots. False scans for any player but picks
//bots over humans.
gentity_t *FindYoungestPlayeronTeam(int team, qboolean HumanOnly) {
	int		YoungestNum = -1;
	int		Age = -1;
	int i;

	for (i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
			continue;
		}

		if ( level.clients[i].sess.sessionTeam != team  
				|| (g_gametype.integer == GT_SIEGE && level.clients[i].sess.siegeDesiredTeam != team) ) 
		{//not on the right team.
			continue;
		}

		//check to see if bot should replace a human as the youngest player.
		//This makes bots be found by age, but bots will be picked over humans reguardless of age.
		if(g_entities[i].r.svFlags & SVF_BOT)
		{//a bot
			if(HumanOnly)
			{//ignore bots
				continue;
			}
			else
			{
				if(YoungestNum != -1 && !(g_entities[YoungestNum].r.svFlags & SVF_BOT))
				{//the youngest player on this team is a human, replace him with this bot.
					Age = level.clients[i].pers.enterTime;
					YoungestNum = i;
					continue;
				}
			}
		}

		//age check
		if(level.clients[i].pers.enterTime > Age)
		{//found a younger player
			Age = level.clients[i].pers.enterTime;
			YoungestNum = i;
		}
	}

	if(YoungestNum != -1) {
		return &g_entities[YoungestNum];
	}

	return NULL;
}

extern int G_CountBotPlayers( int team );
extern int G_CountHumanPlayers( int ignoreClientNum, int team );
void CheckTeamBotBalance(void)
{//Attempts to balance the human/bot ratio on the teams to keep it from becoming a human
	//vs bot situation.  This function does NOT do the standard checks for team balance
	//as since they are already done in CheckTeamBalance.
	
	int BotCount[TEAM_NUM_TEAMS];
	int HumanCount = G_CountHumanPlayers( -1, -1 );
	int	lesserBotTeam = -1;  //The team with the fewer bots
	int HumanChangedToTeam = -1;	//team where the human was moved to.
	gentity_t *HumanSwitchEnt = NULL;

	if(HumanCount < 2)
	{//not enough humans to need to check for bot imbalance.
		return;
	}

	//count up bots
	BotCount[TEAM_BLUE] = G_CountBotPlayers( TEAM_BLUE );
	BotCount[TEAM_RED] = G_CountBotPlayers( TEAM_RED );

	//check to see which team has fewer bots
	if(BotCount[TEAM_RED] - BotCount[TEAM_BLUE] > 1)
	{//red team has too many players
		lesserBotTeam = TEAM_BLUE;
	}
	else if(BotCount[TEAM_BLUE] - BotCount[TEAM_RED] > 1)
	{//blue team has too many players
		lesserBotTeam = TEAM_RED;
	}

	//scan for a human to switch to the bot strong team.
	if( lesserBotTeam == -1 )
	{//teams are good as is
		return;
	}
	else if( lesserBotTeam == TEAM_RED )
	{
		HumanSwitchEnt = FindYoungestPlayeronTeam( TEAM_RED, qtrue );
	}
	else
	{//( lesserBotTeam == TEAM_BLUE ) 
		HumanSwitchEnt = FindYoungestPlayeronTeam( TEAM_BLUE, qtrue );
	}

	if(HumanSwitchEnt)
	{//found someone, force them onto the other team
		if(lesserBotTeam == TEAM_RED)
		{//move to blue team
			SetTeam(HumanSwitchEnt, "b");
			HumanChangedToTeam = TEAM_BLUE;
		}
		else
		{//move to red team
			SetTeam(HumanSwitchEnt, "r");
			HumanChangedToTeam = TEAM_RED;
		}

		//send out the related messages.
		//[ClientNumFix]
		trap_SendServerCommand( HumanSwitchEnt-g_entities, "cp \"You were Team Switched for Game Balance\n\"" );
		//trap_SendServerCommand( HumanSwitchEnt->client->ps.clientNum, "cp \"You were Team Switched for Game Balance\n\"" );
		//[/ClientNumFix]
		trap_SendServerCommand(-1, va("print \"AutoGameBalancer: Moved %s" S_COLOR_WHITE " to %s Team because there wasn't enough humans on that team.\n\"",
			HumanSwitchEnt->client->pers.netname, TeamName(HumanChangedToTeam)));
		//[AdminSys]
		//switch events are now logged to the server log.
		G_LogPrintf("AutoGameBalancer: Moved %s" S_COLOR_WHITE " to %s Team because there wasn't enough humans on that team.\n\"",
			HumanSwitchEnt->client->pers.netname, TeamName(HumanChangedToTeam));
		//[/AdminSys]
	}
}

//debouncer for CheckTeamBalance function
static int CheckTeamDebounce = 0;

//checks the teams to make sure they are even.  If they aren't, bump the newest player to 
//even the teams.
extern int OJP_PointSpread(void);
void CheckTeamBalance(void)
{
	//only check team balance every 30 seconds
	if(g_gametype.integer < GT_TEAM)
	{//non-team games.  Don't do checking.
		return;
	}
	else if((level.time - CheckTeamDebounce) < 30000)
	{
		return;
	}
	else
	{//go ahead and check.
		int	counts[TEAM_NUM_TEAMS];
		int	changeto = -1;
		int changefrom = -1;
		qboolean ScoreBalance = qfalse;

		CheckTeamDebounce = level.time;

		counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE );
		counts[TEAM_RED] = TeamCount( -1, TEAM_RED );

		if(counts[TEAM_RED] - counts[TEAM_BLUE] > 1)
		{//red team has too many players
			changeto = TEAM_BLUE;
			changefrom = TEAM_RED;
		}
		else if(counts[TEAM_BLUE] - counts[TEAM_RED] > 1)
		{//blue team has too many players
			changeto = TEAM_RED;
			changefrom = TEAM_BLUE;
		}

		if(g_teamForceBalance.integer >= 3 && g_gametype.integer != GT_SIEGE)
		{//check the scores 
			if(level.teamScores[TEAM_BLUE] - OJP_PointSpread() >= level.teamScores[TEAM_RED] 
				&& counts[TEAM_BLUE] > counts[TEAM_RED])
			{//blue team is ahead but also has more players
				ScoreBalance = qtrue;
				changeto = TEAM_RED;
				changefrom = TEAM_BLUE;
			}
			else if(level.teamScores[TEAM_RED] - OJP_PointSpread() >= level.teamScores[TEAM_BLUE] 
				&& counts[TEAM_RED] > counts[TEAM_BLUE])
			{//red team is ahead but also has more players
				ScoreBalance = qtrue;
				changeto = TEAM_BLUE;
				changefrom = TEAM_RED;
			}
		}

		//try to even the teams if it's unbalanced
		if( changeto != -1 && changefrom != -1 )
		{//find the youngest player on changefrom team and place them on changeto's team
			gentity_t *ent = FindYoungestPlayeronTeam(changefrom, qfalse);

			if(ent)
			{//switch teams
				//[ClientNumFix]
				trap_SendServerCommand( ent-g_entities, "cp \"You were Team Switched for Game Balance\n\"" );
				//trap_SendServerCommand( ent->client->ps.clientNum, "cp \"You were Team Switched for Game Balance\n\"" );
				//[/ClientNumFix]
				SetTeam(ent, "");
				if(!ScoreBalance)
				{
					trap_SendServerCommand(-1, va("print \"AutoGameBalancer: Moved %s" S_COLOR_WHITE " to %s Team because the teams were uneven.\n\"",ent->client->pers.netname, TeamName(changeto)));
				}
				else
				{
					trap_SendServerCommand(-1, va("print \"AutoGameBalancer: Moved %s" S_COLOR_WHITE " to %s Team because the %s Team is ahead.\n\"",ent->client->pers.netname, TeamName(changeto), TeamName(changefrom)));
				}
			}
		}
		else if(g_teamForceBalance.integer == 4)
		{//check for the human/bot ratio on the teams since the teams are already balanced.
			CheckTeamBotBalance();
		}
	}
}


//[CoOp]
qboolean G_StandardHumanoid( gentity_t *self )
{
	char		GLAName[MAX_QPATH];

	if ( !self || !self->ghoul2 )
	{
		return qfalse;
	}

	trap_G2API_GetGLAName( &self->ghoul2, 0, GLAName );
	assert(GLAName);
	if (GLAName) 
	{
		if ( !Q_stricmpn( "models/players/_humanoid", GLAName, 24 ) )///_humanoid", GLAName, 36) )
		{//only _humanoid skeleton is expected to have these
			return qtrue;
		}
		if ( !Q_stricmp( "models/players/protocol/protocol", GLAName ) )
		{//protocol droid duplicates many of these
			return qtrue;
		}
		if ( !Q_stricmp( "models/players/assassin_droid/model", GLAName ) )
		{//assassin_droid duplicates many of these
			return qtrue;
		}
		if ( !Q_stricmp( "models/players/saber_droid/model", GLAName ) )
		{//saber_droid duplicates many of these
			return qtrue;
		}
		if ( !Q_stricmp( "models/players/hazardtrooper/hazardtrooper", GLAName ) )
		{//hazardtrooper duplicates many of these
			return qtrue;
		}
		if ( !Q_stricmp( "models/players/rockettrooper/rockettrooper", GLAName ) )
		{//rockettrooper duplicates many of these
			return qtrue;
		}
		if ( !Q_stricmp( "models/players/wampa/wampa", GLAName ) )
		{//rockettrooper duplicates many of these
			return qtrue;
		}
	}
	return qfalse;
}
//[/CoOp]


qboolean OJP_AllPlayersHaveClientPlugin(void) {
	int i;
	for(i = 0; i < level.maxclients; i++) {
		if(g_entities[i].inuse && !g_entities[i].client->pers.ojpClientPlugIn) {
			return qfalse;
		}
	}

	return qtrue;
}


//[LastManStanding]
qboolean LMS_EnoughPlayers()
{//checks to see if there's enough players in game to enable LMS rules
	if(level.numNonSpectatorClients < 2)
	{//definitely not enough.
		return qfalse;
	}

	if(g_gametype.integer >= GT_TEAM 
		&& (TeamCount( -1, TEAM_RED ) == 0 || TeamCount( -1, TEAM_BLUE ) == 0) )
	{//have to have at least one player on each team to be able to play LMS
		return qfalse;
	}

	return qtrue;
}
//[/LastManStanding]

