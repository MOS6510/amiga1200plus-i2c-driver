/**
 * Unittest for i2c library...
 * Author: Heiko Pruessing
 */

#include <clib/alib_protos.h>
#include <clib/graphics_protos.h>

#include <strings.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <exec/types.h>
#include <sys/types.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//Auto generated header file for i2c lib...
#include "inline_i2c_lib.h"

#include "../library/i2c_library.h"

// ------------------------ LOCALS -------------------------------

const char I2C_LIBNAME[] = "i2c.library";

struct Library * I2C_BASE_NAME = NULL;
void done(void);

#define COLOR02   "\033[32m"
#define COLOR03   "\033[33m"
#define NORMAL    "\033[0m"
#define CLRSCR    "\033[1;1H\033[2J"
#define CURSOR_UP "\033[1A"
#define TEST_ENTERED() printf("%s...", __func__);
#define TEST_LEAVE(a)  printf("%s\n", (a) ? "success" : "failed!");

// ------------------------ LOCALS -------------------------------

int maxerror = 0;

struct chip
    {
    UBYTE lower_addr, upper_addr;
    STRPTR description;
    };

struct chip chiptab[] =
    {
    { 0x20, 0x20, "PCF8200: speech synthesizer" },
    { 0x20, 0x22, "SAF1135/SAA4700: VPS decoder" },
    { 0x22, 0x22, "SAA5243/SAA5246: teletext decoder" },
    { 0x40, 0x4E, "PCF8574: 8 bit IO expander" },
    { 0x40, 0x4E, "TDA8444: 8 � 6 bit D/A converter" },
    { 0x48, 0x4A, "PCD3311/PCD3312: DTMF/modem/musical tone generator" },
    { 0x70, 0x7E, "PCF8574A: 8 bit IO expander" },
    { 0x70, 0x76, "SAA1064: LED driver 2-4 � 8" },
    { 0x70, 0x72, "PCF8576: LCD driver 1-4 � 40" },
    { 0x74, 0x74, "PCF8577: LCD driver 1-2 � 32" },
    { 0x76, 0x76, "PCF8577A: LCD driver 1-2 � 32" },
    { 0x7C, 0x7E, "PCF8566: LCD driver 1-4 � 24" },
    { 0x78, 0x7A, "PCF8578: LCD dot matrix driver 32�8 / 24�16" },
    { 0x80, 0x86, "SAA1300: power output 5 � 85 mA" },
    { 0x88, 0x88, "TDA8442: DAC & switch for color decoder" },
    { 0x90, 0x9E, "PCF8591: 8 bit DA/AD converter" },
    { 0x90, 0x9E, "TDA8440: AV input selector" },
    { 0xA0, 0xA2, "PCF8583: clock/calendar and 256 byte SRAM" },
    { 0xA0, 0xAE, "PCF8570/PCF8571: 256/128 byte SRAM" },
    { 0xA0, 0xAE, "PCF8581/PCF8582: 256/128 byte EEPROM" },
    { 0xB0, 0xBE, "PCF8570C: 256 byte SRAM" },
    { 0xC0, 0xC6, "SAB3035/SAB3036: tuning and control interface" },
    { 0xD0, 0xD6, "PCF8573: clock/calendar" }
    };

/* Tries its best to "identify" a chip address, but most I2C addresses
 * are ambiguous.
 */
void identify( UBYTE addr ) {
    int i;
    printf( ", might be:\n" );
    for( i = 0; i < (sizeof chiptab / sizeof chiptab[0]); i++ ) {
        if( addr >= chiptab[i].lower_addr && addr <= chiptab[i].upper_addr ) {
            printf( "%s\n", chiptab[i].description );
        }
    }
}

/* Analyze an i2clib return code, returns TRUE if it indicates an error */
int report( ULONG code ) {
   static ULONG lastcode = 0;
   STRPTR s;

   if( code & 0xFF )   /* indicates OK */
      return FALSE;
   else
   {
      if( code != lastcode ) {
         lastcode = code;
         if( (code >> 8) > I2C_NO_REPLY )
         {
            printf( "I2C bus: error 0x%06lx, %s\n", code, I2CErrText( code ) );
            s = GetI2COpponent();
            if( s != NULL )
               printf("\"%s\" has got our hardware\n", s);
            if( ((code >> 8) & 0xff) == I2C_HARDW_BUSY )
               maxerror = 10;
            else
               maxerror = 15;  /* serious hardware problem */
         }
      }
      return TRUE;
   }
}

int scan( int verbose ) {
   int rd=0, wr=0, i;
   char dummy;
   int count=0;

   printf( "I2c scan: ");
   maxerror = 0;
   for( i=0; i<128; i++ )
   {
      wr = !report( SendI2C(2*i, 0, &dummy) );
      rd = !report( ReceiveI2C(2*i+1, 1, &dummy) );
      if (rd || wr)
      {
         count++;
         if( verbose )
            printf("\n");
         printf( "Chip address " );
         if( rd && wr )
            printf( "0x%02x/0x%02x: R/W", 2*i, 2*i+1 );
         else if ( rd )
            printf( "0x%02x: R only", 2*i+1 );
         else
            printf( "0x%02x: W only", 2*i );
         if( verbose )
            identify( 2*i );
         else
            printf( "\n" );
      } else {
         //printf("%02x ", 2*i);
      }
   }
   if( maxerror == 0 && count == 0 ) {
      printf("No chip found!\n");
      return 5;               /* indicate "no replies" */
   }
   else
      return maxerror;
}



int main(int argc, char * argv[]) {
   ULONG ret = 0;
   printf("i2c service tool V1.0\n");

   atexit(done);

   printf("Open %s ...", I2C_LIBNAME);
   I2CBase = (struct Library *)OpenLibrary((const char*)I2C_LIBNAME, 36);
   if (I2CBase) {
      printf("ok (library pointer = 0x%p)\n", I2CBase);
   } else {
      printf("Failed!\n");
      exit(-1);
   }

   printf("Call SetI2CDelay()...");
   ret = SetI2CDelay(40);
   printf("res=%ld\n", ret);

   scan( 1 );
}

void done(void) {
   CloseLibrary(I2CBase);
   I2CBase = NULL;
   printf("Close i2c library...\n");
}
