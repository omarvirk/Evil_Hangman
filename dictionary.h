#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"
#include "generic_vector.h"

/*****************************************************************
 * Reads in from a dictionary.txt files, words until EOF.
 * Each word is placed an interior vector unique to the word length.
 * Each vector of mystrings of a particular word length are at size-1
 * index of an outer vector.
 * The outer vector is returned for use to another program.
 *****************************************************************/
GENERIC_VECTOR dictionary(void);