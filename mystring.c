/*
 Mystring opaque object interface
 */
#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"

struct _MYSTRING
{
    int size;
    char* data;
    int capacity;
};

typedef struct _MYSTRING My_string;
/*****************************************************************************
 * Create a "default" new MYSTRING opaque object (with no initial data)
 *
 *
 * @return
 *   upon failure: NULL
 *     The function fails if memory to complete the operation is not available
 *
 *   upon success:
 *     A MYSTRING handle to a default string object that has:
 *       size     = 0
 *       capacity = MYSTRING_STARTING_CAPACITY
 ****************************************************************************/
MYSTRING mystring_init_default(void)
{
    My_string* temp;
    temp = (My_string*)malloc(sizeof(My_string));
    if(temp != NULL)
    {
        temp->size = 0;
        temp->capacity = MYSTRING_STARTING_CAPACITY;
        temp->data = (char*)malloc(sizeof(char)*temp->capacity);
        if(temp->data == NULL)
        {
            free(temp);
            temp = NULL;
        }
        
    }
    return (MYSTRING) temp;
}

/*****************************************************************************
 * Create a new MYSTRING opaque object from a given c-string (null terminated
 * array).
 *
 * @param c_string
 *   The null-terminated C string used to initialize the new MYSTRING
 *   object.
 *
 * @return
 *   upon failure: NULL
 *     The function fails if memory to complete the operation is not available
 *
 *   upon success:
 *     A MYSTRING handle to a string object representing the given c-string
 *     that has:
 *       size     = number of characters in the initializer, c_string,
 *                  not including its null terminator
 *       capacity = number of characters in the initializer, c_string,
 *                  not including its null terminator
 *
 * @note
 *   Internally, the string is NOT stored with a null terminator. Only enough
 *   memory for the string's length (not including its null terminator) is
 *   allocated, and the c_string's characters are copied into that memory.
 ****************************************************************************/
MYSTRING mystring_init_c_string(const char * const c_string)
{
    My_string *temp;     //use this way..
    temp = (My_string*) malloc(sizeof(My_string));
    int i = 0;
    
    int cSize = 0;
    
    while(c_string[cSize] != '\0') cSize++;
    
    if(temp != NULL)
    {
        temp->size = cSize;
        temp->capacity = cSize;
        temp->data = (char*) malloc(sizeof(char) * temp->capacity);
        if(temp->data == NULL)
        {
            free(temp);
            temp = NULL;
        }
        for(i=0; i<cSize; i++)
        {
            temp->data[i] = c_string[i];
        }
        /*
         if(temp->data == NULL)
         {
         free(temp);
         temp = NULL;
         }
         */
    }
    return (MYSTRING) temp;
}

/*****************************************************************************
 * Destroy a MYSTRING opaque object and set the handle to NULL.
 * Will not attempt to destroy an object if the handle is NULL.
 *
 * @param p_str
 *   Address of a MYSTRING handle so that the function can free the MYSTRING
 *   object, and then change the passed variable to NULL.
 ****************************************************************************/
void mystring_destroy(MYSTRING* p_hString)
{
    My_string* temp = *p_hString;
    if(temp != NULL)
    {
        free(temp->data);
        free(temp);
    }
    *p_hString = NULL;
    return;
}


/*****************************************************************************
 * Retrieves the number of characters that the current string holds.
 *
 * @param hString
 *   MYSTRING handle to the object that you want to query the size of.
 *
 * @return
 *   upon success:
 *     The size of the string is returned
 *
 *   upon failure:
 *    -1 is returned (e.g., hString is NULL)
 ****************************************************************************/
int mystring_size(MYSTRING hString)
{
    MYSTRING s = (MYSTRING)hString;
    int size = -1;
    if (s != NULL) size = s->size;
    return size;
}

/*****************************************************************************
 * Retrieves the total number of characters that the string can contain before
 * it has to do an internal resize operation of its dynamic array.
 *
 * @param hString
 *   MYSTRING handle to the object that you want to query the capacity of.
 *
 * @return
 *   upon success:
 *     The capacity of the string is returend
 *
 *   upon failure:
 *     -1 is returned (e.g., hString is NULL)
 ****************************************************************************/
