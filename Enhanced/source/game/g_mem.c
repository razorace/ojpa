// Copyright (C) 1999-2000 Id Software, Inc.
//
//
// g_mem.c
//


#include "g_local.h"

//[CoOp]
//some SP maps require more memory.  Bumped the memory size a little.
#define POOLSIZE (4 * 88400)
//[/CoOp]

static char		memoryPool[POOLSIZE];
static int		allocPoint = 0;

void *G_Alloc( int size ) {
	char	*p;

	if ( g_debugAlloc.integer ) {
		G_Printf( "G_Alloc of %i bytes (%i left)\n", size, POOLSIZE - allocPoint - ( ( size + 31 ) & ~31 ) );
	}

	if ( allocPoint + size > POOLSIZE ) {
		G_Error( "G_Alloc: failed on allocation of %i bytes\n", size ); // bk010103 - was %u, but is signed
		return NULL;
	}

	p = &memoryPool[allocPoint];

	allocPoint += ( size + 31 ) & ~31;

	return p;
}

void Svcmd_GameMem_f( void ) {
	G_Printf( "Game memory status: %i out of %i bytes allocated\n", allocPoint, POOLSIZE );
}
