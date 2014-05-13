//This file contains functions relate to the saber impact behavior of OJP Enhanced's saber system.
#include "g_local.h"
#include "g_saberbeh.h"
#include "ai_main.h"

extern vmCvar_t		g_debugsaberbehavior;
extern stringID_table_t SaberMoveTable[];
extern stringID_table_t SaberBlockedTable[];

stringID_table_t sabBehaveTable[] =
{
	ENUM2STRING(SABBEHAVE_NONE),
	ENUM2STRING(SABBEHAVE_ATTACK),
	ENUM2STRING(SABBEHAVE_ATTACKPARRIED),
	ENUM2STRING(SABBEHAVE_ATTACKBLOCKED),
	ENUM2STRING(SABBEHAVE_BLOCK),
	ENUM2STRING(SABBEHAVE_BLOCKFAKED),
	ENUM2STRING(SABBEHAVE_PARRY)
};


static GAME_INLINE void ClearSabMech( sabmech_t *sabmech)
{
	sabmech->doStun = qfalse;
	sabmech->doKnockdown = qfalse;
	sabmech->doDisarm = qfalse;
	sabmech->doParry = qfalse;
	sabmech->doSlowBounce = qfalse;
	sabmech->doHeavySlowBounce = qfalse;
#ifdef _DEBUG
	sabmech->behaveMode = SABBEHAVE_NONE;
#endif
}



extern qboolean BG_SaberInNonIdleDamageMove(playerState_t *ps, int AnimIndex);
qboolean SabBeh_RollBalance(gentity_t *self, sabmech_t *mechSelf, qboolean forceMishap,qboolean atkParry)
{
	int randNum; 
	if( self->client->ps.saberAttackChainCount <= BALANCE_LOST )
	{//hard mishap.
		//mechSelf->doKnockdown = qtrue;
		mechSelf->doDisarm = qtrue;
		self->client->ps.saberAttackChainCount = BALANCE_LOW;
		return qtrue;
	}
	
	if(atkParry && self->client->ps.fd.forcePowerLevel[FP_SABER_DEFENSE] >= FORCE_LEVEL_3) {
		return qfalse;//Riposte change
	}

	if( self->client->ps.fd.forcePower < DODGE_CRITICALLEVEL )//added by JRHockney to do more heavybounces like old times
	{//heavy slow bounce
		randNum = Q_irand(0, 99);
		if(randNum < 0 || forceMishap)
		{
			mechSelf->doHeavySlowBounce = qtrue;
			return qtrue;
		}
	}
	else if( self->client->ps.saberAttackChainCount <= BALANCE_LOW )
	{//heavy slow bounce
		randNum = Q_irand(0, 99);
		if(randNum < 0 || forceMishap)
		{
			mechSelf->doHeavySlowBounce = qtrue;
			self->client->ps.saberAttackChainCount = BALANCE_HIGH;
			return qtrue;
		}
	}
	else if( self->client->ps.saberAttackChainCount <= BALANCE_HIGH )
	{//slow bounce
		randNum = Q_irand(0, 99);
		if(randNum < 0 || forceMishap)
		{
			mechSelf->doSlowBounce = qtrue;
			self->client->ps.saberAttackChainCount = BALANCE_MAX;
			return qtrue;
		}
	}
	else if( forceMishap )
	{//perform a slow bounce even if we don't have enough mishap for it.
		mechSelf->doSlowBounce = qtrue;
		self->client->ps.saberAttackChainCount = BALANCE_MAX;
		return qtrue;
	}

	return qfalse;
}


//[WeapAccuracy]
extern qboolean GAME_INLINE WalkCheck( gentity_t * self );
void G_AddMishap(gentity_t *self, int amount)
{//mercs don't suffer mishaps, but they do lose/gain MP
	if(!WalkCheck(self)) {
		if(amount > 0) {
			amount *= 2;
		}
		else {
			amount = amount * .5f;
		}
	}

	//G_Printf("%i: %i: %i Mishap Points\n", level.time, self->s.number, amount);

	self->client->ps.saberAttackChainCount -= amount;

	if(self->client->ps.saberAttackChainCount < BALANCE_NONE) {
		self->client->ps.saberAttackChainCount = BALANCE_NONE;
	}
	else if(self->client->ps.saberAttackChainCount > BALANCE_MAX) {
		self->client->ps.saberAttackChainCount = BALANCE_MAX;
	}
}
//[/WeapAccuracy]


