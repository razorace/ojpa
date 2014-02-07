#ifndef colorRangeDef_t_h
#define colorRangeDef_t_h

#include "../game/q_shared.h"

#define MAX_COLOR_RANGES 10

typedef struct {
	vec4_t	color;
	float		low;
	float		high;
} colorRangeDef_t;

#endif