// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// The double quotes will make the compiler first check the local directory
// for a match. If it fails, then it will treat the file as if it were contained
// within the angle bracket <>, which then checks system paths.
// https://stackoverflow.com/questions/3162030/difference-between-angle-bracket-and-double-quotes-while-including-heade
#include "dictionary.h"

// structure for trie
typedef struct node
{
    bool is_word;
    struct node *cell[27];
}
node;

// In order to reference the trie, the first node "root" is set globally.
node *root;


/*------FUNCTION DECLARATION------*/
// The following are used within the the output functions.

// Assign an index value to letters and apostrophe
int indexer(char a);

// Unload dictionary
void unloader(node *trav);

// Counts words in dictionary
unsigned int counter(node *trav);


/*------OUTPUT FUNCTION DEFINITIONS------*/

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary
    FILE *file = fopen(dictionary, "r");

    // declare array to hold a word
    char word[LENGTH + 1];

    // create a starting node
    root = malloc(sizeof(node));

    // configure node
    if (root)
    {
        root->is_word = false;

        for (int i = 0; i < 27; i++)
            root->cell[i] = NULL;
    }
    else
    {
        // When out of memory
        return false;
    }

    // create a traveling node
    node *trav;

    // scan for each individual word in the dictionary
    while (fscanf(file, "%s", word) != EOF)
    {
        // Set traveling node to start at the root node for each word.
        trav = root;

        // check each char until end of word
        for (int i = 0; word[i] != '\0'; i++)
        {
            // declare an index value for current char in word
            int index = indexer(word[i]);

            // Checks if whether the current cell in the array has an address.
            // If not, create new node.
            if (!trav->cell[index])
            {
                node *new_node = malloc(sizeof(node));

                // configure new_node
                if(new_node)
                {
                    new_node->is_word = false;

                    for (int j = 0; j < 27; j++)
                        new_node->cell[j] = NULL;
                }
                else
                {
                    // When out of memory
                    return false;
                }

                // Make the current cell point to new_node
                trav->cell[index] = new_node;
            }

            // Set the traveling node to point to the new_node.
            trav = trav->cell[index];
        }

        // Mark the last node of the word as true
        trav->is_word = true;
    }

    // close dictionary file
    fclose(file);
    return true;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Create traveling node
    node *trav = root;

    // Retrieve each char until end of word
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = indexer(word[i]);

        // If cell is NULL
        if (!trav->cell[index])
            return false;

        // Move to next node
        trav = trav->cell[index];
    }

    // Once at the final node, check if the node denotes the completion of a word
    if (trav != NULL && trav->is_word)
        return true;
    else
        return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Seperate function used to employ recursion
    return counter(root);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Seperate function used to employ recursion
    unloader(root);
    return true;
}


/*------LOCAL FUNCTION DEFINITIONS------*/

// Provides a index value from 1-27 from the alphabetical letter or an apostrophe
int indexer(char a)
{
    if (isupper(a))
    {
        return (a - 65);
    }
    else if(islower(a))
    {
        return (a - 97);
    }
    else
    {
        return 26;
    }
}

// Free allocated memory in the heap.
/*Adding a double ** would change the pointer root which would be an issue.
    In this case it makes a copy of the root pointer called trav.*/
void unloader(node *trav)
{
    // Current node for this function
    node *current = trav;

    // Check every cell in the node
    for (int i = 0; i < 27; i++)
    {
        // if cell points to another node
        if (trav->cell[i])
        {
            // Follow cell into the next node...
            trav = trav->cell[i];

            // and repeat this process
            unloader(trav);

            // Move trav back to current node of concern
            trav = current;
        }
    }

    // Free the current node
    free(trav);
}

// Count loaded in dictionary
unsigned int counter(node *trav)
{
    // Initiate variable to count words
    unsigned int size = 0;

    // Check if current node denotes a word
    if (trav->is_word)
        size++;

    // Current node for this function
    node *current = trav;

    // For each of the cells in this node
    for (int i = 0; i < 27; i++)
    {
        // Check if they point to another node
        if (trav->cell[i])
        {
            // Travel to said node
            trav = trav->cell[i];

            // Then rerun this function and add back in any words found
            size += counter(trav);

            // Move trav back to node of concern
            trav = current;
        }
    }
    return size;
}

/*
NOTE TO SELF:

When executing ./speller texts/lalaland.txt > student.txt
printf in any of these functions will print to the file, not the command line.

Look into Ternary Search Tree option for future use.

Ranked 603 of 636 5/23/2018
Total Time
Mine: 17.036 s
Fastest: 3.652 s
https://cs50.me/checks/7838e5a77e0900e072a3f2d256decd89fcd486d5

*/