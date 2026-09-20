**lib64** - An ASCII / Base64 encoding library

This is a small library of two routines:

  asctob64 - converts an ASCII string to Base64
  b64toasc - converts a Base64 string to ASCII

This library was written simply because MacOS and FreeBSD
do not have these (or equivalent) routines in the base
distribution. Many libraries such as CURL(3) implement
these functions for their own needs, but do not expose a
simple API

**LIMITATIONS** 

None known.