extern qboolean saberKnockOutOfHand(gentity_t *saberent, gentity_t *saberOwner, vec3_t velocity);
extern void AnimateKnockdown( gentity_t * self, gentity_t * inflictor );
void G_RollBalance(gentity_t *self, gentity_t *inflictor, qboolean forceMishap)
{//This function is for rolling saber mishaps outside the saber behavior code.
	sabmech_t mechSelf;

	ClearSabMech(&mechSelf);

	if(SabBeh_RollBalance(self, &mechSelf, forceMishap,qfalse))
	{//mishap occurred, animate it
		if(mechSelf.doDisarm)
		{
			//RAFIXME - impliment lose vector handling.
			//ButterFingers(&g_entities[self->client->ps.saberEntityNum], self, inflictor, &tr);
			saberKnockOutOfHand(&g_entities[self->client->ps.saberEntityNum], self, vec3_origin);
		}

		if(mechSelf.doKnockdown)
		{
			AnimateKnockdown(self, inflictor);
		}
		else if (mechSelf.doStun)
		{
			//RAFIXME - impliment impact point properly.
			AnimateStun(self, inflictor, self->client->ps.origin);
		}
		else if(mechSelf.doSlowBounce)
		{
			SabBeh_AnimateSlowBounce(self, inflictor);
		}
		else if(mechSelf.doHeavySlowBounce)
		{
			SabBeh_AnimateHeavySlowBounce(self, inflictor);
		}
	}
}


void SabBeh_AddBalance(gentity_t *self, sabmech_t *mechSelf, int amount, qboolean attack)
{
	if(!WalkCheck(self))
	{//running or moving very fast, can't balance as well
		if(amount > 0)
		{
			amount *= 2;
		}
		else
		{
			amount = amount * .5f;
		}
	}

	//G_Printf("%i: %i: %i Mishap Points\n", level.time, self->s.number, amount);

	self->client->ps.saberAttackChainCount -= amount;

	if(self->client->ps.saberAttackChainCount > BALANCE_MAX)
	{
		self->client->ps.saberAttackChainCount = BALANCE_MAX;
	}
	else if(self->client->ps.saberAttackChainCount < BALANCE_NONE)
	{//overflowing causes a full mishap.
		int randNum = Q_irand(0, 2);
		switch (randNum)
		{
		case 0:
			mechSelf->doDisarm = qtrue;
			break;
		case 1:
			mechSelf->doKnockdown = qtrue;
			break;
		};

		self->client->ps.saberAttackChainCount = BALANCE_LOW;
	}
}


