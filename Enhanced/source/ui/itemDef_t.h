#ifndef itemDef_t_h
#define itemDef_t_h

#include "WindowDef_t.h"
#include "colorRangeDef_t.h"

typedef struct itemDef_s {
	Window		window;						// common positional, border, style, layout info
	Rectangle	textRect;					// rectangle the text ( if any ) consumes     
	int			type;						// text, button, radiobutton, checkbox, textfield, listbox, combo
	int			alignment;					// left center right
	int			textalignment;				// ( optional ) alignment for text within rect based on text width
	float		textalignx;					// ( optional ) text alignment x coord
	float		textaligny;					// ( optional ) text alignment x coord
	float		textscale;					// scale percentage from 72pts
	int			textStyle;					// ( optional ) style, normal and shadowed are it for now
	const char	*text;						// display text
	const char	*text2;						// display text, 2nd line
	float		text2alignx;				// ( optional ) text2 alignment x coord
	float		text2aligny;				// ( optional ) text2 alignment y coord
	void		*parent;					// menu owner
	qhandle_t	asset;						// handle to asset
	void		*ghoul2;					// ghoul2 instance if available instead of a model.
	int			flags;						// flags like g2valid, character, saber, saber2, etc.
	const char	*mouseEnterText;			// mouse enter script
	const char	*mouseExitText;				// mouse exit script
	const char	*mouseEnter;				// mouse enter script
	const char	*mouseExit;					// mouse exit script 
	const char	*action;					// select script
//JLFACCEPT MPMOVED
	const char  *accept;
//JLFDPADSCRIPT
	const char * selectionNext;
	const char * selectionPrev;

	const char	*onFocus;					// select script
	const char	*leaveFocus;				// select script
	const char	*cvar;						// associated cvar 
	const char	*cvarTest;					// associated cvar for enable actions
	const char	*enableCvar;				// enable, disable, show, or hide based on value, this can contain a list
	int			cvarFlags;					//	what type of action to take on cvarenables
	sfxHandle_t focusSound;
	int			numColors;					// number of color ranges
	colorRangeDef_t colorRanges[MAX_COLOR_RANGES];
	float		special;					// used for feeder id's etc.. diff per type
	int			cursorPos;					// cursor position in characters
	void		*typeData;					// type specific data ptr's	
	const char	*descText;					//	Description text
	int			appearanceSlot;				// order of appearance
	int			iMenuFont;					// FONT_SMALL,FONT_MEDIUM,FONT_LARGE	// changed from 'font' so I could see what didn't compile, and differentiate between font handles returned from RegisterFont -ste
	qboolean	disabled;					// Does this item ignore mouse and keyboard focus
	int			invertYesNo;
	int			xoffset;
} itemDef_t;

#endif