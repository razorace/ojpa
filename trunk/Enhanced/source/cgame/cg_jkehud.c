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
const float FPBAR_X		= 535.0f;
const float FPBAR_Y		= 425.0f;

const float MPBAR_Y		= 410.0;

const float HPBAR_X		= 10.0f;
const float HPBAR_Y		= 425.0f;
const float HPBAR_W		= 65.0f;
const float HPBAR_H		= 7.0f + 2.1f;

const float APBAR_X		= 10.0f;
const float APBAR_Y		= 440.0f;
const float APBAR_H		= 7.0f + 2.1f;
const float APBAR_W		= 65.0f;

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

void CG_DrawPercentageBox(float x, float y, float width, float height, float percent, const float *borderColor, const float *filledColor) {
	CG_DrawRect(x, y, width, height, 1.0f, borderColor);
	CG_FillRect((x + width), y, -((width)-percent), height, filledColor);
}

//Health
void CG_DrawHealthTicMethod(menuDef_t *menuHUD)
{
	playerState_t	*ps;
	int				healthAmt;
	float			percent = ((float)cg.snap->ps.stats[STAT_HEALTH] / (float)cg.snap->ps.stats[STAT_MAX_HEALTH]) * FPBAR_W;
	vec4_t aColor;
	vec4_t cColor;

	// Can we find the menu?
	if (!menuHUD)
	{
		return;
	}

	ps = &cg.snap->ps;

	// What's the health?
	healthAmt = ps->stats[STAT_HEALTH];
	if (healthAmt > ps->stats[STAT_MAX_HEALTH]) {
		healthAmt = ps->stats[STAT_MAX_HEALTH];
	}

	aColor[0] = 1.0f;
	aColor[1] = 0.0f;
	aColor[2] = 0.0f;
	aColor[3] = 0.5f;
	
	Vector4Copy(colorTable[CT_BLACK], cColor);
	cColor[3] = 0.4f;


	CG_DrawRect(HPBAR_X - 1.0f, FPBAR_Y - 1.0f, FPBAR_W + 2.1f, FPBAR_H + 2.1f, 1.0f, colorTable[CT_BLACK]);
	CG_FillRect(HPBAR_X, FPBAR_Y, percent, FPBAR_H, aColor);

	UI_DrawScaledProportionalString(HPBAR_X+FPBAR_W + 3.0f,FPBAR_Y-4.0f, va( "%i", cg.snap->ps.stats[STAT_HEALTH] ),
		UI_SMALLFONT|UI_DROPSHADOW, colorTable[CT_WHITE] , 0.5f);
}


//Armor
void CG_DrawArmorTicMethod(menuDef_t *menuHUD)
{
	vec4_t			calcColor;
	playerState_t	*ps;
	int				armor, maxArmor;
	float			percent;
	int				currValue;
	vec4_t aColor;
	vec4_t cColor;
	
	calcColor[0] = 0;
	calcColor[1] = 0;
	calcColor[2] = 0;
	calcColor[3] = 0;
	ps = &cg.predictedPlayerState;

	armor = ps->stats[STAT_ARMOR];
	maxArmor = ps->stats[STAT_MAX_HEALTH];

	if (armor > maxArmor)
	{
		armor = maxArmor;
	}

	currValue = armor;


	percent = ((float)armor / (float)maxArmor )* FPBAR_W;

	aColor[0] = 0.0f;
	aColor[1] = 1.0f;
	aColor[2] = 0.0f;
	aColor[3] = 0.5f;
	
	Vector4Copy(colorTable[CT_BLACK], cColor);
	cColor[3] = 0.4f;

	//CG_DrawRect(APBAR_X - 1.0f, APBAR_Y - 1.0f, APBAR_W + 2.1f, APBAR_H + 2.1f, 1.0f, colorTable[CT_BLACK]);
	//CG_FillRect((APBAR_X + APBAR_W), APBAR_Y, -((APBAR_W)-percent), APBAR_H, cColor);
	
	CG_DrawRect(APBAR_X, APBAR_Y, APBAR_W, APBAR_H, 1.0f, colorTable[CT_BLACK]);
	CG_FillRect((APBAR_X + APBAR_W), APBAR_Y, -((APBAR_W)-percent), APBAR_H, cColor);
	//CG_DrawPercentageBox(APBAR_X, APBAR_Y, APBAR_W, APBAR_H, percent, colorTable[CT_BLACK], cColor);

	UI_DrawScaledProportionalString( HPBAR_X+FPBAR_W + 3.0f,APBAR_Y-4.0f, va( "%i", armor ), UI_SMALLFONT|UI_DROPSHADOW, colorTable[CT_WHITE] , 0.5f);
}

