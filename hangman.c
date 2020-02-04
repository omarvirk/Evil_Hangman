#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "mystring.h"
#include "generic_vector.h"
#include "assocarray.h"
#include "dictionary.h"

/*************************FUNCTION DECLARATIONS**********************************/

/**********************************************************************
 * Runs one instance of the game Evil Hangman
 *
 ***********************************************************************/
char evil_hangman(GENERIC_VECTOR dictionary_object, int word_length, int number_of_guesses, char running_tally);

/****************************************************************************
 * Checks if a character is new line or end of file (EOF)
 *
 * @param ch
 *    Character to be checked
 *
 *@param discard
 *  Pointer to an integer that tells calling function whether a character should be discarded or not. 1
 *  1 for discard. 0 for keep
 *
 *@return
 *  Returns 1 if character is EOF, newline character, or a lexicographic space.
 *  0 otherwise
 *
 ***************************************************************************/
int terminate(char ch, int* discard);

/****************************************************************************
 *Takes a mystring and determines if the character exists in the mystring or not
 *
 * @param current_guesses
 *  Mystring to determine if a character exists in the mystring object or not
 *
 * @param potential_guess
 *    Character to be determined if it exists or not
 *
 *
 * @return
 *  Returns 1 if character exists in MYSTRING. Returns 0 if does not
 *
 ***************************************************************************/
int guessed_already_or_not(MYSTRING current_guesses, char potential_guess);

/*********************************************************************************************
 *Takes no argument. Asks user for desired word length for hangman game. Reasks user until acceptable
 *word length is received.
 *
 ***********************************************************************************************/
int get_word_length(void);

/**********************************************************************************************
 * Gets the number of guesses the user would like to have while playing hangman. Returns the
 * number of guesses.
 ***********************************************************************************************/
int get_number_of_guesses(void);

/**********************************************************************************************
 * Determines if the user wants a printable tally of the total number of words left in there bank if
 * they so decide too. Always Returns Lower Case.
 ***********************************************************************************************/
char get_tally_or_not(void);

/************************************************************************************
 * Takes a generic vector. Makes a new key from the word.
 *
 ************************************************************************************/
MYSTRING new_key(GENERIC_VECTOR current_word_list, MYSTRING choices_so_far, int word_length);

/**************************************************************************************
 * Win checker. Returns an integer value
 *
 *
 **************************************************************************************/
int if_wins(MYSTRING key, int word_size);

/***********************************************************************************************
 * Prints a key to insert by taking a word, a character, and a word length. For every letter in
 * in the word that matches the lower_case_guess it inserts the letter at the key index.
 * At the end it will return the new word if every inserted instance of the guess within it.
 ****************************************************************************************************/
MYSTRING key_getter(MYSTRING current_word, MYSTRING current_key ,char lower_case_guess, int word_length);


/**************************************************************************************************
 * Checks if any characters in a string are non-alphabetical characters. If at least one character
 * is not alphabetical it returns 1. Otherwise returns 0.
 ************************************************************************************************/
int alphabetical_checker(MYSTRING hString);

char choose_to_play(void);
/**************************************
 *Helper function for travers
 * Helps print a word list attached to a nodexd
 *************************************/
void visit(MYSTRING key, void *words);

/****************************MAIN FUNCTION***************************************/
int main(int argc, const char * argv[])
{
    int word_length;
    int number_of_guesses;
    char tally_or_not;
    GENERIC_VECTOR dictionary_object = dictionary();
    char play_again_or_not;
    
    
    /*Plays Evil hangman as long as user does not choose word length of 0*/
    do
    {
        srand(time(0));
        fprintf(stdout, "************PLAY EVILLLLL HANGMAN**********\n");
        fprintf(stdout, "What word length would you like to play hangman with?\n");
        
        word_length = get_word_length();
        number_of_guesses = get_number_of_guesses();
        tally_or_not = get_tally_or_not();
        
        play_again_or_not = evil_hangman(dictionary_object,word_length, number_of_guesses,tally_or_not);
    }
    while(play_again_or_not == 'y');
    generic_vector_destroy(&dictionary_object);
    return 0;
}


