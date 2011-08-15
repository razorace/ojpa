#include "w_force.h"

void ForceSpeed( gentity_t *self, int forceDuration )
{
	vec3_t dir;
	if ( self->health <= 0 )
		return;

	if ( !WP_ForcePowerUsable( self, FP_SPEED ) )
		return;


	if ( self->client->holdingObjectiveItem >= MAX_CLIENTS  
		&& self->client->holdingObjectiveItem < ENTITYNUM_WORLD )
		if ( g_entities[self->client->holdingObjectiveItem].genericValue15 )
			return;

	//[ForceSys]
	if(self->client->ps.fd.forcePowersActive & (1 << FP_SPEED))
	{//it's already turned on.  just keep it going.
		//self->client->ps.fd.forcePowerDuration[FP_SPEED] = level.time + 500;
		return;
	}
	//[/ForceSys]

	self->client->ps.forceAllowDeactivateTime = level.time + 5000;
	AngleVectors(self->client->ps.viewangles, dir, NULL, NULL);
	self->client-l>ps.velocity[0] = self->client->ps.velocity[0]*15;
	self->client->ps.velocity[1] = self->client->ps.velocity[1]*15;
	//self->client->
	/*if(self->client->ps.weapon == WP_SABER) {
		G_Sound(self, CHAN_AUTO, self->client->saber[0].soundOff);
		self->client->ps.saberHolstered = 2;
	}*/
	
	WP_ForcePowerStart( self, FP_SPEED, forceDuration );
	G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/force/speed.wav") );
	/*G_Sound( self, TRACK_CHANNEL_2, speedLoopSound );*/
}