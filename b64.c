/* File: b64.c
**
** Author: John Howie, (c) Howie Consulting Inc., 2026.
**
** Description
**
**      This is a the source code for two very simple routines that are used
** to convert ASCII to Base64, and Base64 to ASCII
**
** Modifications
**
** 2026-09-16 John Howie        Original.
**
*/

# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include "b64.h"

static char b64_enc [] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
        };

static char b64_enc_url [] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '-', '_'
        };

static int asc_enc [] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x3F,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
        0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
        0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
        0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x3F,
        0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
        0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* char *asctob64 (const char *ascii, bool urlencode)
**
** This routine to used to convert ASCII to Base64. The function takes two
** arguments - the string to convert, and whether or not to use URL encoding.
** If urlencode is true, the returned Base64 uses the base64url format as
** described in RFC 4648, and the trailing '=' padding characters (if any) are
** returned as %3D.
**
** The function allocates memory for the Base64-encoded string it returns. The
** caller is responsible for freeing it up when no longer required
*/

char *asctob64 (const char *ascii, bool urlencode)
{
        char    *encmatrix, *base64 = (char *) 0, *currb64;
        int     asclen, b64len, currasc;

        // Select the encoding matrix we will use based on the urlencode flag

        encmatrix = (urlencode ? b64_enc_url : b64_enc);

        // Get the length of the ASCII string that was passed to us and then
        // calculate the estimated length of the Base64 string we need to
        // allocate mmemory for. The calculation is four bytes of Base64 for
        // every three bytes of ASCII. We need to add up-to two to the the
        // length for padding characters. We also need to add one for a NULL
        // character at the end. While we could very predictably calculate the
        // length, why bother... We over-estimate the requirements

        asclen = strlen (ascii);
        b64len = ((((asclen / 3) +1) * 4) + 2 +1);

        // Allocate memory for the Base64 we will return

        if ((currb64 = base64 = malloc (b64len)) == (char *) 0) {
                // An error occurred, and we could not allocate the memory
                // necessary for the Base64-encoded string. Do nothing here
                // other than return an empty string

                return (char *) 0;
        }

        // Now we go into a loop, processing each group of three characters in
        // the ASCII string

        for (currasc = 0; currasc < asclen; currasc += 3) {
                // Convert the three ASCII bytes to four Base64 bytes using the
                // rules in RFC 4648

                *(currb64 ++) = encmatrix [((ascii [currasc] & 0xFC) >> 2)];
                *(currb64 ++) = encmatrix [(((ascii [currasc] & 0x03) << 4) | ((ascii [currasc +1] & 0xF0) >> 4))];
                *(currb64 ++) = ((currasc +2 > asclen) ? '=' : encmatrix [(((ascii [currasc +1] & 0x0F) << 2) | ((ascii [currasc +2] & 0xC0) >> 6))]);
                *(currb64 ++) = ((currasc +3 > asclen) ? '=' : encmatrix [(ascii [currasc +2] & 0x3F)]);
        }

        // Terminate the Base64 string with a NULL character

        *currb64 = (char) 0;

        // Return the string

        return base64;
}

/* char *b64toasc (const char *b64)
**
** This function is called to convert the input string, which is in Base64, to
** an ASCII string which is returned to the caller
*/

char *b64toasc (const char *base64)
{
        char    *ascii = (char *) 0, *currasc;
        int     b64len, asclen, currb64;

        // Calculate the length that is required to store the decoded Base64
        // string once it is turned into ASCII. Note that we only estimate the
        // length required so we will be slightly over for smaller strings (but
        // never under)

        b64len = strlen (base64);
        asclen = ((((b64len / 4) +1) *3) +1);

        // Allocate memory for the ASCII string

        if ((currasc = ascii = malloc (asclen)) == (char *) 0) {
                // An error occurred, and we could not allocate the memory
                // required. There is not much we can do, here, so just
                // return

                return (char *) 0;
        }

        // Go into a loop, processing the Base64 string, converting four bytes
        // at a time into three ASCII bytes

        for (currb64 = 0; currb64 < b64len; currb64 += 4) {
                // Convert the current four Base64 bytes to three ASCII bytes

                *(currasc ++) = (((asc_enc [base64 [currb64]] & 0x3F) << 2) | ((asc_enc [base64 [currb64 +1]] & 0x30) >> 4));
                *(currasc ++) = ((base64 [currb64 +2]) == '=' ? (char) 0 : (((asc_enc [base64 [currb64 +1]] & 0x0F) << 4) | ((asc_enc [base64 [currb64 +2]] & 0x3C) >> 2)));
                *(currasc ++) = ((base64 [currb64 +3]) == '=' ? (char) 0 : (((asc_enc [base64 [currb64 +2]] & 0x03) << 6) | asc_enc [base64 [currb64 +3]] & 0x3F));
        }

        // Terminate the ASCII string with a NULL

        *currasc = (char) 0;

        // Return the decoded ASCII string

        return ascii;
}