/*************************FUNCTION DEFINITIONS***********************************/
char evil_hangman(GENERIC_VECTOR dictionary_object, int word_length, int number_of_guesses, char running_tally)
{
    GENERIC_VECTOR *pWord_list;
    GENERIC_VECTOR current_word_list;
    MYSTRING choices_so_far = mystring_init_default();
    MYSTRING single_turn_guess = mystring_init_default();
    MYSTRING initial_key = mystring_init_default();
    MYSTRING update_key;
    MYSTRING *p_to_chosen_word;
    MYSTRING *p_current_word;
    MYSTRING current_key;
    MY_SET word_groups;
    int i;
    int white_space;
    int guesses_remaining = number_of_guesses;
    char lower_case_guess;
    int current_list_size;
    int random_number;
    int guessed_letter_correctly;
    
    /*Get Initial word list of word_size-1 from the dictionary file*/
    pWord_list = (GENERIC_VECTOR*)generic_vector_at(dictionary_object, word_length-1);
    current_word_list = vector_init_vector(*pWord_list);
    
    /*Creates an intro key for map and reads words from initial word pool into the rooot node of the map*/
    for(i=0; i<word_length; i++)
        mystring_push(initial_key, '-');
    
    while(guesses_remaining > 0)
    {
        /*Gets the current word list size*/
        current_list_size = generic_vector_size(current_word_list);
        
        /*Prints current word pool size to screen if selected to do so*/
        if(running_tally == 'y') fprintf(stdout, "Total Number of Words Left in Pool :%d\n", current_list_size);
        
        /*States How many guesses you have remaining, and the letters you have guessed so far*/
        fprintf(stdout, "Guesses Remaining: %d\n", guesses_remaining);
        fprintf(stdout, "Letters Chosen So Far: ");
        mystring_output(choices_so_far, stdout);
        fprintf(stdout, "\n");
        
        fprintf(stdout, "Your current word key is:\t");
        mystring_output(initial_key, stdout);
        fprintf(stdout, "\n");
        
        /*takes guesses until player enters a single character guess*/
        fprintf(stdout, "please make a single letter guess: ");
        white_space = 1;
        mystring_input(single_turn_guess, stdin, white_space, terminate);
        while(mystring_size(single_turn_guess) != 1 || alphabetical_checker(single_turn_guess))
        {
            fprintf(stdout, "that was not a single character guess!\n please guess a single character you have not used yet!");
            mystring_input(single_turn_guess, stdin, white_space, terminate);
        }
        
        /*makes guess lower case*/
        lower_case_guess= tolower((mystring_get(single_turn_guess,0)));
        
        /*if it has not been guessed*/
        if(!(guessed_already_or_not(choices_so_far,lower_case_guess)))
        {
            mystring_push(choices_so_far, lower_case_guess);
            guesses_remaining--;
        }
        
        /**************************************************************************************************************
         *Creates a new tree. Gets a new key based off the guesses accumulated so far.
         * Uses the current word list size to get all of the words and inserts the word by key.
         * If a new key is to be created, it will be and so inserted into the tree appropriately
         ***************************************************************************************************************/
        word_groups = my_set_init_default();
        
        for(i=0; i<current_list_size; i++)
        {
            p_current_word = (MYSTRING*)generic_vector_at(current_word_list,i);
            update_key = key_getter(*p_current_word,initial_key, lower_case_guess, word_length);
            my_set_add(word_groups, update_key, *p_current_word);
            mystring_destroy(&update_key);
        }
        
        generic_vector_destroy(&current_word_list);
        current_word_list = vector_init_vector(largest_word_list(word_groups));
        p_current_word = (MYSTRING*)generic_vector_at(current_word_list,0);
        current_key = key_getter(*p_current_word, initial_key, lower_case_guess, word_length);
        mystring_destroy(&initial_key);
        initial_key = mystring_init_substring(current_key, 0, word_length);
        mystring_destroy(&current_key);
        
        my_set_destroy(&word_groups);
        
        guessed_letter_correctly= 0;
        for(i=0; i<word_length; i++)
            if(mystring_get(initial_key,i)==lower_case_guess) guessed_letter_correctly++;
        
        if(guessed_letter_correctly >0)  fprintf(stdout, "You guessed Correctly! You're letter %c is in the word!\n", lower_case_guess);
        else fprintf(stdout, "You guessed incorrectly MUAHAHAHAH! Your letter %c is not in the selected word!\n", lower_case_guess);
        
        
        /*Get a new key from the neweset vectorof mystrings.*/
        /*If player has lost let them know what the word is by chosing a random word out what is remaining!*/
        if(if_wins(initial_key, word_length) == 0)
        {
            fprintf(stdout, "CONGRATULATIONS!! YOU WIN!!!\nThe word was:\t");
            mystring_output(initial_key,stdout);
            fprintf(stdout, "\n");
            guesses_remaining = 0;
        }
        else if(guesses_remaining <= 0)
        {
            fprintf(stdout, "Sorry! You Have Lost :(. Try again next time!\n");
            fprintf(stdout, "As it turns out the chosen word was: ");
            current_list_size = generic_vector_size(current_word_list);
            random_number = rand() % current_list_size;
            p_to_chosen_word = (MYSTRING*)generic_vector_at(current_word_list, random_number);
            mystring_output(*p_to_chosen_word, stdout);
            fprintf(stdout, "\n");
            guesses_remaining = 0;
        }
    }
    mystring_destroy(&initial_key);
    mystring_destroy(&single_turn_guess);
    mystring_destroy(&choices_so_far);
    generic_vector_destroy(&current_word_list);
    return choose_to_play();
}

