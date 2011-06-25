//[JKEHud]
//	--	cg_jkehud.cpp	--
//	--	JKE HUD code	--


//	--	Include some files	--

#include "cg_local.h"

#include "bg_saga.h"

#include "../ui/ui_shared.h"
#include "../ui/ui_public.h"



//	--	Define some shit	--

#define MAX_HUD_TICS 8

const float FPBAR_H		= 7.0f;
const float FPBAR_W		= 65.0f;
const float FPBAR_X		= 537.0f;
const float FPBAR_Y		= 420.0f;

#define DPBAR_H			65.0f
#define DPBAR_W			13.0f
#define DPBAR_X			538.0f
#define DPBAR_Y			367.0f



//	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--
//	  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
//	--	--	--	--	--	START OF TIC METHOD	--	--	--	--	--	--	--
//	  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
//	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--

const char *armorTicName[MAX_HUD_TICS] = 
{
"armor_tic1",	"armor_tic2", 
"armor_tic3",	"armor_tic4",
"armor_tic5",	"armor_tic6",
"armor_tic7",	"armor_tic8",
};
const char *forceTicName[MAX_HUD_TICS] = 
{
"force_tic1",	"force_tic2", 
"force_tic3",	"force_tic4",
"force_tic5",	"force_tic6",
"force_tic7",	"force_tic8",
};
const char *dodgeTicName[MAX_HUD_TICS] = 
{
"dodge_tic1",	"dodge_tic2", 
"dodge_tic3",	"dodge_tic4",
"dodge_tic5",	"dodge_tic6",
"dodge_tic7",	"dodge_tic8",
};
const char *ammoTicName[MAX_HUD_TICS] = 
{
"ammo_tic1",	"ammo_tic2", 
"ammo_tic3",	"ammo_tic4",
"ammo_tic5",	"ammo_tic6",
"ammo_tic7",	"ammo_tic8",
};
const char *mishapTicName[] = 
{
"mishap_tic1",	"mishap_tic2", 
"mishap_tic3",	"mishap_tic4", 
"mishap_tic5",	"mishap_tic6",
"mishap_tic7",	"mishap_tic8",
"mishap_tic9",	"mishap_tic10",
"mishap_tic11",	"mishap_tic12",
"mishap_tic13",	"mishap_tic14",
"mishap_tic15",
};

//Health
void CG_DrawHealthTicMethod(menuDef_t *menuHUD)
{
	vec4_t			calcColor;
	playerState_t	*ps;
	int				healthAmt;
	itemDef_t		*focusItem;

	// Can we find the menu?
	if (!menuHUD)
	{
		return;
	}

	ps = &cg.snap->ps;

	// What's the health?
	healthAmt = ps->stats[STAT_HEALTH];
	if (healthAmt > ps->stats[STAT_MAX_HEALTH])
	{
		healthAmt = ps->stats[STAT_MAX_HEALTH];
	}

	focusItem = Menu_FindItemByName(menuHUD, "health_tic1");

	/*if (focusItem)
	{
		if (healthAmt > 0)
		{
			calcColor[0] = calcColor[1] = calcColor[2] = calcColor[3] = 1;

			if (healthAmt < ps->stats[STAT_MAX_HEALTH])
			{
				calcColor[1] = calcColor[2] = (float) healthAmt / (float) ps->stats[STAT_MAX_HEALTH];
			}

			trap_R_SetColor( calcColor );

			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}
	}*/

	// Print the mueric amount
	focusItem = Menu_FindItemByName(menuHUD, "healthamount");
	if (focusItem)
	{// Print health amount
		trap_R_SetColor( focusItem->window.foreColor );	

		CG_DrawNumField (
			focusItem->window.rect.x, 
			focusItem->window.rect.y, 
			3, 
			ps->stats[STAT_HEALTH], 
			focusItem->window.rect.w, 
			focusItem->window.rect.h, 
			NUM_FONT_SMALL,
			qfalse);
	}
}


