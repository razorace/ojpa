#include "g_seeker.h"

void FindGenericEnemyIndex(gentity_t *self)
{ //Find another client that would be considered a threat.
	gentity_t *besten = NULL;
	float blen = 99999999;

	for(int i = 0; i < MAX_CLIENTS; i++) {
		gentity_t *ent = &g_entities[i];

		if (ent && ent->client && ent->s.number != self->s.number && ent->health > 0 && !OnSameTeam(self, ent) && ent->client->ps.pm_type != PM_INTERMISSION && ent->client->ps.pm_type != PM_SPECTATOR)
		{
			vec3_t a;
			VectorSubtract(ent->client->ps.origin, self->client->ps.origin, a);
			float tlen = VectorLength(a);

			if (tlen < blen &&
				InFront(ent->client->ps.origin, self->client->ps.origin, self->client->ps.viewangles, 0.8f ) &&
				OrgVisible(self->client->ps.origin, ent->client->ps.origin, self->s.number)) {
				blen = tlen;
				besten = ent;
			}
		}
	}

	if (!besten) {
		return;
	}

	self->client->ps.genericEnemyIndex = besten->s.number;
}


void SeekerDroneUpdate(gentity_t *self) {
	vec3_t org, elevated, dir, endir;
	float angle;
	float prefig = 0;
	trace_t tr;
	const vec3_t a = {1, 0, 0};

	if (!(self->client->ps.eFlags & EF_SEEKERDRONE)) {
		self->client->ps.genericEnemyIndex = -1;
		return;
	}

	if (self->health < 1) {
		VectorCopy(self->client->ps.origin, elevated);
		elevated[2] += 40;

		angle = ((level.time / 12) & 255) * (M_PI * 2) / 255; //magical numbers make magic happen
		dir[0] = cos(angle) * 20;
		dir[1] = sin(angle) * 20;
		dir[2] = cos(angle) * 5;
		VectorAdd(elevated, dir, org);

		G_PlayEffect(EFFECT_SPARK_EXPLOSION, org, a);

		self->client->ps.eFlags -= EF_SEEKERDRONE;
		self->client->ps.genericEnemyIndex = -1;

		return;
	}

	if (self->client->ps.droneExistTime >= level.time && 
		self->client->ps.droneExistTime < (level.time + 5000))
	{
		self->client->ps.genericEnemyIndex = 1024 + self->client->ps.droneExistTime;
		if (self->client->ps.droneFireTime < level.time) {
			G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/laser_trap/warning.wav") );
			self->client->ps.droneFireTime = level.time + 100;
		}
		
		return;
	}
	else if (self->client->ps.droneExistTime < level.time) {
		VectorCopy(self->client->ps.origin, elevated);
		elevated[2] += 40;

		prefig = min(max(((self->client->ps.droneExistTime-level.time) / 80), 1), 55);

		elevated[2] -= 55 - prefig;

		angle = ((level.time / 12) & 255) * (M_PI * 2) / 255; //magical numbers make magic happen
		dir[0] = cos(angle) * 20;
		dir[1] = sin(angle) * 20;
		dir[2] = cos(angle) * 5;
		VectorAdd(elevated, dir, org);

		G_PlayEffect(EFFECT_SPARK_EXPLOSION, org, a);

		self->client->ps.eFlags -= EF_SEEKERDRONE;
		self->client->ps.genericEnemyIndex = -1;

		return;
	}

	if (self->client->ps.genericEnemyIndex == -1) {
		self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
	}

	if (self->client->ps.genericEnemyIndex != ENTITYNUM_NONE && self->client->ps.genericEnemyIndex != -1) {
		gentity_t *en = &g_entities[self->client->ps.genericEnemyIndex];

		if (!en || !en->client) {
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else if (en->s.number == self->s.number) {
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else if (en->health < 1) {
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else if (OnSameTeam(self, en)) {
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else if (!InFront(en->client->ps.origin, self->client->ps.origin, self->client->ps.viewangles, 0.8f )) {
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else if (!OrgVisible(self->client->ps.origin, en->client->ps.origin, self->s.number)) {
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
	}

	if (self->client->ps.genericEnemyIndex == ENTITYNUM_NONE || self->client->ps.genericEnemyIndex == -1) {
		FindGenericEnemyIndex(self);
	}

	if (self->client->ps.genericEnemyIndex != ENTITYNUM_NONE && self->client->ps.genericEnemyIndex != -1) {
		gentity_t *en = &g_entities[self->client->ps.genericEnemyIndex];

		VectorCopy(self->client->ps.origin, elevated);
		elevated[2] += 40;

		angle = ((level.time / 12) & 255) * (M_PI * 2) / 255; //magical numbers make magic happen
		dir[0] = cos(angle) * 20;
		dir[1] = sin(angle) * 20;
		dir[2] = cos(angle) * 5;
		VectorAdd(elevated, dir, org);

		//org is now where the thing should be client-side because it uses the same time-based offset
		if (self->client->ps.droneFireTime < level.time) {
			trap_Trace(&tr, org, NULL, NULL, en->client->ps.origin, -1, MASK_SOLID);

			if (tr.fraction == 1 && !tr.startsolid && !tr.allsolid)
			{
				VectorSubtract(en->client->ps.origin, org, endir);
				VectorNormalize(endir);

				WP_FireGenericBlasterMissile(self, org, endir, qfalse, 15, 2000, MOD_BLASTER);
				G_SoundAtLoc( org, CHAN_WEAPON, G_SoundIndex("sound/weapons/bryar/fire.wav") );

				self->client->ps.droneFireTime = level.time + Q_irand(400, 700);
			}
		}
	}
}

