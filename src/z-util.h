/* File z-util.h */

/*
 * Copyright (c) 1997 Ben Harrison
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.
 */

#ifndef INCLUDED_Z_UTIL_H
#define INCLUDED_Z_UTIL_H

#include "h-basic.h"


/*
 * Extremely basic stuff, like global temp and constant variables.
 * Also, some very useful low level functions, such as "streq()".
 * All variables and functions in this file are "addressable".
 */


/**** Available variables ****/

/* From 283 for mpw */
extern errr errr_tmp;

/* A cptr to the name of the program */
extern cptr argv0;


/* Aux functions */
extern void (*plog_aux)(cptr);
extern void (*quit_aux)(cptr);
extern void (*core_aux)(cptr);


/**** Available Functions ****/

/* Test equality, prefix, suffix */
extern bool streq(cptr s, cptr t);
extern bool prefix(cptr s, cptr t);
extern bool suffix(cptr s, cptr t);


/* Print an error message */
extern void plog(cptr str);

/* Exit, with optional message */
extern void quit(cptr str);

/* Dump core, with optional message */
extern void core(cptr str);

/* Copy a string */
extern size_t my_strcpy(char *buf, const char *src, size_t bufsize);
extern size_t my_strcat(char *buf, const char *src, size_t bufsize);



#endif


