/*
**     $Author: Bipsen $
**     $Filename: i2c_library.h $
**     $Release: 3.00 $
**     $Revision: 40.0 $
**     $Date: 1998/08/11 22:17:48 $
**
**     Headerfile to include in programs using i2c.library
**
*/

#include <exec/types.h>
#include <exec/libraries.h>

extern struct Library *I2C_Base;

#if 0
/*--- functions in v37.0 or higher ---*/
#pragma libcall I2C_Base AllocI2C 1E 9002
#pragma libcall I2C_Base FreeI2C 24 0
#pragma libcall I2C_Base SetI2CDelay 2A 1
#pragma libcall I2C_Base InitI2C 30 0
#pragma libcall I2C_Base SendI2C 36 91003
#pragma libcall I2C_Base ReceiveI2C 3C 91003
/*--- functions in v38.0 or higher ---*/
#pragma libcall I2C_Base GetI2COpponent 42 0
/*--- functions in v39.0 or higher ---*/
#pragma libcall I2C_Base I2CErrText 48 1
#pragma libcall I2C_Base ShutDownI2C 4E 0
#pragma libcall I2C_Base BringBackI2C 54 0
#endif

/* Proto-types for functions */
#if 0
BYTE AllocI2C(UBYTE Delay_Type,char *Name);
void FreeI2C(void);
ULONG SetI2CDelay(ULONG ticks);
void InitI2C(void);
ULONG SendI2C(UBYTE addr, UWORD number, UBYTE i2cdata[]);
ULONG ReceiveI2C(UBYTE addr, UWORD number, UBYTE i2cdata[]);
STRPTR GetI2COpponent(void);
STRPTR I2CErrText(ULONG errnum);
void ShutDownI2C(void);
BYTE BringBackI2C(void);
#endif

/* Definitions for return-codes etc. */

/* If you call SetI2CDelay only to read the delay, not change it: */
#define I2CDELAY_READONLY 0xffffffff    /* V39+ ! */

/* Type of delay to pass to AllocI2C (obsolete in V39+, see docs): */
#define DELAY_TIMER 1   /* Use timer.device for SCL-delay  */
#define DELAY_LOOP  2   /* Use for/next-loop for SCL-delay */

/* Allocation Errors */
/* (as returned by AllocI2C, BringBackI2C, or found in the middle high */
/* byte of the error codes from V39's SendI2C/ReceiveI2C) */
enum {
    I2C_OK=0,               /* Hardware allocated successfully */
    I2C_PORT_BUSY,          /* \_Allocation is actually done in two steps: */
    I2C_BITS_BUSY,          /* / port & bits, and each step may fail */
    I2C_NO_MISC_RESOURCE,   /* Shouldn't occur, something's very wrong */
    I2C_ERROR_PORT,         /* Failed to create a message port */
    I2C_ACTIVE,             /* Some other I2C client has pushed us out */
    I2C_NO_TIMER            /* Failed to open the timer.device */
    };

/* I/O Errors */
/* (as found in the middle low byte of the error codes from V39's */
/* SendI2C/ReceiveI2C) */
enum {
    /*I2C_OK=0,*/       /* Last send/receive was OK */
    I2C_REJECT=1,       /* Data not acknowledged (i.e. unwanted) */
    I2C_NO_REPLY,       /* Chip address apparently invalid */
    SDA_TRASHED,        /* SDA line randomly trashed. Timing problem? */
    SDA_LO,             /* SDA always LO \_wrong interface attached, */
    SDA_HI,             /* SDA always HI / or none at all? */
    SCL_TIMEOUT,        /* \_Might make sense for interfaces that can */
    SCL_HI,             /* / read the clock line, but currently none can. */
    I2C_HARDW_BUSY      /* Hardware allocation failed */
    };

/* ======================================================================== */
/* === I2C_Base =========================================================== */
/* ======================================================================== */
/*
 * Starting with V40, i2c.library exposes some statistics counters, and a
 * hint what kind of hardware implementation you are dealing with, in its
 * base structure. These data weren't present in any of the previous
 * releases, so check the lib version before you try to read them.
 */

/* This structure is READ ONLY, and only present in V40 or later! */
struct I2C_Base
    {
    struct Library LibNode;
    ULONG SendCalls;        /* calls to SendI2C */
    ULONG SendBytes;        /* bytes actually sent */
    ULONG RecvCalls;        /* calls to ReceiveI2C */
    ULONG RecvBytes;        /* bytes actually received */
    ULONG Lost;             /* calls rejected due to resource conflicts */
    ULONG Unheard;          /* calls to addresses that didn't reply */
    ULONG Overflows;        /* times a chip rejected some or all of our data */
    ULONG Errors;           /* errors caused by hardware/timing problems */
    UBYTE HwType;           /* implementation: 0=par, 1=ser, 2=disk, 3=card */
                            /* 4=smart card */
    /* The data beyond this point is private and is different between
     * most of the various i2c.library implementations anyway.
     */
    };
