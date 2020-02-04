

#include <stdio.h>
#include <stdlib.h>
#include "assocarray.h"
#include "mystring.h"
#include "status.h"
#include "generic_vector.h"

typedef struct node Node;
typedef struct my_set My_set;
struct node
{
    MYSTRING data;
    GENERIC_VECTOR words;
    Node* left;
    Node* right;
    int height;
};
struct my_set
{
    Node* head;
};
/*********************************************
 *Function Declarations
 *  Used for functions implicity to the .c file and not
 *  necessary for user interface
 *********************************************/
Boolean find(Node* head, MYSTRING item);
Boolean  insert(Node** head, MYSTRING key, MYSTRING word);
void destroy(Node* root);
void in_order_traverse(Node* head, void (*visit)(MYSTRING, void*));
Node*  set_remove(Node* head, MYSTRING item);
int size(Node* head);
int height(Node* head);
int max(int a, int b);
Node* right_rotate(Node* head);
Node* left_rotate(Node* head);
Node* findmin(Node* head);
Node* new_node(MYSTRING key);
GENERIC_VECTOR find_max(GENERIC_VECTOR current_max, GENERIC_VECTOR hVector);
void traverse_for_largest(Node* head, Node **max, int *temp_max);
/***************************************************
 * Returns difference of left and right tree heights
 ***************************************************/
int balance_factor(Node *head);

/*******************************************************
 * Creates a handle to a MY_SET Object
 *
 * @return
 *  If successful will return a handle to a MY_SET object
 *   If Failure will return NULL
 *******************************************************/
MY_SET my_set_init_default(void)
{
    My_set* temp = (My_set*) malloc(sizeof(My_set));
    if(temp != NULL)
        temp->head = NULL;
    return (MY_SET) temp;
}
/*******************************************************
 * Takes a handle to and determines if a given item
 * exists in a set or not
 *
 * @param hMY_SET
 *      Handle to a set object
 *
 * @param item
 *      Item to be determined it exists in the set or not
 *
 * @return
 *      Returns TRUE if item is in the set. Exits on error
 *
 ********************************************************/
Boolean my_set_is_element(MY_SET hMY_SET, MYSTRING item)
{
    // Silly rabbit...
    My_set* set = (My_set*) hMY_SET;
    if(set == NULL) exit(1);
    return find(set->head,item);
}
Boolean find(Node* head, MYSTRING item)
{
    if(head == NULL) return FALSE;
    else if(mystring_compare(head->data,item) == 0)  return TRUE;
    else
    {
        if(mystring_compare(head->data, item) == 1) return find(head->left, item);
        else if(mystring_compare(head->data,item) == -1) return find(head->right,item);
    }
    return FALSE;
}
Boolean my_set_add(MY_SET hMY_SET, MYSTRING key, MYSTRING word)
{
    My_set* set = (My_set*) hMY_SET;
    if(set == NULL) return FALSE;
    return insert(&set->head,key,word);
}

Node* new_node(MYSTRING key)
{
    Node *pNode = (Node*)malloc(sizeof(Node));
    Generic_vector_functions mystring_functions;
    mystring_functions.assignment = string_assignment;
    mystring_functions.destructor = string_destructor;
    if(pNode != NULL)
    {
        pNode->left = NULL;
        pNode->right = NULL;
        pNode->data = mystring_init_substring(key,0,mystring_size(key));
        pNode->words = generic_vector_init_default(mystring_functions);
        pNode->height = 1;
    }
    return pNode;
}

Boolean insert(Node** head, MYSTRING key, MYSTRING word)
{
    int balance;
    if(*head != NULL && mystring_compare(key, (*head)->data) == 0)
        return generic_vector_push_back((*head)->words,word);
    else
    {
        if(*head == NULL)
        {
            Node *pNode = new_node(key);
            if(pNode != NULL)
            {
                *head = pNode;
                generic_vector_push_back((*head)->words,word);
                return TRUE;
            }
            else return FALSE;
        }
        if (mystring_compare(key, (*head)->data) == -1)
        {
            if(insert(&((*head)->right),key,word) == FALSE) return FALSE;
            else if(insert(&((*head)->left),key,word) == FALSE) return FALSE;
        }
        (*head)->height= max(height((*head)->left), height((*head)->right)) + 1;
        balance = balance_factor(*head);
        if(balance > 1 && mystring_compare(key, (*head)->left->data) < 0)
        {
            *head = right_rotate(*head);
            return TRUE;
        }
        if(balance < -1 && mystring_compare(key, (*head)->right->data) > 0)
        {
            *head = left_rotate(*head);
            return SUCCESS;
        }
        // Left Right Case
        if(balance > 1 && mystring_compare(key, (*head)->left->data) > 0)
        {
            (*head)->left = left_rotate((*head)->left);
            *head = right_rotate(*head);
            return TRUE;
        }
        // Right Left Case
        if(balance < -1 && mystring_compare(key, (*head)->right->data) < 0)
        {
            (*head)->right = right_rotate((*head)->right);
            *head = left_rotate(*head);
            return TRUE;
        }
        return TRUE;
    }
}
void my_set_destroy(MY_SET* p_hMY_SET)
{
    if(p_hMY_SET != NULL)
    {
        My_set* set = (My_set*)*p_hMY_SET;
        if(set != NULL)
        {
            destroy(set->head);
            free(set);
        }
    }
}
void destroy(Node* root)
{
    if(root == NULL) return;
    else
    {
        generic_vector_destroy(&root->words);
        destroy(root->left);
        destroy(root->right);
        mystring_destroy(&root->data);
        free(root);
    }
}
Boolean my_set_traverse(MY_SET hMY_SET, void (*visit)(MYSTRING, void*))
{
    My_set* set = (My_set*) hMY_SET;
    if(set != NULL)
    {
        in_order_traverse(set->head,visit);
        return SUCCESS;
    }
    return FAILURE;
}