int mystring_capacity(MYSTRING hString)
{
    MYSTRING s = (MYSTRING)hString;
    int capacity = -1;
    if(s != NULL) capacity = s->capacity;
    return capacity;
    
}

/*****************************************************************************
 * Outputs the string to the given output file handle.
 *
 * @param hString
 *   MYSTRING handle to the object whose string data is to be output.
 *
 * @param out
 *   output file handle as the target for writing the string.
 *
 * @return
 *   upon success:
 *     MYSTRING_STATUS_SUCCESS
 *
 *   upon failure:
 *     MYSTRING_STATUS_ERROR (e.g., either of the hString or out handles are
 *     invalid)
 ****************************************************************************/
MyString_Status mystring_output(MYSTRING hString, FILE* out)
{
    int i=0;
    
    if(out != NULL && hString != NULL)
    {
        while(i < (hString->size))
        {
            fprintf(out, "%c",hString->data[i]);
            i++;
        }
        return MYSTRING_STATUS_SUCCESS;
    }
    return MYSTRING_STATUS_ERROR;
}

/*****************************************************************************
 * Concatenate a c-string to an existing MYSTRING object. The c-string is
 * concatenated to the end of MYSTRING object's existing string data, which
 * may require enlarging the MYSTRING object's capacity.
 *
 * Upon failure, the MYSTRING object must be left unaltered.
 *
 * @param hString
 *   The MYSTRING handle which is the destination of the concatenation
 *
 * @param c_string_Source
 *   The c-string which is the source of the concatenation
 *
 * @return
 *   upon success:
 *     MYSTRING_STATUS_SUCCESS
 *
 *   upon failure:
 *     MYSTRING_STATUS_ERROR (e.g., either of the hString or out handles are
 *     invalid)
 ****************************************************************************/
MyString_Status mystring_concatenate_c_string(MYSTRING hString,
                                              char * c_string)
{
    int string_size = 0;
    int new_size_needed = 0;
    int i=0;
    int j=0;
    
    MYSTRING temp = mystring_init_default();
    
    if(hString == NULL || c_string==NULL) return MYSTRING_STATUS_ERROR;
    
    while(c_string[string_size] != '\0') string_size++;
    new_size_needed = hString->size + string_size;
    
    temp->data = (char*)realloc(temp->data, sizeof(char)*new_size_needed);
    if(temp->data == NULL)
    {
        mystring_destroy(&temp);
        temp = NULL;
        return MYSTRING_STATUS_ERROR;
    }
    for(i=0; i<hString->size; i++) temp->data[i] = hString->data[i];
    
    if(new_size_needed > hString->capacity) hString->capacity = new_size_needed;
    
    for(i=hString->size; i<new_size_needed; i++)
    {
        temp->data[i] = c_string[j];
        j++;
    }
    
    hString->size = new_size_needed;
    hString->data = (char*)realloc(hString->data, sizeof(char)*hString->capacity);
    if(hString->data == NULL)
    {
        free(hString);
        mystring_destroy(&temp);
        return MYSTRING_STATUS_ERROR;
    }
    
    for(i=0; i<hString->size;i++) hString->data[i] = temp->data[i];
    mystring_destroy(&temp);
    return MYSTRING_STATUS_SUCCESS;
}

/*****************************************************************************
 * Concatenate a source MYSTRING object to an existing, destination MYSTRING
 * object. The source object's string data is concatenated to the end of the
 * destination object's string data, which may require enlarging the
 * destination object's capacity. The source object is not altered.
 *
 * Upon failure, the destination object must be left unaltered.
 *
 * @param hStringDest
 *   The MYSTRING handle which is the destination of the concatenation
 *
 * @param hStringSource
 *   The MYSTRING handle which is the source of the concatenation
 *
 * @return
 *   upon success:
 *     MYSTRING_STATUS_SUCCESS
 *
 *   upon failure:
 *     MYSTRING_STATUS_ERROR (e.g., either of the hString handle or c_string
 *     pointer is null)
 ****************************************************************************/
