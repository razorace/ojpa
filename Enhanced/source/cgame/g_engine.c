#include "cg_local.h"

cvar_t *Cvar_FindVar(const char *cvarname) 
{
	__asm
	{
		mov edi, cvarname
		mov eax, 0x4393B0
		call eax
	}
}