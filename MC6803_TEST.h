
#ifndef __MAIN_H_INCLUDE
#define __MAIN_H_INCLUDE

#ifndef EXECUTION_DELAY
#	define EXECUTION_DELAY 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <libMC6803E.h>
#include "Image.h"

uint8_t * loadFile(char *, unsigned int);
int GetResponse(char char_true, char char_false, int default_resp, char * request);

#endif
