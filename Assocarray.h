#ifndef MY_SET_H
#define MY_SET_H
#include "mystring.h"
#include "generic_vector.h"

enum boolean
{
    FALSE, TRUE
};

typedef enum boolean Boolean;
typedef struct _MY_SET *MY_SET;

/*******************************************************
 * Creates a handle to a MY_SET Object
 *
 * @return
 *  If successful will return a handle to a MY_SET object
 *   If Failure will return NULL
 *******************************************************/
MY_SET my_set_init_default();

/*******************************************************
 * Takes a handle to and determines if a given item
 * exists in a set or not
 *
 * @param hMY_SET
 *      Handle to a set object
 *
 * @param item
 *      Item to be determined it exists in the set or not
 *y
 * @return
 *      Returns TRUE if item is in the set. Exits on error
 *
 ********************************************************/
Boolean my_set_is_element(MY_SET hMY_SET, MYSTRING item); //returns TRUE if item is in the set; exit on error


Boolean my_set_is_empty(MY_SET hMY_SET); //returns TRUE if the set is empty; exit on error


int my_set_size(MY_SET hMY_SET); //returns the number of elements in the set; -1 on error


Boolean my_set_traverse(MY_SET hMY_SET, void (*visit)(MYSTRING, void*)); // call visit on each node in the set. (Use an inorder traversal of your AVL tree)


Boolean my_set_add(MY_SET hMY_SET, MYSTRING key, MYSTRING word); //adds item to the set if it is not already in it.

Boolean my_set_remove(MY_SET hMY_SET, MYSTRING item);//find and remove item from the set if it is present otherwise ignore.


void my_set_destroy(MY_SET* p_hMY_SET);//destroy the data structure and set the handle pointed to by the argument to NULL.

int my_set_height(MY_SET hMY_SET);

GENERIC_VECTOR largest_word_list(MY_SET hSet); //gets the node of a set with the largest vector size

MYSTRING get_root_key(MY_SET hMY_SET); //returns the mystring of the root node;

GENERIC_VECTOR get_specific_list(void* words);
#endif