extern qboolean WP_SabersCheckLock( gentity_t *ent1, gentity_t *ent2 );
void SabBeh_AttackVsAttack( gentity_t *self, sabmech_t *mechSelf, 
								gentity_t *otherOwner, sabmech_t *mechOther, qboolean *selfMishap, qboolean *otherMishap )
{//set the saber behavior for two attacking blades hitting each other
	qboolean atkfake = (self->client->ps.userInt3 & (1 << FLAG_ATTACKFAKE)) 
		? qtrue : qfalse;
	qboolean otherfake = (otherOwner->client->ps.userInt3 & (1 << FLAG_ATTACKFAKE)) 
		? qtrue : qfalse;

	if(atkfake && !otherfake)
	{//self is sololy faking
		//set self
		SabBeh_AddBalance(self, mechSelf, MPCOST_ATTACK_FAKE_ATTACKED, qtrue);
#ifdef _DEBUG
		mechSelf->behaveMode = SABBEHAVE_BLOCKFAKED;
#endif

		//set otherOwner
		if (WP_SabersCheckLock(self, otherOwner))
		{	
			self->client->ps.userInt3 |= ( 1 << FLAG_LOCKWINNER );
			self->client->ps.saberBlocked = BLOCKED_NONE;
			otherOwner->client->ps.saberBlocked = BLOCKED_NONE;
		}
		SabBeh_AddBalance(otherOwner, mechOther, MPCOST_ATTACKING_ATTACK_FAKE, qtrue);
#ifdef _DEBUG
		mechOther->behaveMode = SABBEHAVE_ATTACK;
#endif
	}
	else if(otherfake && !atkfake)
	{//only otherOwner is faking
		//set self
		if (WP_SabersCheckLock(otherOwner, self))
		{	
			self->client->ps.saberBlocked = BLOCKED_NONE;
			otherOwner->client->ps.userInt3 |= ( 1 << FLAG_LOCKWINNER );
			otherOwner->client->ps.saberBlocked = BLOCKED_NONE;
		}
		SabBeh_AddBalance(self, mechSelf, MPCOST_ATTACKING_ATTACK_FAKE, qtrue);
#ifdef _DEBUG
		mechSelf->behaveMode = SABBEHAVE_ATTACK;
#endif

		//set otherOwner
		SabBeh_AddBalance(otherOwner, mechOther, MPCOST_ATTACK_FAKE_ATTACKED, qtrue);
#ifdef _DEBUG
		mechOther->behaveMode = SABBEHAVE_BLOCKFAKED;
#endif
	}
	else
	{//either both are faking or neither is faking.  Either way, it's cancelled out
		//set self
		SabBeh_AddBalance(self, mechSelf, MPCOST_ATTACK_ATTACK, qtrue);
#ifdef _DEBUG
		mechSelf->behaveMode = SABBEHAVE_ATTACK;
#endif

		//set otherOwner
		SabBeh_AddBalance(otherOwner, mechOther, MPCOST_ATTACK_ATTACK, qtrue);
#ifdef _DEBUG
		mechOther->behaveMode = SABBEHAVE_ATTACK;
#endif
	}

	if(g_debugsaberbehavior.integer)
	{
		G_Printf("%i: SaberBeh Attack vs Attack: %i (self %s:%s): %s, %i (other %s:%s): %s\n", 
			level.time, self->s.number, GetStringForID(SaberMoveTable, self->playerState->saberMove), GetStringForID(SaberBlockedTable, self->playerState->saberBlocked), GetStringForID(sabBehaveTable, mechSelf->behaveMode), 
			otherOwner->s.number, GetStringForID(SaberMoveTable, otherOwner->playerState->saberMove), GetStringForID(SaberBlockedTable, otherOwner->playerState->saberBlocked), GetStringForID(sabBehaveTable, mechOther->behaveMode));
	}
}