MyString_Status mystring_concatenate_mystring(MYSTRING hStringDest,
                                              MYSTRING hStringSource)
{
    int i=0;
    int j=0;
    int new_size_needed = hStringDest->size + hStringSource->size;
    MYSTRING temp = mystring_init_default();
    
    temp->data = (char*)realloc(temp->data, sizeof(char)*hStringDest->size);
    
    for(i=0;i<hStringDest->size;i++) temp->data[i] = hStringDest->data[i];
    
    if(hStringDest == NULL || hStringSource == NULL) return MYSTRING_STATUS_ERROR;
    
    if(hStringDest->capacity < new_size_needed)
    {
        free(hStringDest->data);
        hStringDest->data = (char*)malloc(sizeof(char)*new_size_needed);
        if(hStringDest->data == NULL)
        {
            free(hStringDest->data);
            hStringDest->data = (char*)malloc(sizeof(char)*hStringDest->size);
            return MYSTRING_STATUS_ERROR;
        }
        hStringDest->capacity = new_size_needed;
    }
    
    for(i=0; i<hStringDest->data[i]; i++) hStringDest->data[i] = temp->data[i];
    
    for(i=hStringDest->size; i < new_size_needed; i++)
    {
        hStringDest->data[i] = hStringSource->data[j];
        j++;
    }
    free(temp->data);
    free(temp);
    
    hStringDest->size = new_size_needed;
    
    return MYSTRING_STATUS_SUCCESS;
}

/*****************************************************************************
 * Truncate a string object to a given length. No change is made if the
 * specified new length is greater than the object's current length.
 *
 * @param hString
 *   MYSTRING handle of the object whose string data is to be truncated
 *
 * @param newMaxLen
 *   new length of the string (if greater than the current length)
 *
 * @return
 *   upon success:
 *     MYSTRING_STATUS_SUCCESS
 *
 *   upon failure:
 *     MYSTRING_STATUS_ERROR (e.g., either of the hString or out handles are
 *     invalid)
 ****************************************************************************/
MyString_Status mystring_truncate(MYSTRING hString, int newMaxLen)
{
    if(hString->data == NULL || newMaxLen < 0 || hString->size == 0) return MYSTRING_STATUS_ERROR;
    
    if(hString->size >= newMaxLen) hString->size = newMaxLen;
    
    return MYSTRING_STATUS_SUCCESS;
}

/**
 * Append one character to the end of the string, simulating a stack push
 * operation.
 *
 * @param hString
 *   MYSTRING handle of the object whose string data is being manipulated
 *
 * @param ch
 *   The character to be appended to the string object
 *
 * @return
 *   upon success:
 *     MYSTRING_STATUS_SUCCESS
 *
 *   upon failure:
 *     MYSTRING_STATUS_ERROR (e.g., the hString handle is invalid)
 */
MyString_Status mystring_push(MYSTRING hString, char ch)
{
    char char_array[2];
    char_array[0] = ch;
    char_array[1] = '\0';
    if(hString == NULL) return MYSTRING_STATUS_ERROR;
    return  mystring_concatenate_c_string(hString, char_array);
}

/**
 * Remove one character from the end of the string, simulating a stack pop
 * operation.
 *
 * @param hString
 *   MYSTRING handle of the object whose string data is being manipulated
 *
 * @return
 *   If the string object has size at least one, the last character is removed
 *   from the end of the string (i.e, the string is truncated to one less than
 *   its previous size), and the removed character is returned.
 *
 *   If the string object has size 0, a null character ('\0') is returned.
 */
char mystring_pop(MYSTRING hString)
{
    char popped_character;
    if(hString->size == 0) return '\0';
    else if(hString->size == 1)
    {
        popped_character = hString->data[hString->size-1];
        hString->size =0;
        return popped_character;
    }
    else
    {
        popped_character = hString->data[hString->size-1];
        hString->size -= 1;
        return popped_character;
    }
}


/**
 * Obtain the character that is at the end of the string.
 *
 * @param hString
 *   MYSTRING handle of the object whose string data is being manipulated
 *
 * @return
 *   If the string object has size at least one, the last character of the
 *   string is returned.
 *
 *   If the string object has size 0, a null character ('\0') is returned.
 *
 *   In either case, the string object is not modified in any way.
 */
char mystring_peek(MYSTRING hString)
{
    if(hString->size == 0) return '\0';
    return hString->data[(hString->size)-1];
}
/**
 * Get a character from a given position in the string
 *
 * @param hString
 *   MYSTRING handle of the object whose string data is being manipulated
 *
 * @param index
 *   Index of the character to be retrieved
 *
 * @return
 *   If the string object has size at least one greater than the requested
 *   index (i.e., the requested index exists in the string), the character at
 *   that position is returned.
 *
 *   If the requested index does not exist in the string, a null character
 *   ('\0') is returned.
 */
