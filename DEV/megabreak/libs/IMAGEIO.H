/*
    Image I/O - Functions for reading and writing graphic files

    Last updated: 28/7-97
*/

#include <fstream.h>
#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include "standard.h"               // Standard definitions

// Standard image structure definition

struct image {
    unsigned int width;
    unsigned int height;
    byte *pointer;
};

#include "rawread.h"                // Functions for reading dat-files
#include "pcxfunc.h"                // Functions for reading pcx-files