//Armor
void CG_DrawArmorTicMethod(menuDef_t *menuHUD)
{
	vec4_t			calcColor;
	playerState_t	*ps;
	int				armor, maxArmor;
	itemDef_t		*focusItem;
	float			percent,quarterArmor;
	int				i,currValue,inc;

	calcColor[0] = 0;
	calcColor[1] = 0;
	calcColor[2] = 0;
	calcColor[3] = 0;
	ps = &cg.predictedPlayerState;

	if (!menuHUD)
	{// Can we find the menu?
		return;
	}

	focusItem = Menu_FindItemByName(menuHUD, "armorshell");

	if (focusItem)
	{
		CG_DrawPic( 
			focusItem->window.rect.x,
			focusItem->window.rect.y,
			focusItem->window.rect.w, 
			focusItem->window.rect.h, 
			focusItem->window.background
			);
	}

	armor = ps->stats[STAT_ARMOR];
	maxArmor = ps->stats[STAT_MAX_HEALTH];

	if (armor > maxArmor)
	{
		armor = maxArmor;
	}

	currValue = armor;
	inc = (float) maxArmor / MAX_HUD_TICS;

	for (i=(MAX_HUD_TICS-1);i>=0;i--)
	{
		focusItem = Menu_FindItemByName(menuHUD, armorTicName[i]);

		if (!focusItem)
		{
			continue;
		}

		if (currValue <= 0)
		{// don't show tic
			break;
		}
		else if (currValue < inc)
		{// partial tic (alpha it out)
			percent = (float) currValue / inc;
			calcColor[3] *= percent;
		}

		trap_R_SetColor( calcColor);

		if ((i==(MAX_HUD_TICS-1)) && (currValue < inc))
		{
			if (cg.HUDArmorFlag)
			{
				CG_DrawPic( 
					focusItem->window.rect.x,
					focusItem->window.rect.y,
					focusItem->window.rect.w, 
					focusItem->window.rect.h, 
					focusItem->window.background
					);
			}
		}
		else 
		{
			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}

		currValue -= inc;
	}

	// If armor is low, flash a graphic to warn the player
	if (armor)
	{// Is there armor? Draw the HUD Armor TIC
		quarterArmor = (float) (ps->stats[STAT_MAX_HEALTH] / 4.0f);

		// Make tic flash if armor is at 25% of full armor
		if (ps->stats[STAT_ARMOR] < quarterArmor)
		{// Do whatever the flash timer says
			if (cg.HUDTickFlashTime < cg.time)
			{// Flip at the same time
				cg.HUDTickFlashTime = cg.time + 400;
				if (cg.HUDArmorFlag)
				{
					cg.HUDArmorFlag = qfalse;
				}
				else
				{
					cg.HUDArmorFlag = qtrue;
				}
			}
		}
		else
		{
			cg.HUDArmorFlag=qtrue;
		}
	}
	else
	{// No armor? Don't show it.
		cg.HUDArmorFlag=qfalse;
	}

	focusItem = Menu_FindItemByName(menuHUD, "armoramount");

	if (focusItem)
	{// Print armor amount
		trap_R_SetColor( focusItem->window.foreColor );	

		CG_DrawNumField (
			focusItem->window.rect.x, 
			focusItem->window.rect.y, 
			3, 
			armor, 
			focusItem->window.rect.w, 
			focusItem->window.rect.h, 
			NUM_FONT_SMALL,
			qfalse);
	}
}


//Balance
void CG_DrawBalanceTicMethod(centity_t *cent, menuDef_t *menuHUD)
{
	itemDef_t		*focusItem;
	int				i;

	if (!menuHUD)
	{//Can we find the menu?
		return;
	}

	for (i = cg.snap->ps.saberAttackChainCount-1; i >= 0; i--)
	{
		focusItem = Menu_FindItemByName(menuHUD, mishapTicName[i]);

		if (focusItem)
		{
			CG_DrawPic( 
					focusItem->window.rect.x,
					focusItem->window.rect.y,
					focusItem->window.rect.w, 
					focusItem->window.rect.h, 
					focusItem->window.background
					);
		}


	}
}