char mystring_get(MYSTRING hString, int index)
{
    if(hString==NULL) return '\0';
    return hString->data[index];
}

/**
 * Put a character at a given index in the string. A character may only be
 * placed at an index where a character currently exists. This function may
 * not be used to increase the size of the string.
 *
 * @param hString
 *   MYSTRING handle of the object whose string data is being manipulated
 *
 * @param index
 *   Index where the character should be placed
 *
 * @param ch
 *   The character to be put at the specified index
 *
 * @return
 *   upon success:
 *     MYSTRING_STATUS_SUCCESS
 *
 *   upon failure:
 *     MYSTRING_STATUS_ERROR (index out of bounds)
 *
 *
 */
MyString_Status mystring_put(MYSTRING hString, int index, char ch)
{
    if(index < 0) return MYSTRING_STATUS_ERROR;
    if(hString->size < index+1) return MYSTRING_STATUS_ERROR;
    hString->data[index] = ch;
    return MYSTRING_STATUS_SUCCESS;
}

/**
 * Create a new MYSTRING opaque object from a substring of an existing, source
 * MYSTRING opaque object. The source object's string data, beginning at the
 * specified index and extending for the specified length, is used as the new
 * object's string data. The new object's size and capacity are set to exactly
 * the number of characters copied from the source object to the new
 * object. It is an error if the specified index is out of bounds, or if the
 * number of characters specified by length, beyond the index, is out of
 * bounds.
 *
 * @param hStringSource
 *   The MYSTRING handle which contains the substring to be copied to this new
 *   object.
 *
 * @param index
 *   The index, within the source object, of the beginning of the string data
 *   with which to initialize this new object.
 *
 * @param length
 *   The number of characters, beginning at index, to be copied from the
 *   source object, with which to initialize this new object.
 *
 * @return
 *   upon failure: NULL
 *     The function fails if memory to complete the operation is not
 *     available, or if the index is out of bounds within the source object,
 *     or if the length is less than one or greater than the number of
 *     characters available in the source object beyond the specified index.
 *
 *   upon success:
 *     A MYSTRING handle that has size and capacity set to the specified
 *     length, and string data copied from the source object.
 */
MYSTRING mystring_init_substring(MYSTRING hStringSource, int index, int length)
{
    My_string* hString;
    if(index<0 || length<1 || hStringSource->size < length) return MYSTRING_STATUS_ERROR;
    hString = (My_string*)malloc(sizeof(My_string));
    if(hString != NULL)
    {
        hString->size = length;
        hString->capacity = length;
        hString->data = (char*)malloc(sizeof(char)*length);
        if(hString->data == NULL)
        {
            free(hString);
            hString= NULL;
        }
    }
    int i;
    int j=0;
    
    for(i=index;j<length; i++)
    {
        hString->data[j] = hStringSource->data[i];
        j++;
    }
    return hString;
}


/**
 * Convert a string in a MYSTRING object to a c_string
 *
 * @param hString
 *   The MYSTRING handle of the string containing the data to be converted to
 *   a c_string
 *
 * @param c_string_arr
 *   The address of a statically- or dynamically-allocated character array
 *   into which the c_string will be placed.
 *
 * @param arrSize
 *   The maximum number of characters that can be copied to c_string_arr. The
 *   array will ALWAYS be null-terminated, even if that means that less than
 *   the entire string data from the MYSTRING object can be copied given this
 *   size.
 *
 * @return
 *   upon failure:
 *     NULL, e.g., hString or c_string_arr is null; arrSize is less than one
 *
 *   upon success:
 *     The address of the beginning of the c_string_arr is returned, allowing
 *     this function to be used as an expression value, e.g., to pass to
 *     printf.
 */
char * mystring_to_c_string(MYSTRING hString, char c_string_arr[], int arrSize)
{
    int i;
    
    if(arrSize <= hString->size)
    {
        for(i=0; i<arrSize-1; i++)
        {
            c_string_arr[i] = hString->data[i];
        }
        c_string_arr[i] = '\0';
    }
    if(arrSize > hString->size)
    {
        for(i=0; i < hString->size; i++)
            c_string_arr[i] = hString->data[i];
        c_string_arr[i] = '\0';
    }
    if(hString == NULL || arrSize<1 || c_string_arr == NULL) return NULL;
    return &c_string_arr[0];
}