int terminate(char ch, int* discard)
{
    if(ch == '\n' || ch== EOF || isspace(ch))
    {
        *discard = 1;
        return 1;
    }
    *discard = 0;
    return 0;
}

int guessed_already_or_not(MYSTRING current_guesses, char potential_guess)
{
    int i;
    for(i=0; i<mystring_size(current_guesses); i++)
        if(mystring_get(current_guesses,i) == potential_guess) return 1;
    return 0;
}

int get_word_length(void)
{
    int word_length;
    fflush(stdin);
    if (fscanf(stdin, "%d", &word_length)  == 1) {
        while(word_length < 2 || word_length >29 || word_length == 27 || word_length ==26 || word_length ==23 || word_length ==25\
              || (word_length >= 'a' && word_length <= 'z') || (word_length >= 'A' && word_length <= 'Z'))
        {
            fprintf(stdout, "You're word length is invalid!\nYou must choose a word length greater than 2,\
                    less than 29, and exluding 23,25,26 and 27\nPlease choose again:");
            getchar();
            if(fscanf(stdin, "%d", &word_length) == 1);
        }
    }
    fflush(stdin);
    return word_length;
}

int get_number_of_guesses(void)
{
    int number_of_guesses;
    fprintf(stdout, "How many guesses would you like to have to win?\n");
    while( fscanf(stdin, "%d", &number_of_guesses) != 1) {
        getchar();
        fprintf(stdout, "You have not entered a valid number of guesses. Please Try again :");
        if(number_of_guesses < 0) {
            fprintf(stdout, "%d is too low, and is an invalid number of guesses.\
                    Please type another value:\n",number_of_guesses);
        } else if ( (number_of_guesses >= 'a' && number_of_guesses <= 'z') || (number_of_guesses >= 'A' && \
                                                                               number_of_guesses <= 'Z') || (number_of_guesses > 26) ) {
            fprintf(stdout, "%d is too high, or could be a non-integer input, and is an invalid number of guesses.\
                    Please type another value bettwen 1 and 26:\n", number_of_guesses);
        }
    }
    return number_of_guesses;
}

