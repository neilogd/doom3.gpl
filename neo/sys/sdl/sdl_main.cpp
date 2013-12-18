/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).  

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "../../idlib/precompiled.h"
#pragma hdrstop

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

#include "include/SDL.h"

#if NEILOGD_SDL_BUILD

// critical sections.
static SDL_mutex* s_mutexes[ MAX_CRITICAL_SECTIONS ];

/*
==================
Sys_EnterCriticalSection
==================
*/
void Sys_EnterCriticalSection( int index ) {
	assert( index >= 0 && index < MAX_CRITICAL_SECTIONS );
	if ( SDL_TryLockMutex( s_mutexes[index] ) == 0 ) {
		SDL_LockMutex( s_mutexes[index] );
	}
}

/*
==================
Sys_LeaveCriticalSection
==================
*/
void Sys_LeaveCriticalSection( int index ) {
	assert( index >= 0 && index < MAX_CRITICAL_SECTIONS );
	SDL_UnlockMutex( s_mutexes[index] );
}


int main( int argc, char **argv ) {
	Sys_SetPhysicalWorkMemory( 192 << 20, 1024 << 20 );

	// no abort/retry/fail errors
	SetErrorMode( SEM_FAILCRITICALERRORS );

	for ( int i = 0; i < MAX_CRITICAL_SECTIONS; i++ ) {
		s_mutexes[i] = SDL_CreateMutex();
	}

	// get the initial time base
	Sys_Milliseconds();

	Sys_FPU_SetPrecision( FPU_PRECISION_DOUBLE_EXTENDED );

	// combine the args into a windows-style command line
	char	cmdline[4096];
	cmdline[0] = 0;
	for ( int i = 1 ; i < argc ; i++ ) {
		strcat( cmdline, argv[i] );
		if ( i < argc - 1 ) {
			strcat( cmdline, " " );
		}
	}
	common->Init( argc, (const char**)argv, cmdline );

	while( 1 ) {
		common->Async();
		common->Frame();
	}
}

#endif //