extern bot_state_t *botstates[MAX_CLIENTS];
extern qboolean BG_SuperBreakWinAnim( int anim );
extern stringID_table_t animTable [MAX_ANIMATIONS+1];
extern qboolean BG_InSlowBounce(playerState_t *ps);
extern qboolean G_InAttackParry(gentity_t *self);
extern int OJP_SaberBlockCost(gentity_t *defender, gentity_t *attacker, vec3_t hitLoc);
extern void WP_SaberBlockNonRandom( gentity_t *self, vec3_t hitloc, qboolean missileBlock );
extern qboolean G_BlockIsParry( gentity_t *self, gentity_t *attacker, vec3_t hitLoc );
extern qboolean G_BlockIsQuickParry( gentity_t *self, gentity_t *attacker, vec3_t hitLoc );
extern void BG_AddFatigue( playerState_t * ps, int Fatigue);
void SabBeh_AttackVsBlock( gentity_t *attacker, sabmech_t *mechAttacker, 
								gentity_t *blocker, sabmech_t *mechBlocker, vec3_t hitLoc, qboolean hitSaberBlade,
								qboolean *attackerMishap, qboolean *blockerMishap)
{//set the saber behavior for an attacking vs blocking/parrying blade impact
	qboolean startSaberLock = qfalse;
	qboolean parried = G_BlockIsParry(blocker, attacker, hitLoc);
	
	qboolean atkparry = G_InAttackParry(blocker);
	qboolean atkfake = (attacker->client->ps.userInt3 & (1 << FLAG_ATTACKFAKE)) 
		? qtrue : qfalse;

	G_BlockIsQuickParry(blocker,attacker,hitLoc);//Return value was never used, so I just call it now
	if(parried && blocker->r.svFlags & SVF_BOT 
		&& BOT_ATTACKPARRYRATE * botstates[blocker->s.number]->settings.skill > Q_irand(0,999))
	{//bot performed an attack parry (by cheating a bit)
		//G_Printf("%i: %i: Bot Cheat Attack Parried\n", level.time, blocker->s.number);
		atkparry = qtrue;
	}

	/*
	if(parried && atkparry)
	{
		G_Printf("%i: %i: Attack Parried\n", level.time, blocker->s.number);
	}
	*/

	if(BG_SuperBreakWinAnim(attacker->client->ps.torsoAnim))
	{//attacker was attempting a superbreak and he hit someone who could block the move, rail him for screwing up.
		*attackerMishap = SabBeh_RollBalance(attacker, mechAttacker, qtrue,qfalse);
		SabBeh_AddBalance(attacker, mechAttacker, MPCOST_BLOCKED_SUPERBREAK, qtrue);
#ifdef _DEBUG
			mechAttacker->behaveMode = SABBEHAVE_ATTACKPARRIED;
#endif

		SabBeh_AddBalance(blocker, mechBlocker, MPCOST_BLOCKING_SUPERBREAK, qfalse);
#ifdef _DEBUG
			mechBlocker->behaveMode = SABBEHAVE_BLOCK;
#endif
	}
	else if(atkfake)
	{//attacker faked before making this attack, treat like standard attack/attack
		if(parried)
		{//defender parried the attack fake.
			*attackerMishap = SabBeh_RollBalance(attacker, mechAttacker, atkparry,qtrue);
			SabBeh_AddBalance(attacker, mechAttacker, MPCOST_PARRIED_ATTACKFAKE, qtrue);
#ifdef _DEBUG
			mechAttacker->behaveMode = SABBEHAVE_ATTACK;
#endif
			//DMURPHY
			if (blocker->client->pers.cmd.buttons & BUTTON_SPECIALBUTTON2 //(was Button_15)
				&& blocker->client->ps.fd.forcePowerLevel[FP_SABER_DEFENSE] >= FORCE_LEVEL_2)
			{
				attacker->client->ps.userInt3 |= (1 << FLAG_QUICKPARRY);
			}
			else
			{
			attacker->client->ps.userInt3 |= ( 1 << FLAG_PARRIED );
			}

			SabBeh_AddBalance(blocker, mechBlocker, MPCOST_PARRYING_ATTACKFAKE, qfalse);
#ifdef _DEBUG
			mechBlocker->behaveMode = SABBEHAVE_BLOCK;
#endif
		}
		else if(atkparry)
		{
			saberKnockOutOfHand(&g_entities[attacker->client->ps.saberEntityNum], attacker, vec3_origin);
		}
		else
		{//otherwise, the defender stands a good chance of having his defensives broken.	
			SabBeh_AddBalance(attacker, mechAttacker, MPCOST_BLOCKED_ATTACK_FAKE, qtrue);

			if(attacker->client->ps.fd.saberAnimLevel == SS_DESANN)
			{
				SabBeh_AddBalance(blocker, mechBlocker, MPCOST_BLOCKING_DESANN_ATTACK_FAKE, qfalse);
			}
			else
			{//standard attacker
				SabBeh_AddBalance(blocker, mechBlocker, MPCOST_BLOCKING_ATTACK_FAKE, qfalse);
			}

#ifdef _DEBUG
			mechAttacker->behaveMode = SABBEHAVE_ATTACK;
#endif
			if (WP_SabersCheckLock(attacker, blocker))
			{	
				attacker->client->ps.userInt3 |= ( 1 << FLAG_LOCKWINNER );
				attacker->client->ps.saberBlocked = BLOCKED_NONE;
				blocker->client->ps.saberBlocked = BLOCKED_NONE;
				startSaberLock = qtrue;
			}
			
#ifdef _DEBUG
			mechBlocker->behaveMode = SABBEHAVE_BLOCKFAKED;
#endif
		}

	}
	else if(hitSaberBlade && BG_InSlowBounce(&blocker->client->ps) 
		&& blocker->client->ps.userInt3 & (1 << FLAG_OLDSLOWBOUNCE)
		&& attacker->client->ps.fd.saberAnimLevel == SS_TAVION)
	{//blocker's saber was directly hit while in a slow bounce, disarm the blocker!
		mechBlocker->doDisarm = qtrue;
		blocker->client->ps.saberAttackChainCount = BALANCE_HIGH;
#ifdef _DEBUG
		mechBlocker->behaveMode = SABBEHAVE_BLOCKFAKED;
#endif

		//set attacker
		SabBeh_AddBalance(attacker, mechAttacker, MPCOST_TAVION_ATTACK_SLOW_BOUNCE, qtrue);
#ifdef _DEBUG
		mechAttacker->behaveMode = SABBEHAVE_ATTACK;
#endif
	}
	else
	{//standard attack
		//set blocker
#ifdef _DEBUG
		mechBlocker->behaveMode = SABBEHAVE_BLOCK;
#endif

		//set attacker
		if(parried)
		{
		//parry values
			if(attacker->client->ps.saberMove == LS_A_LUNGE
			|| attacker->client->ps.saberMove == LS_SPINATTACK
			|| attacker->client->ps.saberMove == LS_SPINATTACK_DUAL)
			{//attacker's lunge was parried, force mishap.
				*attackerMishap = SabBeh_RollBalance(attacker, mechAttacker, qtrue,atkparry);
			}
			else
			{
				*attackerMishap = SabBeh_RollBalance(attacker, mechAttacker, atkparry,atkparry);
			}
			SabBeh_AddBalance(attacker, mechAttacker, MPCOST_PARRIED, qtrue);
#ifdef _DEBUG
			mechAttacker->behaveMode = SABBEHAVE_ATTACKPARRIED;
#endif
			//[QuickParry]
			
			if (blocker->client->pers.cmd.buttons & BUTTON_SPECIALBUTTON2 //Was Button 15 DMURPHY
				&& blocker->client->ps.fd.forcePowerLevel[FP_SABER_DEFENSE] >= FORCE_LEVEL_2)
			{
				attacker->client->ps.userInt3 |= ( 1 << FLAG_QUICKPARRY);
			}
			else
			{
				attacker->client->ps.userInt3 |= ( 1 << FLAG_PARRIED );
			}
			//[/QuickParry]

			SabBeh_AddBalance(blocker, mechBlocker, MPCOST_PARRYING, qfalse);
			
		}
		else
		{//blocked values
			SabBeh_AddBalance(attacker, mechAttacker, MPCOST_BLOCKED_ATTACK, qtrue);
			if(attacker->client->ps.fd.saberAnimLevel == SS_TAVION)
			{//aqua styles deals MP to players that don't parry it.
				SabBeh_AddBalance(blocker, mechBlocker, MPCOST_BLOCKING_TAVION_ATTACK, qfalse);
			}
			else if(attacker->client->ps.fd.saberAnimLevel == SS_STRONG)
			{
				blocker->client->ps.fd.forcePower -= FPCOST_BLOCKING_STRONG_ATTACK;
				SabBeh_AddBalance(blocker, mechBlocker, MPCOST_BLOCKING_ATTACK, qfalse);
			}
			//racc - This doesn't actually ever happen. :S
			else if(attacker->client->ps.fd.saberAnimLevel==SS_TAVION
				&& attacker->client->skillLevel[SK_GREENSTYLE] == FORCE_LEVEL_3
				&& (attacker->client->ps.userInt3 & FLAG_QUICKPARRY))
			{
				SabBeh_AddBalance(blocker, mechBlocker, MPCOST_QUICKPARRY_TAVION_ATTACK, qfalse);
			}
			else
			{//standard blocker cost
				SabBeh_AddBalance(blocker, mechBlocker, MPCOST_BLOCKING_ATTACK, qfalse);
			}
			
#ifdef _DEBUG
			mechAttacker->behaveMode = SABBEHAVE_ATTACKBLOCKED;
#endif
		}
	}

	if(!OnSameTeam(attacker, blocker) || g_friendlySaber.integer)
	{//don't do parries or charge/regen DP unless we're in a situation where we can actually hurt the target.
		if(parried)
		{//parries don't cost any DP and they have a special animation
			//qboolean regenSound = qfalse;
			mechBlocker->doParry = qtrue;
		}
		else if(!startSaberLock)
		{//normal saber blocks
			//update the blocker's block move
			blocker->client->ps.saberLockFrame = 0; //break out of saberlocks.
			WP_SaberBlockNonRandom(blocker, hitLoc, qfalse);
		}
	}

	//do saber DP cost.

	/*
	// debugger message.
	G_Printf("%i: %i: Saber Block Cost: %i atk: %s %s blk: %s %s\n", level.time, blocker->s.number, OJP_SaberBlockCost(blocker, attacker, hitLoc), 
		GetStringForID( animTable, attacker->client->ps.torsoAnim ), GetStringForID( SaberMoveTable, attacker->client->ps.saberMove ), 
		GetStringForID( animTable, blocker->client->ps.torsoAnim ), GetStringForID( SaberMoveTable, blocker->client->ps.saberMove ) );
	*/

	//[ExpSys] -- [DUALRAWR]
	G_DodgeDrain(blocker, attacker, OJP_SaberBlockCost(blocker, attacker, hitLoc));
	//[/ExpSys]

	//costs FP as well.
	BG_AddFatigue(&blocker->client->ps, 1);

	if(g_debugsaberbehavior.integer)
	{
		G_Printf("%i: SaberBeh Attack vs Block: %i (attacker %s:%s): %s, %i (blocker %s:%s): %s\n", 
			level.time, attacker->s.number, GetStringForID(SaberMoveTable, attacker->playerState->saberMove), GetStringForID(SaberBlockedTable, attacker->playerState->saberBlocked), GetStringForID(sabBehaveTable, mechAttacker->behaveMode), 
			blocker->s.number, GetStringForID(SaberMoveTable, blocker->playerState->saberMove), GetStringForID(SaberBlockedTable, blocker->playerState->saberBlocked), GetStringForID(sabBehaveTable, mechBlocker->behaveMode));
	}
}