void CG_FillRect2( float x, float y, float width, float height, const float *color );

//Balance
void CG_DrawBalanceTicMethod(centity_t *cent, menuDef_t *menuHUD)
{
	float			percent;
	vec4_t aColor;
	vec3_t color;


	int amt = cg.snap->ps.saberAttackChainCount;
	if(amt > BALANCE_MAX) amt = BALANCE_MAX;
	
	// What's the health?
	percent = ((float)amt / (float)BALANCE_MAX)  * (float)FPBAR_W;


	
	//aColor[0] = 1.0f;
	//aColor[1] = 0.0f;
	//aColor[2] = 1.0f;
	//aColor[3] = 0.5f;
	//173,35,75
	//99,9,71
	aColor[0] = 99.0f / 255;
	aColor[1] = 9.0f / 255;
	aColor[2] = 71.0f / 255;
	aColor[3] = 1;

	CG_DrawRect(FPBAR_X - 1.0f, MPBAR_Y- 1.0f, FPBAR_W + 2.1f, FPBAR_H + 2.1f, 1.0f, colorTable[CT_BLACK]);

	CG_FillRect(FPBAR_X, MPBAR_Y, percent, FPBAR_H, aColor);
	
	color[0] = aColor[0];
	color[1] = aColor[1];
	color[2] = aColor[2];

	UI_DrawScaledProportionalString( FPBAR_X - 12.0f,MPBAR_Y-4.0f, va( "%i", cg.snap->ps.saberAttackChainCount ),
		UI_SMALLFONT|UI_DROPSHADOW|UI_RIGHT, colorTable[CT_WHITE], 0.5f );
}