char get_tally_or_not(void)
{
    int selection_check;
    char tally_or_not;
    //    while(getchar() != '\n');
    fprintf(stdout,"Would you like to have a running total of the number of words that would still fit your current selection(s) per turn?\nType y/n:\n");
    selection_check = fscanf(stdin, " %c", &tally_or_not);
    while(tally_or_not != 'y' && tally_or_not != 'n' && tally_or_not != 'Y' && tally_or_not != 'N')
    {
        fprintf(stdout, "Please type y for yes, or n for no only:\n");
        selection_check = fscanf(stdin, " %c", &tally_or_not);
        while(selection_check != 1)
        {
            fprintf(stdout, "You have not seleceted a valid input for the answer. Please try again: ");
            selection_check = fscanf(stdin, " %c", &tally_or_not);
        }
    }
    return tolower(tally_or_not);
}

MYSTRING new_key(GENERIC_VECTOR current_word_list, MYSTRING choices_so_far, int word_length)
{
    MYSTRING temp_key;
    MYSTRING *p_temp_key;
    MYSTRING current_key = mystring_init_default();
    int i;
    int j;
    p_temp_key = (MYSTRING*)generic_vector_at(current_word_list,0);
    temp_key = mystring_init_substring(*p_temp_key, 0, word_length);
    mystring_output(temp_key, stdout);
    fprintf(stdout, "Got the temp key about from init!\n");
    
    for(i=0; i<word_length; i++) mystring_push(current_key,'-');
    for(i=0; i<word_length; i++)
    {
        for(j=0; j<mystring_size(choices_so_far); j++)
            if(mystring_get(temp_key,i) == mystring_get(choices_so_far,j)) mystring_put(current_key,i,mystring_get(choices_so_far,j));
    }
    mystring_output(current_key, stdout);
    fprintf(stdout, "got the current key :(\n");
    mystring_destroy(&temp_key);
    return current_key;
}

int if_wins(MYSTRING key, int word_size)
{
    int win_checker;
    win_checker = 0;
    int i;
    for(i=0;i<word_size;i++)
        if(mystring_get(key,i) == '-') win_checker++;
    return win_checker;
}

MYSTRING key_getter(MYSTRING current_word,MYSTRING current_key, char lower_case_guess, int word_length)
{
    MYSTRING key_to_insert;
    int i;
    
    key_to_insert =  mystring_init_substring(current_key, 0, word_length);
    for(i=0; i<word_length; i++)
        if(mystring_get(current_word,i) == lower_case_guess) mystring_put(key_to_insert,i,lower_case_guess);
    return key_to_insert;
}

void visit(MYSTRING hString, void* node)
{
    int i;
    // mystring_output(hString, stdout);
    fprintf(stdout, "\n");
    GENERIC_VECTOR pVector = node;
    MYSTRING *pString;
    
    for(i=0; i<generic_vector_size(pVector); i++)
    {
        pString = (MYSTRING*)generic_vector_at(pVector,i);
        mystring_output(*pString, stdout);
        fprintf(stdout, "\n");
    }
}

int alphabetical_checker(MYSTRING hString)
{
    int i;
    for(i=0; i<mystring_size(hString); i++)
        if(!(isalpha(mystring_get(hString,i)))) return 1;
    return 0;
}

char choose_to_play(void)
{
    char choice;
    fprintf(stdout, "Would you like to play EVIL HANGMANNNN again type y/n?\n");
    if(fscanf(stdin, " %c", &choice) == 1) {
        while(choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
            fprintf(stdout, "Please choose y for yes and n for no! :\n");
            if (fscanf(stdin, " %c", &choice) == 1 ) continue;
        }
    }
    return tolower(choice);
}