extern int OJP_SaberCanBlock(gentity_t *self, gentity_t *atk, qboolean checkBBoxBlock, vec3_t point, int rSaberNum, int rBladeNum);
void SabBeh_RunSaberBehavior( gentity_t *self, sabmech_t *mechSelf, 
								gentity_t *otherOwner, sabmech_t *mechOther, vec3_t hitLoc, 
								qboolean *didHit, qboolean otherHitSaberBlade )
{	
	qboolean selfMishap = qfalse;
	qboolean otherMishap = qfalse;

	//initalize the sab mechanic data.
	ClearSabMech(mechSelf);
	ClearSabMech(mechOther);

	if(!otherOwner)
	{//not a saber-on-saber hit, no mishap handling.
		return;
	}

	//G_Printf("BG_SaberInNonIdleDamageMove\n");
	if(BG_SaberInNonIdleDamageMove(&self->client->ps, self->localAnimIndex) )
	{//self is attacking
		//G_Printf("(y)\n");
		if(BG_SaberInNonIdleDamageMove(&otherOwner->client->ps, otherOwner->localAnimIndex)) 
		{//and otherOwner is attacking
			SabBeh_AttackVsAttack(self, mechSelf, otherOwner, mechOther, &selfMishap, &otherMishap);
		}
		else if(OJP_SaberCanBlock(otherOwner, self, qfalse, hitLoc, -1, -1))
		{//and otherOwner is blocking or parrying
			//this is called with dual with both sabers[DUALRAWR]
			SabBeh_AttackVsBlock(self, mechSelf, otherOwner, mechOther, hitLoc, otherHitSaberBlade,
				&selfMishap, &otherMishap);
			*didHit = qfalse;
		}
		else
		{//otherOwner in some other state
			//no mishaps for this at the moment.
		}
	}
	else if( OJP_SaberCanBlock(self, otherOwner, qfalse, hitLoc, -1, -1) )
	{//self is blocking or parrying
		if(BG_SaberInNonIdleDamageMove(&otherOwner->client->ps, otherOwner->localAnimIndex))
		{//and otherOwner is attacking
			SabBeh_AttackVsBlock(otherOwner, mechOther, self, mechSelf, hitLoc, qtrue,
				&otherMishap, &selfMishap);
		}
		else if(OJP_SaberCanBlock(otherOwner, self, qfalse, hitLoc, -1, -1))
		{//and otherOwner is blocking or parrying
		}
		else
		{//otherOwner in some other state
			//no mishaps for this at the moment.
		}
	}
	else
	{//whatever other states self can be in.  (returns, bounces, or something)
		//just act like no mishaps can happen
	}
}


