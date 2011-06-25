#ifndef WindowDef_t_h
#define WindowDef_t_h

#include "Rectangle.h"
#include "../game/q_shared.h"

// FIXME: do something to separate text vs window stuff
typedef struct {
	Rectangle rect;                 // client coord rectangle
	Rectangle rectClient;           // screen coord rectangle
	const char *name;               //
	const char *group;              // if it belongs to a group
	const char *cinematicName;		  // cinematic name
	int cinematic;								  // cinematic handle
	int style;                      //
	int border;                     //
	int ownerDraw;									// ownerDraw style
	int ownerDrawFlags;							// show flags for ownerdraw items
	float borderSize;               // 
	int flags;                      // visible, focus, mouseover, cursor
	Rectangle rectEffects;          // for various effects
	Rectangle rectEffects2;         // for various effects
	int offsetTime;                 // time based value for various effects
	int nextTime;                   // time next effect should cycle
	vec4_t foreColor;               // text color
	vec4_t backColor;               // border color
	vec4_t borderColor;             // border color
	vec4_t outlineColor;            // border color
	qhandle_t background;           // background asset  
} windowDef_t;

typedef windowDef_t Window;

#endif