//Ammo
void CG_DrawAmmoTicMethod(centity_t *cent, menuDef_t *menuHUD)
{
	playerState_t	*ps;
	int				i;
	vec4_t			calcColor;
	float			value,inc = 0.0f,percent;
	itemDef_t		*focusItem;

	ps = &cg.snap->ps;

	if (!menuHUD)
	{// Can we find the menu?
		return;
	}

	if (!cent->currentState.weapon )
	{// We don't have a weapon right now
		return;
	}

	value = ps->ammo[weaponData[cent->currentState.weapon].ammoIndex];
	if (value < 0)
	{// No ammo
		return;
	}

	focusItem = Menu_FindItemByName(menuHUD, "ammoamount");

	if (weaponData[cent->currentState.weapon].energyPerShot == 0 &&
		weaponData[cent->currentState.weapon].altEnergyPerShot == 0)
	{//just draw "infinite"
		inc = 8 / MAX_HUD_TICS;
		value = 8;

		focusItem = Menu_FindItemByName(menuHUD, "ammoinfinite");
		if (focusItem)
		{
			UI_DrawProportionalString(focusItem->window.rect.x, focusItem->window.rect.y, "--", NUM_FONT_SMALL, focusItem->window.foreColor);
		}
	}
	else
	{
		focusItem = Menu_FindItemByName(menuHUD, "ammoamount");
		if (focusItem)
		{

			if ( (cent->currentState.eFlags & EF_DOUBLE_AMMO) )
			{
				inc = (float) (ammoData[weaponData[cent->currentState.weapon].ammoIndex].max*2.0f) / MAX_HUD_TICS;
			}
			else
			{
				inc = (float) ammoData[weaponData[cent->currentState.weapon].ammoIndex].max / MAX_HUD_TICS;
			}
			value = ps->ammo[weaponData[cent->currentState.weapon].ammoIndex];

			CG_DrawStringExt(
				focusItem->window.rect.x, 
				focusItem->window.rect.y,
				va("%i",(int)value),
				0,
				qfalse,
				qfalse,
				focusItem->window.rect.w, 
				focusItem->window.rect.h,
				8);
		}
	}

	for (i=MAX_HUD_TICS-1;i>=0;i--)
	{// Draw tics
		focusItem = Menu_FindItemByName(menuHUD, ammoTicName[i]);

		if (!focusItem)
		{
			continue;
		}

		if ( value <= 0 )
		{// done
			break;
		}
		else if (value < inc)
		{// partial tic
			percent = value / inc;
			calcColor[3] = percent;
		}

		CG_DrawPic( 
			focusItem->window.rect.x,
			focusItem->window.rect.y,
			focusItem->window.rect.w, 
			focusItem->window.rect.h, 
			focusItem->window.background
			);

		value -= inc;
	}

}


//Saber Style
void CG_DrawSaberStyleTicMethod(centity_t *cent, menuDef_t *menuHUD)
{
	itemDef_t		*focusItem;

	if (!cent->currentState.weapon )
	{// We don't have a weapon right now
		return;
	}

	if ( cent->currentState.weapon != WP_SABER )
	{
		return;
	}

	if (!menuHUD)
	{// Can we find the menu?
		return;
	}


	switch ( cg.predictedPlayerState.fd.saberDrawAnimLevel )
	{
	//Fast
	case 1:
		focusItem = Menu_FindItemByName(menuHUD, "saberstyle_fast");
		if (focusItem)
		{
			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}

		break;

	//Medium
	case 2:
		focusItem = Menu_FindItemByName(menuHUD, "saberstyle_medium");
		if (focusItem)
		{
			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}
		break;

	//Strong
	case 3:
		focusItem = Menu_FindItemByName(menuHUD, "saberstyle_slow");
		if (focusItem)
		{
			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}
		break;

	//Desann
	case 4:
		focusItem = Menu_FindItemByName(menuHUD, "saberstyle_desann");
		if (focusItem)
		{
			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}
		break;

	//Tavion
	case 5:
		focusItem = Menu_FindItemByName(menuHUD, "saberstyle_tavion");
		if (focusItem)
		{
			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}
		break;

	//Dual
	case 6:
		focusItem = Menu_FindItemByName(menuHUD, "saberstyle_dual");
		if (focusItem)
		{
			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}
		break;

	//Staff
	case 7:
		focusItem = Menu_FindItemByName(menuHUD, "saberstyle_staff");
		if (focusItem)
		{
			CG_DrawPic( 
				focusItem->window.rect.x,
				focusItem->window.rect.y,
				focusItem->window.rect.w, 
				focusItem->window.rect.h, 
				focusItem->window.background
				);
		}
		break;
	}

}


