/*
 * Copyright (C) 2020 by Heiko Pruessing
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
*/

// This module contains the library startup code.
// This file should be the first object linked into the device
// it's our "device startup code".

// ------------------- INCLUDES ------------------------

#include <proto/exec.h>
#include <proto/utility.h>
#include <exec/types.h>
#include <exec/resident.h>
#include <libinit.h>

#include "helper.h"
#include "version.h"

#include "i2clib.h"

// ------------------- Globals ---------------------------

extern const APTR InitTab[4];
struct ExecBase * SysBase = NULL;

// ------------------- local Prototypes ------------------------

SAVEDS struct Library * asmLibInit(REG(a0, BPTR segmentList),
                                   REG(d0, I2CLibrary * lib),
                                   REG(a6, struct ExecBase * exec_base));

SAVEDS BPTR asmLibExpunge(REG(a6, I2CLibrary * library));

// ------------------- IMPLEMENTATION ------------------------

/**
 * First function of the device. Dummy function. "libnix" needs "exit()" to be referenced.
 * Also this is save way to prevent execution from CLI.
 */
LONG asmLibNullFunction( void )
{
   return 0;
}
ALIAS(exit, asmLibNullFunction);

const char LibName[]     __attribute__((used)) = LIBRARY_NAME;
const char LibIdString[] __attribute__((used)) = VERSION_STRING;

static const struct Resident RomTag __attribute__((used)) =
{
  RTC_MATCHWORD,
  (struct Resident *)&RomTag,
  (struct Resident *)&RomTag+sizeof(RomTag),
  RTF_AUTOINIT,
  LIBRARY_VERSION,
  NT_LIBRARY,
  0, /* PRIO */
  (char *)LibName,
  (char *)LibIdString,
  (APTR)&InitTab
};

const APTR InitTab[4] =
{
  (APTR)sizeof(I2CLibrary),
  (APTR)&__FuncTable__[1],
  (APTR)NULL, // => No "InitTable": initialize Library base by hand (in function "LibInit()" )
  (APTR)&asmLibInit
};



/**
 * AmigaOS entry point "DevInit".
 *
 * @param REG(a0,BPTR SegListPointer)
 * @param REG(d0,struct Library * Library)
 * @param REG(a6,struct Library * execBase)
 * @return D0 Library...
 */
struct Library * asmLibInit(REG(a0,BPTR segmentList),
                            REG(d0,I2CLibrary * library),
                            REG(a6,struct ExecBase * execbase))
{
   //Pick up Exec Base...
   SysBase = execbase;
   library->segmentList = segmentList;

   //init device structure
   library->library.lib_Node.ln_Type = NT_DEVICE;
   library->library.lib_Node.ln_Name = (UBYTE *)LibName;
   library->library.lib_Flags        = LIBF_CHANGED | LIBF_SUMUSED;
   library->library.lib_Version      = (ULONG)LIBRARY_VERSION;
   library->library.lib_Revision     = (ULONG)LIBRARY_REVISION;
   library->library.lib_IdString     = (char *)LibIdString;

   return &library->library;
}


/**
 * AmigaOS entry point "Open Library".
 *
 * @param REG(d0, Own Library
 * @param REG(a6, struct Library * dev)
 */
struct Library * asmLibOpen(
      REG(d0, ULONG ver),
      REG(a6, I2CLibrary * library) )
{
   library->library.lib_OpenCnt++;
   library->library.lib_Flags &= ~(LIBF_DELEXP);
   return &library->library;
}

/**
 * AmigaOS entry point "Close Device".
 *
 * @param REG(a1,APTR iorq)
 * @param REG(a6,struct Library * DevBase)
 * @return
 */
SAVEDS
BPTR asmLibClose( REG(a6, I2CLibrary * library))
{
   library->library.lib_OpenCnt--;
   if (0 == library->library.lib_OpenCnt) {
      if (library->library.lib_Flags & LIBF_DELEXP) {
         asmLibExpunge(library);
      }
   }

   return NULL;
}

/**
 * AmigaOS entry point "Expunge device".
 *
 * @param REG(a6,struct Library * DevBase)
 * @return
 */
SAVEDS
BPTR asmLibExpunge(REG(a6, I2CLibrary * library))
{
   BPTR segmentList = 0l;
   if (library->library.lib_OpenCnt > 0) {
      //still in use, set flag to remove next time...
      library->library.lib_Flags |= LIBF_DELEXP;
   } else {

      //remove lib now...

      Remove(&library->library.lib_Node);

      segmentList      = library->segmentList;
      ULONG devbase    = (ULONG)library;
      LONG devbasesize = (ULONG)library->library.lib_NegSize;
      devbase          = devbase - devbasesize;
      devbasesize     += (ULONG)library->library.lib_PosSize;

      FreeMem((APTR)devbase,devbasesize);
   }
   return segmentList;
}

/**
 * Now add all "Library Functions" to the Library/Device.
 * WARNING: All functions must exist! If not, no warning is generated and the ADD2LIST is not called!
 * Errors here would re-order the function table and funny things happen when using the library!
 */
ADD2LIST(asmLibOpen,         __FuncTable__,22);
ADD2LIST(asmLibClose,        __FuncTable__,22);
ADD2LIST(asmLibExpunge,      __FuncTable__,22);
ADD2LIST(asmLibNullFunction, __FuncTable__,22); // => Unused AmigaOS function (but must be present in table!)
//TODO: Add further library calls here...
ADDTABL_END();