extern void NPC_Pain(gentity_t *self, gentity_t *attacker, int damage);
void SabBeh_AnimateSlowBounce(gentity_t* self, gentity_t *inflictor)
{
	self->client->ps.userInt3 |= ( 1 << FLAG_SLOWBOUNCE );
	if ( self->s.number < MAX_CLIENTS )
	{
		G_AddEvent( self, Q_irand(EV_PUSHED1, EV_PUSHED3), 0 );
	}
	else
	{//npc
		NPC_Pain( self, inflictor, 0 );
	}
	self->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;
}


void SabBeh_AnimateHeavySlowBounce(gentity_t* self, gentity_t *inflictor)
{
	self->client->ps.userInt3 |= ( 1 << FLAG_SLOWBOUNCE );
	self->client->ps.userInt3 |= ( 1 << FLAG_OLDSLOWBOUNCE );
	if ( self->s.number < MAX_CLIENTS )
	{
		G_AddEvent( self, Q_irand(EV_PUSHED1, EV_PUSHED3), 0 );
	}
	else
	{//npc
		NPC_Pain( self, inflictor, 0 );
	}
	self->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;
}


//[SaberLockSys]
qboolean SabBeh_ButtonforSaberLock(gentity_t* self)
{//checks to see if the player is pressing the correct direction to advance the saber lock for this saber lock animation.

	//must be holding down alt-attack
	if(!(self->client->pers.cmd.buttons & BUTTON_ALT_ATTACK))
	{
		return qfalse;
	}

	switch(self->client->ps.torsoAnim)
	{
	//top attacks
	case BOTH_BF1LOCK:
	case BOTH_BF2LOCK:
	case BOTH_LK_S_DL_T_L_1:
	case BOTH_LK_S_ST_T_L_1:
	case BOTH_LK_S_S_T_L_1:
	case BOTH_LK_DL_DL_T_L_1:
	case BOTH_LK_DL_ST_T_L_1:
	case BOTH_LK_DL_S_T_L_1:
	case BOTH_LK_ST_DL_T_L_1:
	case BOTH_LK_ST_ST_T_L_1:
	case BOTH_LK_ST_S_T_L_1:
		if(self->client->pers.cmd.forwardmove > 0)
		{
			return qtrue;
		}
		break;
	//right
	case BOTH_CWCIRCLELOCK:
	case BOTH_CCWCIRCLELOCK:
	case BOTH_LK_S_DL_S_L_1:
	case BOTH_LK_S_ST_S_L_1:
	case BOTH_LK_S_S_S_L_1:
	case BOTH_LK_DL_DL_S_L_1:
	case BOTH_LK_DL_ST_S_L_1:
	case BOTH_LK_DL_S_S_L_1:
	case BOTH_LK_ST_DL_S_L_1:
	case BOTH_LK_ST_ST_S_L_1:
	case BOTH_LK_ST_S_S_L_1:
		if(self->client->pers.cmd.rightmove > 0)
		{
			return qtrue;
		}
		break;
	default:
		return qfalse;
	};

	return qfalse;
}
//[/SaberLockSys]


//[SaberSys]
void BG_ReduceMishapLevel(playerState_t *ps)
{//reduces a player's mishap meter by one level
	if(ps->saberAttackChainCount <= BALANCE_NONE)
	{
		ps->saberAttackChainCount = BALANCE_LOW;
	}
	else if(ps->saberAttackChainCount <= BALANCE_LOW)
	{
		ps->saberAttackChainCount = BALANCE_HIGH;
	}
	else if(ps->saberAttackChainCount <= BALANCE_HIGH)
	{
		ps->saberAttackChainCount = BALANCE_MAX;
	}
	else
	{
		ps->saberAttackChainCount = BALANCE_MAX;
	}

}
//[/SaberSys]