//Force
void CG_DrawForcePower( menuDef_t *menuHUD )
{
	vec4_t			aColor;
	vec4_t			cColor;
	itemDef_t		*focusItem;
	float			percent = ((float)cg.snap->ps.fd.forcePower / 100.0f) * FPBAR_W;
	
	//color of the bar
	aColor[0] = 0.503f;
	aColor[1] = 0.375f;
	aColor[2] = 0.996f;
	aColor[3] = 0.5f;

	if (cg.forceHUDTotalFlashTime > cg.time || (cg_entities[cg.snap->ps.clientNum].currentState.userInt3 &  ( 1 << FLAG_FATIGUED)))
	{//color of the bar
		aColor[0] = 1.0f;
		aColor[1] = 0.0f;
		aColor[2] = 0.0f;
		aColor[3] = 0.5f;
		if (cg.forceHUDNextFlashTime < cg.time)	
		{
			cg.forceHUDNextFlashTime = cg.time + 400;
			trap_S_StartSound (NULL, 0, CHAN_LOCAL, cgs.media.noforceSound );
		}
	}
	else
	{// turn HUD back on if it had just finished flashing time.
		cg.forceHUDNextFlashTime = 0;
	}


	if (percent > FPBAR_W)
	{
		return;
	}

	if (percent < 0.1f)
	{
		percent = 0.1f;
	}

	Vector4Copy(colorTable[CT_BLACK], cColor);
	cColor[3] = 0.4f;

	CG_DrawRect(FPBAR_X - 1.0f, FPBAR_Y - 1.0f, FPBAR_W + 2.1f, FPBAR_H + 2.1f, 1.0f, colorTable[CT_BLACK]);
	CG_FillRect(FPBAR_X + ( FPBAR_W- percent), FPBAR_Y, FPBAR_W-(FPBAR_W-percent), FPBAR_H, aColor);
	CG_FillRect((FPBAR_X), FPBAR_Y, (FPBAR_W)-percent, FPBAR_H, cColor);

	//CG_DrawRect(FPBAR_X - 1.0f, FPBAR_Y - 1.0f, FPBAR_W + 2.1f, FPBAR_H + 2.1f, 1.0f, colorTable[CT_BLACK]);
	//CG_FillRect(FPBAR_X, FPBAR_Y, FPBAR_W-(FPBAR_W-percent), FPBAR_H, aColor);
	//CG_FillRect((FPBAR_X + percent), FPBAR_Y, (FPBAR_W)-percent, FPBAR_H, cColor);

	//CG_DrawRect(FPBAR_X - 1.0f, FPBAR_Y - 1.0f, FPBAR_W + 2.0f, FPBAR_H + 2.1f, 1.0f, colorTable[CT_BLACK]);
	//CG_FillRect(FPBAR_X, FPBAR_Y+(FPBAR_H-percent), FPBAR_W, FPBAR_H-(FPBAR_H-percent), aColor);
	
	//CG_FillRect(FPBAR_X, FPBAR_Y, FPBAR_W, FPBAR_H-percent, cColor);
	
	focusItem = Menu_FindItemByName(menuHUD, "forceamount");

	if (focusItem)
	{// Print force amount
		trap_R_SetColor( colorTable[CT_WHITE] );	

		CG_DrawNumField (
			FPBAR_X - 19.0f, 
			FPBAR_Y + 0.3f, 
			3, 
			cg.snap->ps.fd.forcePower, 
			4, 
			7, 
			NUM_FONT_SMALL,
			qfalse);
	}
}

//	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--
//	  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
//	--	--	--	--	--	END OF TIC METHOD	--	--	--	--	--	--	--
//	  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
//	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--


