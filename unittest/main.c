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

struct Library * i2clib = NULL;
void done(void);

#define COLOR02   "\033[32m"
#define COLOR03   "\033[33m"
#define NORMAL    "\033[0m"
#define CLRSCR    "\033[1;1H\033[2J"
#define CURSOR_UP "\033[1A"
#define TEST_ENTERED() printf("%s...", __func__);
#define TEST_LEAVE(a)  printf("%s\n", (a) ? "success" : "failed!");


int main(int argc, char * argv[]) {
   printf("i2c unittest\n");

   atexit(done);

   i2clib = (struct Library *)OpenLibrary("i2c.library",0);

   printf("Open i2c library = 0x%p\n", (APTR)i2clib);
}

void done(void) {
   CloseLibrary(i2clib);
   i2clib = NULL;
   printf("Close i2c library...\n");
}
