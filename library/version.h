/*
 * Copyright (C) 2019 by Heiko Pruessing
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
*/

#ifndef __DEV_VERSION__
#define __DEV_VERSION__

//The device versions and the device build time...
#define LIBRARY_VERSION  1
#define LIBRARY_REVISION 0
#define LIBRARY_BUILD    1

//It seems that the date should be in format (dd Mmm yyyy).
//Only than AmigaOS (3.x) version command can extract it.
#define LIBRARY_VERSION_DATE "(12 Apr 2020)"

// ---------------------- Do not change anything behind that line -------------------------------------------

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#if DEBUG > 0
#define COMPILED_TIME " " __TIME__
#else
#define COMPILED_TIME
#endif

#if defined(__mc68060__)
#define CPUTYPE "68060"
#elif defined(__mc68040__)
#define CPUTYPE "68040"
#elif defined(__mc68030__)
#define CPUTYPE "68030"
#elif defined(__mc68020__)
#define CPUTYPE "68020"
#else
#define CPUTYPE "68000"
#endif

#define LIBRARY_BUILD_STRING STR(LIBRARY_BUILD)
#if DEBUG > 0
#define DEBUG_TEXT "DEBUG build " LIBRARY_BUILD_STRING " "
#else
#define DEBUG_TEXT "(Build " LIBRARY_BUILD_STRING ") "
#endif

#ifndef LIBRARY_NAME
#error You forgot to define the device name! Must be a compiler predefine...
#endif


#define VERSION_STRING "$VER " LIBRARY_NAME " " STR(LIBRARY_VERSION) "." STR(LIBRARY_REVISION) " " \
                       LIBRARY_VERSION_DATE " " \
                       DEBUG_TEXT \
                       "(" CPUTYPE ")" \
                       " (c) by Heiko Pruessing " COMPILED_TIME \
                       "\r\n\0"

#endif
