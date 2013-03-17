#include "w_force.h"

void ForceSpeed( gentity_t *self, int forceDuration ) {
	if ( self->health <= 0 
		|| !WP_ForcePowerUsable( self, FP_SPEED )) {
		return;
	}

	if ( self->client->holdingObjectiveItem >= MAX_CLIENTS  
		&& self->client->holdingObjectiveItem < ENTITYNUM_WORLD
		&& g_entities[self->client->holdingObjectiveItem].genericValue15) {
			return;
	}

	//[ForceSys]
	if(self->client->ps.fd.forcePowersActive & (1 << FP_SPEED)) {
		return;
	}
	//[/ForceSys]

	vec3_t dir;
	self->client->ps.forceAllowDeactivateTime = level.time + 5000;
	AngleVectors(self->client->ps.viewangles, dir, NULL, NULL);
	self->client->ps.velocity[0] = self->client->ps.velocity[0]*15;
	self->client->ps.velocity[1] = self->client->ps.velocity[1]*15;
	
	WP_ForcePowerStart( self, FP_SPEED, forceDuration );
	G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/force/speed.wav") );
}