//Ammo
void CG_DrawAmmoTicMethod(centity_t *cent, menuDef_t *menuHUD)
{
	playerState_t	*ps;
	float			value,percent;
	vec4_t aColor;
	vec4_t cColor;
	float max;
	vec3_t color;

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
	//ammoData[AMMO_BLASTER].max;
	
	max = ammoData[weaponData[cent->currentState.weapon].ammoIndex].max;
	percent = ((float)ps->ammo[weaponData[cent->currentState.weapon].ammoIndex] / max )* FPBAR_W;

	aColor[0] = 0.23f;
	aColor[1] = 0.3f;
	aColor[2] = 0.31f;
	aColor[3] = 0.5f;

	
	Vector4Copy(colorTable[CT_BLACK], cColor);
	cColor[3] = 0.4f;


	CG_DrawRect(FPBAR_X - 1.0f, APBAR_Y - 1.0f, FPBAR_W + 2.1f, FPBAR_H + 2.1f, 1.0f, colorTable[CT_BLACK]);
		CG_FillRect(FPBAR_X + ( FPBAR_W- percent), APBAR_Y, FPBAR_W-(FPBAR_W-percent), FPBAR_H, aColor);
		CG_FillRect((FPBAR_X), APBAR_Y, (FPBAR_W)-percent, FPBAR_H, cColor);


	color[0] = aColor[0];
	color[1] = aColor[1];
	color[2] = aColor[2];

	UI_DrawScaledProportionalString( FPBAR_X - 12.0f,APBAR_Y-4.0f, va( "%i",(int)value  ),
		UI_SMALLFONT|UI_DROPSHADOW|UI_RIGHT, colorTable[CT_WHITE], 0.5f );
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

int lastForceAmount = -1;
int forceLostAmount = 0;
int forceLostTimer = 0;

//Force
void CG_DrawForcePower( menuDef_t *menuHUD )
{
	vec4_t			aColor;
	vec4_t			cColor;
	int				currentForceAmount = cg.snap->ps.fd.forcePower;
	float			percent = ((float)currentForceAmount / 100.0f) * FPBAR_W;
	int	forceAmountDifference = currentForceAmount - (lastForceAmount == -1 ? currentForceAmount : lastForceAmount);
	vec3_t color;

	forceLostAmount -= forceAmountDifference;
	
	if(forceLostAmount < 0) {
		forceLostAmount = 0;
	}

	if(forceLostTimer < cg.time) {
		forceLostTimer = cg.time + 50;

		if(forceLostAmount > 0) {
			forceLostAmount--;
		}
	}

	//color of the bar -- DD: What are these silly magic numbers??
	if(cg.snap->ps.fd.forcePowerLevel[FP_SEE] > 0) {
		//aColor[0] = 0.503f;
		//aColor[1] = 0.375f;
		//aColor[2] = 0.996f;
		//aColor[3] = 0.5f;
		//22,147,165
		aColor[0] = 22.0f / 255;
		aColor[1] = 147.0f / 255;
		aColor[2] = 165.0f / 255;
		aColor[3] = 1;
	}
	else {
		aColor[0] = 1.0f;
		aColor[1] = 1.0f;
		aColor[2] = 0.0f;
		aColor[3] = 0.5f;
	}
	
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


	if (percent > FPBAR_W) {
		return;
	}

	if (percent < 0.1f) {
		percent = 0.1f;
	}

	Vector4Copy(colorTable[CT_BLACK], cColor);
	cColor[3] = 0.4f;

	CG_DrawRect(FPBAR_X - 1.0f, FPBAR_Y - 1.0f, FPBAR_W + 2.1f, FPBAR_H + 2.1f, 1.0f, colorTable[CT_BLACK]);

	CG_FillRect(FPBAR_X + ( FPBAR_W- percent), FPBAR_Y, FPBAR_W-(FPBAR_W-percent), FPBAR_H, aColor);

	if(forceLostAmount != 0) {
		//199,40,23
		vec4_t lostColor;
		float lPercent;
		lostColor[0] = 231.0f / 255;
		lostColor[1] = 53.0f / 255;
		lostColor[2] = 37.0f / 255;
		lostColor[3] = 1;
		//128,15,37
		//231,53,37
		lPercent = ((float)forceLostAmount / 100.0f) * FPBAR_W;
		CG_FillRect(FPBAR_X + ( FPBAR_W- percent)-lPercent, FPBAR_Y, FPBAR_W-(FPBAR_W-lPercent), FPBAR_H, lostColor);
	}
	
	color[0] = aColor[0];
	color[1] = aColor[1];
	color[2] = aColor[2];

	UI_DrawScaledProportionalString( FPBAR_X - 12.0f,FPBAR_Y-4.0f, va( "%i", cg.snap->ps.fd.forcePower ),
		UI_SMALLFONT|UI_DROPSHADOW|UI_RIGHT,colorTable[CT_WHITE], 0.5f);

	lastForceAmount = currentForceAmount;
}

//	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--
//	  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
//	--	--	--	--	--	END OF TIC METHOD	--	--	--	--	--	--	--
//	  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
//	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--	--

//Draw the (max 2) gadgets the player has
void CG_DrawGadgets(void)
{	
	//if(cg.snap->ps.stats[STAT_HOLDABLE_ITEMS] == 0)
		//return; //We don't have any items

	int inven1 = cg.snap->ps.stats[STAT_GADGET_1], inven2 = cg.snap->ps.stats[STAT_GADGET_2];

	if(inven1 != 999)
	{
		if((cg.snap->ps.stats[STAT_HOLDABLE_ITEMS] & (1 << inven1) ) )
		{
			//trap_R_SetColor(NULL);
			trap_R_SetColor(colorTable[CT_ICON_BLUE]);
			CG_DrawPic( 110, 420, 30, 30, cgs.media.invenIcons[inven1] );
			
		}
		else
		{
			trap_R_SetColor(colorTable[CT_RED]);
			CG_DrawPic( 110, 420, 30, 30, cgs.media.invenIcons[inven1] );
			//trap_R_SetColor(colorTable[CT_RED]);
		}
		UI_DrawScaledProportionalString(125, 446, "SB1", 
						UI_RIGHT|UI_DROPSHADOW, 
						colorTable[CT_ICON_BLUE], 
						0.4);
	}
	if(inven2 != 999)
	{
		if(cg.snap->ps.stats[STAT_HOLDABLE_ITEMS] & (1 << inven2) )
		{
			//trap_R_SetColor(NULL);
			trap_R_SetColor(colorTable[CT_ICON_BLUE]);
			CG_DrawPic( 150, 420, 30, 30, cgs.media.invenIcons[inven2] );		
			

		}
		else
		{
			//trap_R_SetColor(NULL);
			trap_R_SetColor(colorTable[CT_RED]);
			CG_DrawPic( 153, 420, 30, 30, cgs.media.invenIcons[inven2] );
			
		}

		UI_DrawScaledProportionalString(163, 446, "SB2", 
						UI_RIGHT|UI_DROPSHADOW, 
						colorTable[CT_ICON_BLUE], 
						0.4);
	}
}


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

		if (menuHUD) {
			if (cg.predictedPlayerState.pm_type != PM_SPECTATOR) {
				CG_DrawArmorTicMethod(menuHUD);
				CG_DrawHealthTicMethod(menuHUD);

				CG_DrawGadgets();
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

			if(cg.predictedPlayerState.pm_type != PM_SPECTATOR)
			{
				CG_DrawForcePower(menuHUD);
				CG_DrawBalanceTicMethod(cent, menuHUD);
			}
			

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
