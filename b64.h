/* File: b64.h
**
** Author: John Howie, (c) Howie Consulting Inc., 2026.
**
** Description
**
**      This is a the header file for two very simple ASCII to Base64 and
** Base64 to ASCII conversion routines.
**
** Modifications
**
** 2026-09-16 John Howie        Original.
**
*/

# ifndef __B64_H__
# define __B64_H__

char *asctob64 (const char *ascii, bool urlencode);
char *b64toasc (const char *b64);

# endif // __B64_H__
