#ifndef colorRangeDef_t_h
#define colorRangeDef_t_h

#include "../game/q_shared.h"

const int MAX_COLOR_RANGES	=	10;

typedef struct {
	vec4_t	color;
	float		low;
	float		high;
} colorRangeDef_t;

#endif