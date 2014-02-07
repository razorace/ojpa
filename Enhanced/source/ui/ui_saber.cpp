/*
=======================================================================

USER INTERFACE SABER LOADING & DISPLAY CODE

=======================================================================
*/

// leave this at the top of all UI_xxxx files for PCH reasons...
//
//#include "../server/exe_headers.h"
#include "ui_local.h"
#include "ui_shared.h"

void UI_CacheSaberGlowGraphics( void )
{//FIXME: these get fucked by vid_restarts
	redSaberGlowShader			= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/red_glow" );
	redSaberCoreShader			= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/red_line" );
	orangeSaberGlowShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/orange_glow" );
	orangeSaberCoreShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/orange_line" );
	yellowSaberGlowShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/yellow_glow" );
	yellowSaberCoreShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/yellow_line" );
	greenSaberGlowShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/green_glow" );
	greenSaberCoreShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/green_line" );
	blueSaberGlowShader			= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/blue_glow" );
	blueSaberCoreShader			= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/blue_line" );
	purpleSaberGlowShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/purple_glow" );
	purpleSaberCoreShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/purple_line" );
	//[RGBSabers]
	rgbSaberGlowShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/rgb_glow" );
	rgbSaberCoreShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/rgb_line" );
	rgbSaberCore2Shader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/rgb_core" );
	blackSaberGlowShader		= trap_R_RegisterShaderNoMip( "gfx/effects/sabers/black_glow" );
	//[/RGBSabers]

}
