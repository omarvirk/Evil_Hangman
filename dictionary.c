/************************************************************************************
 * Purpose: Program that creates a vector of vectors of MYSTRING* The outer vector contains a list of words by length of 1-23.
 * This program takes each word from a dictionary file named dictionary.txt
 * and assign it the appropriate vector of mystring objects.
 *
 * By: Andrew Beane
 * Date: 4-1-14
 *
 **************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "mystring.h"
#include "generic_vector.h"
#include "dictionary.h"
/*********************************FUNCTION DECLARATIONS******************************/

/***************************************************************
 * Reads a character to determine if the character is a terminator
 * character
 *
 * @param ch
 *   Character Read. If the character is a space character or EOF is terminated.
 *
 * @param discard
 *     If the character is a space or EOF then it is discarded.
 *
 * @return
 *     Return TRUE if the character read should cause termination. Return FALSE otherwise
 ***************************************************************/
int if_terminate(char ch, int* discard);

/*****************************************************************
 * Reads in from a dictionary.txt files, words until EOF.
 * Each word is placed an interior vector unique to the word length.
 * Each vector of mystrings of a particular word length are at size-1
 * index of an outer vector.
 * The outer vector is returned for use to another program.
 *
 *****************************************************************/
GENERIC_VECTOR dictionary(void)
{
    GENERIC_VECTOR outer_vector;
    GENERIC_VECTOR inner_vector;
    Generic_vector_functions vector_functions;
    Generic_vector_functions mystring_functions;
    GENERIC_VECTOR *ptr_temp;
    
    MYSTRING hString;
    int white_space;
    int i;
    int size;
    
    FILE* dictionary;
    
    vector_functions.assignment = vector_assignment;
    vector_functions.destructor = vector_destructor;
    
    outer_vector = generic_vector_init_default(vector_functions);
    
    mystring_functions.assignment = string_assignment;
    mystring_functions.destructor = string_destructor;
    
    for(i=0; i<29; i++)
    {
        inner_vector = generic_vector_init_default(mystring_functions);
        generic_vector_push_back(outer_vector, inner_vector);
        generic_vector_destroy(&inner_vector);
    }
    //fprintf(stdout,"%d\n", generic_vector_size(outer_vector));
    hString = mystring_init_default();
    
    dictionary = fopen("dictionary.txt", "r");
    
    /* Grabs words from the dictionary file while there are still words to grab.
     Pushes the word to an appropriate generic vector depending on word size, if an
     interior vector exists for that word size.
     */
    
    white_space = 1;
    while(mystring_input(hString, dictionary, white_space, if_terminate)== MYSTRING_STATUS_SUCCESS)
    {
        size = mystring_size(hString) - 1;
        // if no letters, ...
        if (size < 0)
        {
            // then we're at end of file so don't need this mystring object
            mystring_destroy(&hString);
            break;
        }
        
        ptr_temp= (GENERIC_VECTOR*)generic_vector_at(outer_vector, size);
        if (ptr_temp == NULL)
        {
            // should not ever reach here since all vectors for every size
            // exist.
            printf("generic_vector_at returned null for size=%d\n", size);
            mystring_destroy(&hString);
            fclose(dictionary);
            return outer_vector;
        }
        generic_vector_push_back(*ptr_temp, hString);
    }
    /*Destroy mystring after all words pushed to interior vectors of mystring*/
    mystring_destroy(&hString);
    
    /*Can close file after all words are read in*/
    fclose(dictionary);
    
    //   generic_vector_destroy(&outer_vector);
    
    return outer_vector;
}

/******************************HELPER FUNCTIONS*******************************/

int if_terminate(char ch, int* discard)
{
    if(ch == EOF || isspace(ch))
    {
        *discard = 1;
        return 1;
    }
    
    *discard = 0;
    return 0;
}