/**
 * Read input from a given file, until being told to stop reading characters,
 * or until EOF.
 *
 * IMPORTANT:
 *   The input replaces, entirely, any data that was already in the MYSTRING
 *   object.
 *
 * @param hString
 *   The MYSTRING handle of the string into which the input data should be
 *   placed, replacing any prior data in this string
 *
 * @param hFile
 *   A FILE* handle from which input should be read
 *
 * @param bIgnoreLeadingWhiteSpace
 *   If true, ignore any leading white space. Leading white space means any
 *   spaces, horizontal or vertical tabs, and newlines that occur before any
 *   non-whitespace character has been encountered. The function pointed to be
 *   fTerminate is not called for leading white space, when this argument is
 *   true, but is called when this argument is false.
 *
 * @param fTerminate
 *   A pointer to a function to be called upon reading each character, to
 *   determine whether the character just read is a "terminator", i.e.,
 *   whether mystring_input should return after storing (or not storing, as
 *   indicated by *pbDiscardChar) this character. This function will be called
 *   as each character is read. It must return TRUE (non-zero) if the
 *   just-read character should terminate input. It should set
 *   *pbDiscardChar, as described below, before returning.
 *
 *   The fTerminate function's signature is:
 *     int (* fTerminate)(char ch, int * pbDiscardChar);
 *
 *   with the following documentation:
 *     @param ch
 *       The character that was just read. fTerminate should determine whether
 *       this character is a terminator.
 *
 *     @param pbDiscardChar
 *       Whether this character should be added to the MYSTRING object. Prior
 *       to returning TRUE (non-zero) or FALSE (zero), fTerminate should set
 *       *pbDiscardChar to one of the following values:
 *
 *         0: Do not discard the character. Add it to the MYSTRING object
 *         1: Discard the character. Do not add it to the MYSTRING object
 *
 *     @return
 *       TRUE if this character should cause termination of input; FALSE
 *       otherwise.
 *
 *   The fTerminate function is not called upon EOF.
 *
 * @return
 *   upon failure:
 *     MYSTRING_STATUS_ERROR (any of the arguments are null)
 *
 *   upon success:
 *     MYSTRING_STATUS_SUCCESS
 *
 * @example 1 (fairly simple)
 *   Let's say that you want to accept all "words", i.e., sequences of
 *   characters separated by white space. The white space character should be
 *   discarded. You could write a simple termination function to accept
 *   characters other than white space, discard white space, and terminate
 *   upon reaching any white space. The function must meet the signature
 *   requirements of fTerminate:
 *
 *   int acceptNonWhite(char ch, int * pbDiscardChar)
 *   {
 *     // If the character is white space...
 *     if (isspace(ch))
 *     {
 *       // ... then we want it to be discarded, ...
 *       *pbDiscardChar = TRUE;
 *
 *       // ... and we want to terminate input to this MYSTRING object
 *       return TRUE;
 *     }
 *
 *     // In any other case (non-white), do not discard the character, ...
 *     *pbDiscardChar = FALSE;
 *
 *     // ... and do not terminate input
 *     return FALSE;
 *   }
 *
 *   // Now we can call mystring_input using that function:
 *   ...
 *   status = mystring_input(hString, stdin, acceptNonWhite);
 *   ...
 *
 * @example 2 (more sophisticated)
 *   Assume that you want to accept all sequences of digits, ignoring any even
 *   digit. Any other character is to terminate input and that chraracter is
 *   to be ignored. In this scenario, if the input were "12345 23476abc" then
 *   you want the MYSTRING object to contain "135", and the next time this
 *   function was called with the same arguments, it would retrieve "37". On
 *   the first call, the space is discarded. ON the second call, the 'a' is
 *   discarded. You might define a function such as acceptOddDigits, that
 *   meets the signature requirements of fTerminate:
 *
 *   int acceptOddDigits(char ch, int * pbDiscardChar)
 *   {
 *     // If the character is a digit, ...
 *     if (isdigit(ch))
 *     {
 *       // First, convert the character containing the digit to its numeric
 *       // value.
 *       ch -= '0';
 *
 *       // Discard the character if it is even; do not discard it otherwise.
 *       *pbDiscardChar = (ch % 2 == 0);
 *
 *       // Do not terminate. We want to keep retrieving input.
 *       return FALSE;
 *     }
 *
 *     // It's not a digit. Throw away the character, ...
 *     *pbDiscardChar = TRUE;
 *
 *     // and terminate input into this MYSTRING object
 *     return TRUE;
 *   }
 *
 *   // Now we can call mystring_input using that function:
 *   ...
 *   status = mystring_input(hString, stdin, acceptOddDigits);
 *   ...
 */