MYSTRING get_root_key(MY_SET hMY_SET)
{
    My_set* set = (My_set*) hMY_SET;
    return set->head->data;
}
void in_order_traverse(Node* head, void (*visit)(MYSTRING, void*))
{
    if(head == NULL) return;
    else
    {
        in_order_traverse(head->left,visit);
        visit(head->data,head->words);
        in_order_traverse(head->right, visit);
        return;
    }
}
Boolean my_set_is_empty(MY_SET hMY_SET)
{
    My_set* set = (My_set*) hMY_SET;
    if(set == NULL) exit(1);
    if(set->head == NULL) return TRUE;
    return FALSE;
}
Boolean my_set_remove(MY_SET hMY_SET, MYSTRING item)
{
    My_set* set = (My_set*) hMY_SET;
    if(set == NULL) return FAILURE;
    set->head = set_remove(set->head,item);
    return SUCCESS;
}
Node* set_remove(Node* head, MYSTRING item)
{
    int balfac;
    Node* temp;
    if(head == NULL) return head;
    if(mystring_compare(item, head->data) == -1) head->left = set_remove(head->left,item);
    else if(mystring_compare(item, head->data) == 1) head->right = set_remove(head->right,item);
    else
    {
        if(head->left == NULL || head->right == NULL)
        {
            temp = head->left ? head->left : head->right;
            if(temp == NULL)
            {
                temp = head;
                head = NULL;
            }
            else *head = *temp;
            free(temp);
        }
        else
        {
            temp = findmin(head->right);
            head->data = temp->data;
            head->right = set_remove(head->right, temp->data);
        }
    }
    if(head == NULL) return head;
    head->height = max(height(head->left), height(head->right)) + 1;
    balfac = balance_factor(head);
    if(balfac == 2 && balance_factor(head->left) >= 0) return right_rotate(head);
    if(balfac == 2 && balance_factor(head->left) < 0)
    {
        head->left =  left_rotate(head->left);
        return right_rotate(head);
    }
    if(balfac == -2 && balance_factor(head->right) <= 0) return left_rotate(head);
    if(balfac == -2 && balance_factor(head->right) > 0)
    {
        head->right = right_rotate(head->right);
        return left_rotate(head);
    }
    return head;
}
int my_set_size(MY_SET hMY_SET)
{
    My_set* set = (My_set*) hMY_SET;
    if(set == NULL) return -1;
    return size(set->head);
}
int size(Node* head)
{
    if(head == NULL) return 0;
    return 1 + size(head->left) + size(head->right);
}
int my_set_height(MY_SET hMY_SET)
{
    My_set* set = (My_set*) hMY_SET;
    if(set == NULL) return -1;
    return height(set->head);
}
int height(Node* head)
{
    if(head == NULL) return 0;
    return head->height;
}
int max(int a, int b)
{
    if(a > b) return a;
    return b;
}
int balance_factor(Node *head)
{
    return (height(head->left) - height(head->right));
}
Node* left_rotate(Node *head)
{
    Node *temp = head;
    if(temp == NULL) return NULL;
    else if(temp->right == NULL) return NULL;
    else
    {
        temp = head->right;
        head->right = temp->left;
        temp->left = head;
        head->height = max(height(head->left),height(head->right)) + 1;
        temp->height = max(height(temp->left),height(temp->right)) + 1;
    }
    return temp;
}
Node* right_rotate(Node *head)
{
    Node *temp = head;
    if(temp == NULL) return NULL;
    else if(temp->left == NULL) return NULL;
    else
    {
        temp = head->left;
        head->left = temp->right;
        temp->right = head;
        head->height = max(height(head->left),height(head->right)) + 1;
        temp->height = max(height(temp->left),height(temp->right)) + 1;
    }
    return temp;
}
Node* findmin(Node* head)
{
   Node* temp = head;
   // while(temp->left != NULL)
        temp = temp->left;
    return temp;
}

GENERIC_VECTOR get_specific_list(void* words)
{
    return (GENERIC_VECTOR)words;
}

GENERIC_VECTOR largest_word_list(MY_SET hSet)
{
    My_set* set = (My_set*)hSet;
    Node* temp;
    int size = 0;
    int *max = &size;
    
    if(set->head != NULL)
    {
        traverse_for_largest(set->head, &temp, max);
        return temp->words;
    }
    return NULL;
}

void traverse_for_largest(Node* head, Node **max, int *temp_max)
{
    if(head == NULL)
        return;
    traverse_for_largest(head->left, max, temp_max);
    if(generic_vector_size(head->words) > *temp_max)
    {
        *temp_max = generic_vector_size(head->words);
        *max = head;
    }
    traverse_for_largest(head->right,max,temp_max);
}

GENERIC_VECTOR find_max(GENERIC_VECTOR current_max, GENERIC_VECTOR hVector)
{
    int current_max_size = generic_vector_size(current_max);
    int size = generic_vector_size(hVector);
    
    if(current_max_size >= size) return current_max;
    else return hVector;
    return current_max;
}
