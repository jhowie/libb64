/* File: asctob64.c
**
** Author: John Howie, (c) Howie Consulting Inc., 2026.
**
** Description
**
**      This small program takes at least one argument, the ASCII string to
** convert to Base64. The string can be enclosed in quotes if it contains
** spaces. An optional flag, -u, tells the program to use URL encoding when
** converting to Base64.
**
** Modifications
**
** 2026-09-19 John Howie        Original.
**
*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "b64.h"

int main (int argc, char *argv []);
void DisplayHelp (void);

int main (int argc, char *argv [])
{
        int     c;
        bool    urlencode = false;
        char    *base64;

        // Check if we got any flags on the command line

        while ((c = getopt (argc, argv, "hu")) != -1) {
                // Check to see what command line switch / flag we got

                switch (c) {
                case 'h':
                        // The user wants to see some help information

                        DisplayHelp ();
                        return 0;
                        break;

                case 'u':
                        // The user wants to URL encode the Base64 string

                        urlencode = true;
                        break;

                default:
                        // We got an unknown command line switch, just display
                        // help information and return

                        fprintf (stderr, "%s: unknown option (%c)\n", argv [0], c);
                        DisplayHelp ();
                        return 1;
                        break;
                }
        }

        // Update argc and argv, so we can get the remaining argument, the
        // string to convert to Base64

        argc -= optind;
        argv += optind;
        
        // Check that we have one argument

        if (argc != 1) {
                // We are missing the mandatory argument. Display help and
                // return

                DisplayHelp ();
                return 1;
        }

        // Convert the argument to Base 64, display it, free up the string, and
        // then return

        base64 = asctob64 (argv [0], urlencode);
        printf ("%s\n", base64);
        free (base64);
        return 0;
}

/* void DisplayHelp (void
**
** This function is called to display help information
*/

void DisplayHelp (void)
{
        printf ("Usage: asctob64 [-hu] <string>\n");
        printf ("\n");
        printf ("This program converts the ASCII string passed on the command line to Base64, and\n");
        printf ("prints out the result.\n");
        printf ("\n");
        printf ("The command accepts the following flags:\n");
        printf ("\n");
        printf ("\t-h\tDisplays this help information and then returns\n");
        printf ("\t-u\tUses the URL encoding rules when converting the string\n");
        printf ("\n");
}