//Draw the actual HUD
void JKEHUD(centity_t *cent)
{
	menuDef_t	*menuHUD = NULL;
	itemDef_t	*focusItem = NULL;
	const char *scoreStr = NULL;
	int	scoreBias;
	char scoreBiasStr[16];

	if (cg_hudFiles.integer == 1)
	{//Tic method
		// Draw the left HUD 
		menuHUD = Menus_FindByName("lefthud");

		if (menuHUD)
		{
			itemDef_t *focusItem;

			// Print frame
			/*focusItem = Menu_FindItemByName(menuHUD, "frame");
			if (focusItem)
			{
				CG_DrawPic( 
					focusItem->window.rect.x, 
					focusItem->window.rect.y, 
					focusItem->window.rect.w, 
					focusItem->window.rect.h, 
					focusItem->window.background 
					);			
			}*/

			if (cg.predictedPlayerState.pm_type != PM_SPECTATOR)
			{
				CG_DrawArmorTicMethod(menuHUD);
				CG_DrawHealthTicMethod(menuHUD);
			}
		}
		else
		{ 
			//CG_Error("CG_ChatBox_ArrayInsert: unable to locate HUD menu file ");
		}

		//scoreStr = va("Score: %i", cgs.clientinfo[cg.snap->ps.clientNum].score);
		if ( cgs.gametype == GT_DUEL )
		{//A duel that requires more than one kill to knock the current enemy back to the queue
			//show current kills out of how many needed
			scoreStr = va("%s: %i/%i", CG_GetStringEdString("MP_INGAME", "SCORE"), cg.snap->ps.persistant[PERS_SCORE], cgs.fraglimit);
		}
		else if (0 && cgs.gametype < GT_TEAM )
		{	// This is a teamless mode, draw the score bias.
			scoreBias = cg.snap->ps.persistant[PERS_SCORE] - cgs.scores1;
			if (scoreBias == 0)
			{	// We are the leader!
				if (cgs.scores2 <= 0)
				{	// Nobody to be ahead of yet.
					Com_sprintf(scoreBiasStr, sizeof(scoreBiasStr), "");
				}
				else
				{
					scoreBias = cg.snap->ps.persistant[PERS_SCORE] - cgs.scores2;
					if (scoreBias == 0)
					{
						Com_sprintf(scoreBiasStr, sizeof(scoreBiasStr), " (Tie)");
					}
					else
					{
						Com_sprintf(scoreBiasStr, sizeof(scoreBiasStr), " (+%d)", scoreBias);
					}
				}
			}
			else // if (scoreBias < 0)
			{	// We are behind!
				Com_sprintf(scoreBiasStr, sizeof(scoreBiasStr), " (%d)", scoreBias);
			}
			scoreStr = va("%s: %i%s", CG_GetStringEdString("MP_INGAME", "SCORE"), cg.snap->ps.persistant[PERS_SCORE], scoreBiasStr);
		}
		else
		{	// Don't draw a bias.
			scoreStr = va("%s: %i", CG_GetStringEdString("MP_INGAME", "SCORE"), cg.snap->ps.persistant[PERS_SCORE]);
		}

		menuHUD = Menus_FindByName("righthud");

		if (menuHUD)
		{
			if (cgs.gametype != GT_POWERDUEL)
			{
				focusItem = Menu_FindItemByName(menuHUD, "score_line");
				if (focusItem)
				{
					UI_DrawScaledProportionalString(
						focusItem->window.rect.x, 
						focusItem->window.rect.y, 
						scoreStr, 
						UI_RIGHT|UI_DROPSHADOW, 
						focusItem->window.foreColor, 
						0.7);
				}
			}

			/*focusItem = Menu_FindItemByName(menuHUD, "frame");
			if (focusItem)
			{
				CG_DrawPic( 
					focusItem->window.rect.x, 
					focusItem->window.rect.y, 
					focusItem->window.rect.w, 
					focusItem->window.rect.h, 
					focusItem->window.background 
					);			
			}*/

			CG_DrawForcePower(menuHUD);
			CG_DrawBalanceTicMethod(cent, menuHUD);

			// Draw ammo tics or saber style
			if ( cent->currentState.weapon == WP_SABER ) {
				CG_DrawSaberStyleTicMethod(cent, menuHUD);
			}
			else {
				CG_DrawAmmoTicMethod(cent, menuHUD);
			}
		}
		else { 
			//CG_Error("CG_ChatBox_ArrayInsert: unable to locate HUD menu file ");
		}
	}
	if (cg_hudFiles.integer == 2)
	{//Raz0r's method
		//RAZFIXME: lol do this nao!
	}
}
//[/JKEHud]
