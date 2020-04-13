/*
 * i2clib.h
 *
 *  Created on: 12 Apr 2020
 *      Author: Heiko Pruessing
 */

#ifndef __I2CLIB_H_
#define __I2CLIB_H_

#include "i2c_library.h"


typedef struct {
   struct Library library;
   BPTR           segmentList;
   //TODO:
} I2CLibrary;

#endif /* I2CLIB_H_ */