MyString_Status mystring_input(MYSTRING hString,
                               FILE * hFile,
                               int bIgnoreLeadingWhiteSpace,
                               int (* fTerminate)(char ch,
                                                  int * pbDiscardChar))
{
    char character_grabbed;
    int number_of_conversions;
    int if_discard=0;
    
    hString->size = 0;
    
    if(hString == NULL || fTerminate==NULL || hFile==NULL) return MYSTRING_STATUS_ERROR;
    if(bIgnoreLeadingWhiteSpace)
    {
        number_of_conversions = fscanf(hFile, " %c", &character_grabbed);
        ungetc(character_grabbed, hFile);
    }
    number_of_conversions = fscanf(hFile, "%c", &character_grabbed);
    while(number_of_conversions == 1 && !fTerminate(character_grabbed, &if_discard))
    {
        if(if_discard == 0)
        {
            mystring_push(hString,character_grabbed);
        }
        number_of_conversions = fscanf(hFile, "%c", &character_grabbed);
    }
    if(number_of_conversions==1 && if_discard == 0) mystring_push(hString, character_grabbed);
    if(number_of_conversions != 1 && number_of_conversions != EOF) return MYSTRING_STATUS_ERROR;
    return MYSTRING_STATUS_SUCCESS;
}
/*********************************************
 * Makes a deep copy of a mystring object
 *
 ***********************************************/
void string_assignment(void** left, void* right)
{
    char* temp;
    int i;
    My_string* real_left = (My_string*) *left;
    My_string* real_right = (My_string*) right;
    
    if(real_left == NULL) *left = real_left = (My_string*) mystring_init_default();
    if(real_left != NULL && real_right != NULL)
    {
        //both objects "supposedly" exist so try to do a deep copy
        //if the left side is not big enough to store the right's data then
        //make it bigger.
        if(real_left->capacity < real_right->size)
        {
            real_left->capacity = real_right->size;
            temp = (char*)malloc(sizeof(char)*real_left->capacity);
            if(temp == NULL)
            {
                fprintf(stderr, "I ran out of memory and I give up...\n");
                exit(1);
            }
            free(real_left->data);
            real_left->data = temp;
        }
        //fix left's size to match right's
        real_left->size = real_right->size;
        //copy over all the data
        for(i=0; i<real_right->size; i++)
            real_left->data[i] = real_right->data[i];
    }
    else
    {
        //if right is NULL we can't really do the right thing so let's just crash for now
        fprintf(stderr, "Tried to do string_assignment but the right hand side was NULL\n");
        exit(1);
    }
    
}
/*******************************************************************************
 * String compares compare lexicographical order of two mystring objects
 *
 *@param MYSTRING one
 * First mystring to be compared lexicographically
 *
 *@param MYSTRING two
 * Second mystring to be compared lexicographically
 *
 *@return
 * if MYSTRING one is lexicographically greater return 1
 * if MYSTRING two is lexicographically greater return -1
 * if MYSTRING objects  are equal (same word) return 0;
 *
 *
 *******************************************************************************/
int mystring_compare(MYSTRING one, MYSTRING two)
{
    My_string* one_real = (My_string*)one;
    My_string* two_real = (My_string*)two;
    
    int i = 0;
    while( i < one_real->size)
    {
        if(two_real->size < i)
            return 1;
        else if(one_real->data[i] < two_real->data[i])
            return -1;
        else if(one_real->data[i] > two_real->data[i])
            return 1;
        i++;
    }
    
    return 0;
}


/**************************************
 * Usable wrapper for mystring_destroy
 **************************************/
void string_destructor(void **object)
{
    mystring_destroy((MYSTRING*)object);
    return;
}