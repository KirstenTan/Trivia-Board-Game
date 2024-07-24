#include <stdio.h>
#include <string.h>
#include "functions.h"

/*  adminPhase displays the admin menu with 10 options.

    @return 0 to exit the function
*/
int
adminPhase()
{
	int nElem, option, i;
	entryType entry[150];

	nElem = 0;
	for (i = 0; i < MAXWORDS; i++) // Initialize entry's elements
        resetEntry(&entry[i]);
    
    divider();
    printf("   _      _       _        ___ _\n");
    printf("  /_\\  __| |_ __ (_)_ _   | _ \\ |_  __ _ ___ ___\n");
    printf(" / _ \\/ _` | '  \\| | ' \\  |  _/ ' \\/ _` (_-</ -_)\n");
    printf("/_/ \\_\\__,_|_|_|_|_|_||_| |_| |_||_\\__,_/__/\\___|\n");
		
    // Display menu
	do 
    {
        divider();
        printf("\n /\\  _| _ . _   |\\/| _ _\n");
        printf("/--\\(_|||||| )  |  |(-| )|_|\n\n\n");       

        printf("1 %c%c Add Word\n", 196, 196);
        printf("2 %c%c Add Trivia\n", 196, 196);
        printf("3 %c%c Modify Entry\n", 196, 196);
        printf("4 %c%c Delete Word\n", 196, 196);
        printf("5 %c%c Delete Clue\n", 196, 196);
        printf("6 %c%c View Words\n", 196, 196);
        printf("7 %c%c View Clues\n", 196, 196);
        printf("8 %c%c Export\n", 196, 196);
        printf("9 %c%c Import\n", 196, 196);
        printf("10 %c%c Back to Main Menu\n\n", 196, 196);

        option = rangeQuestion("Enter option", 10);

		switch (option) 
        {
			case 1: addWord(entry, &nElem); break;
			case 2: addTrivia(entry, nElem, 1); break;
			case 3: modifyEntry(entry, nElem); break;
			case 4: deleteWord(entry, "", &nElem); break;
			case 5: deleteClue(entry, &nElem); break;
			case 6: viewEntries(entry, nElem); break;
			case 7: viewClues(entry, nElem); break;
			case 8: exportFile(entry, nElem); break;
			case 9: importFile(entry, &nElem); break;
            case 10: backToMainMenu(entry, &option, nElem); break;
		}
	} while (option != 10);

    return 0;
}

/*  addWord allows the user to add up to 150 entries to the game. It asks the user for the
    word/phrase as well as the accompanying trivia. It also updates the number of elements after a
    word is added. 

    @param entry - entryType array containing all the entries made
    @param *nElem - number of entires made

    @return 0 to exit the function
*/
int
addWord (entryType entry[],
         int *nElem)
{
    char ans;
    string20 newWord;

    divider();
    printf("\n /\\  _| _|  |  | _  _ _|\n");
    printf("/--\\(_|(_|  |/\\|(_)| (_|\n\n\n");

    // Display error message if the max number of entries has been reached
    if (*nElem == 150)
    {
        printf("You have already reached the maximum number of entries. You cannot add any more.\n\n");
        return 0;
    }

    // Get word from user
    do 
    {   
        printf("Enter word/phrase: ");
        getString(newWord, 20);
    } while (!validWord(entry, newWord, *nElem));

    strcpy(entry[*nElem].word, newWord);
    addTrivia(entry, *nElem, 2);
    (*nElem)++;

    // Insert the word into the alphabetically arranged array
    if (*nElem > 1) 
        sortAlphabetical(entry, *nElem);

    printf("\nSuccessfully added a word!\n\n");
    
    // Display warning message if the max number of entries has been reached
    if (*nElem == MAXWORDS)
        printf("WARNING: You have reached the maximum number of entries. You will not be able to add any more.\n\n");

    else
    {
        ans = binaryQuestion("Add another word?");
        if (ans == 'y' || ans== 'Y')
            addWord(entry, nElem);
    }

    return 0;
}

/*  getWord asks the user for a word.

    @param entry - entryType array containing all the entries made
    @param question - question to be asked
    @param nElem - number of entries made
    @param type - indicator of where this function was called from
        if type = 1, this function was called from add trivia
*/
int
getWord(entryType entry[],
        string62 question,
        int nElem,
        int type)
{
    int index, option; 
    string20 word;

    viewWords(entry, nElem);

    do
    {
        option = 0; // reset
        printf("%s: ", question);
        getString(word, 20);

        index = searchWord(entry, word, nElem);

        if (index == -1 || (type == 1 && entry[index].nTrivia == MAXTRIVIA))
        {
            if (index == -1)
                printf("INVALID: The word you're looking for hasn't been added to the game.\n\n");
            else if (type == 1 && entry[index].nTrivia == MAXTRIVIA)
                printf("INVALID: You have already reached the max number of 10 trivia per entry.\n\n");
        
            printf("  1 %c%c Search for another word.\n", 196, 196);
            printf("  2 %c%c Go back to admin menu.\n\n", 196, 196);

            option = rangeQuestion("Enter option", 2);
            if (option == 2)
                return -1;
            printf("\n");
        }
    } while (option == 1);

    return index;
}

/*  viewWords displays the list of words added to the game.

    @param entry - entryType array containing all the entries made
    @param nElem - number of entries made
*/
void
viewWords(entryType entry[], 
          int nElem)
{
    int i;

    for (i = 0; i < nElem; i++)
        printf("Entry %2d: %s\n", i + 1, entry[i].word);
    printf("\n");
}

/*  searchWord performs a linear search to search for a keyword on an entryType array.
    
    @param entry - entryType array containing all the entries made
    @param key - word to be searched for in the array
    @param nElem - number of entries made

    @return i if the key is found
    @return -1 if the key is not found
*/
int
searchWord(entryType entry[], 
           string20 key, 
           int nElem)
{
    int i;

    for (i = 0; i < nElem; i++)
        if (strcmp(key, entry[i].word) == 0)
            return i;

    return -1;
}

/*  validWord checks if a given word passes the two conditions:
        1. It starts with an alphabetical letter
        2. It does not already exist in the entryType array

    @param entry - entryType array containing all the entries made
    @param key - word to be checked
    @param nElem - number of entries made
    
    @return 0 if the keyword is INVALID
    @return 1 if the keyword is VALID
*/
int
validWord(entryType entry[],
          string20 word,
          int nElem)
{
    int valid = 1;

    // Check if the word starts with an non-special character
    if (!(word[0] >= 'A' && word[0] <= 'Z') && !(word[0] >= 'a' && word[0] <= 'z') &&
        !(word[0] >= '0' && word[0] <= '9')) 
    {
        printf("INVALID: The word/phrase must start with an non-special character. Please try again.\n\n");
        valid = 0;
    }
        
    // Check if the word doesn't already exist in the entryType array
    if (searchWord(entry, word, nElem) != -1)
    {
        printf("INVALID: This word/phrase already exists in the game. Please try again.\n\n");
        valid = 0;
    }

    return valid;
}

/*  sortAlphabetical performs a linear search on an entryType array to sort the last element
    alphabetically. It also considers the case (uppercase or lowercase) of the letters in the word.
    
    @param entry - entryType array containing all the entries made
    @param nElem - number of entries made
    Pre-condition: All elements in entry, except for the last, are alphabetically arranged based on
        their words.
*/
void
sortAlphabetical(entryType entry[],
                 int nElem)
{
    int i;
    string20 word1, word2;
    entryType lastEntry;

    i = nElem - 2;
    resetEntry(&lastEntry);
    lastEntry = entry[nElem - 1];

    // Convert words' letters' to lowercase for comparison purposes
    convertLower(entry[i].word, word1);
    convertLower(entry[nElem - 1].word, word2);

    // Compare spelling, move entries until word2 is greather than word1
    while (i >= 0 && strcmp(word2, word1) < 0)
    {   
        entry[i + 1] = entry[i];
        i--;

        if (i >= 0)
            convertLower(entry[i].word, word1);
    }

    // If the spellings are the same, compare cases
    if (strcmp(word2, word1) == 0)
    {   // Move entries until last word is greater than the next word
        while (i >= 0 && strcmp(lastEntry.word, entry[i].word) < 0)
        {
            entry[i + 1] = entry[i];
            i--;
        }
    }

    // Insert last word
    entry[i + 1] = lastEntry;
}

/*  convertLower converts a string into all lowercase letters and stores it in another string.

    @param original - given string it its original capitalization format
    @param converted - given string in lowercase format
*/
void
convertLower(string20 original,
             string20 converted)
{
    int i;

    for (i = 0; i < strlen(original); i++)
    {
        if (original[i] >= 'A' && original[i] <= 'Z')
            converted[i] = original[i] + 32;
        else
            converted[i] = original[i];
    } 

    converted[i] = '\0';
}

/*  addTrivia allows the user to add up to 10 trivia to a word. 

    @param entry - entryType array containing all the entries made
    @param nElem - number of entries made
    @param type - indicator of where this function was called from
        if type = 1, the function was called from main
        if type = 2, the function was called from addWord
    
    @return 0 to exit the function
*/
int
addTrivia(entryType entry[],
          int nElem,
          int type)
{
    int i, index, full;
    int *nTrivia;
    char ans;
    triviaType newTrivia;

    full = 1;
    
    if (type == 1)
    {   
        divider();
        printf("            ___\n");
        printf(" /\\  _| _|   | _.  . _\n");
        printf("/--\\(_|(_|   || |\\/|(_|\n\n\n");

        if (nElem == 0)
        {
            printf("No entries have been made.\n");
            return 0;
        }

        // Display error message if all words have reached the max number of trivia
        for (i = 0; i < nElem; i++)
            if (entry[i].nTrivia < 10)
                full = 0;
        
        if (full == 1)
        {
            printf("You have reached the maximum of 10 trivia for each word. You cannot add any more.\n\n");
            return 0;
        }

        index = getWord(entry, "Which word would you like to add a trivia to?", nElem, 1);
        if (index == -1)
            return 0;
    } 
    
    else // if type == 2
        index = nElem;

    nTrivia = &entry[index].nTrivia;

    // Get trivia
    do 
    {   // Loop to get multiple trivia
        do 
        {   // Loop to check if trivia is valid
            printf("\nTrivia %d\n", *nTrivia + 1);
            printf("  Enter relation: ");
            getString(newTrivia.relation, 30);

            printf("  Enter relation value: ");
            getString(newTrivia.relationVal, 30);
            printf("\n");
        } while (!validTrivia(entry[index], newTrivia, *nTrivia));

        entry[index].trivia[*nTrivia] = newTrivia;
        (*nTrivia)++;

        if (type == 1)
            printf("Successfully added a trivia!\n\n");

        // Ask  user if they want to add more trivia
        if (*nTrivia < MAXTRIVIA) 
            ans = binaryQuestion("Add more trivia?");

    } while ((ans == 'y' || ans == 'Y') && *nTrivia < MAXTRIVIA);

    if (*nTrivia == MAXTRIVIA) 
        printf("WARNING: You have reached the max limit of 10 trivia per entry. You cannot add any more.\n\n");

    // Ask user if they want to add trivia to another word
    if (type == 1 && (ans == 'n' || ans == 'N'))
    {
        ans = binaryQuestion("Add trivia to another word?");
        if (ans == 'y' || ans == 'Y')
            addTrivia(entry, nElem, 1);
    }

    return 0;
}

/*  validTrivia checks if a given trivia passes the two conditions:
        1. The relation and relation value both start with a non-special character.
        2. The trivia is unique.
            - It is not unique when the relation and relation value match those of another trivia
              for the same word
    
    @param entry - current entry where trivia is being added to
    @param key - trivia to be checked
    @param nElem - number of trivias made

    @return 0 if the trivia is INVALID
    @return 1 if the trivia is VALID
*/
int
validTrivia(entryType entry,
            triviaType key,
            int nElem)
{
    int i, valid = 1;

    // Check if 'relation' starts with a non-special character
    if (!(key.relation[0] >= 'A' && key.relation[0] <= 'Z') &&
        !(key.relation[0] >= 'a' && key.relation[0] <= 'z') &&
        !(key.relation[0] >= '0' && key.relation[0] <= '9')) 
    {
        printf("INVALID: The relation must start with an alphabetical character. Please try again.\n");
        valid = 0;
    }
        
    // Check if 'relationVal' starts with an alphabetical character
    if (!(key.relationVal[0] >= 'A' && key.relationVal[0] <= 'Z') && 
        !(key.relationVal[0] >= 'a' && key.relationVal[0] <= 'z') &&
        !(key.relationVal[0] >= '0' && key.relationVal[0] <= '9')) 
    {
        printf("INVALID: The relation value must start with an alphabetical character. Please try again.\n");
        valid = 0;
    }

    // Check if the entered trivia does not already exist.
    for (i = 0; i < entry.nTrivia; i++) 
    {
        if (strcmp(key.relation, entry.trivia[i].relation) == 0 && strcmp(key.relationVal, entry.trivia[i].relationVal) == 0) 
        {
            printf("INVALID: This trivia already exists. Please try again.\n");
            valid = 0;
        }
    }
    
    return valid;
}

/*  modifyEntry allows the user to modify an entry's word or trivia.

    @param entry - entryType array containing all the entries made
    @param nElem - number of entries made

    @return 0 to exit the function
*/
int
modifyEntry(entryType entry[],
            int nElem)
{
    int i, option, index, index2, nTrivia, ans;
    string20 newWord;
    triviaType newTrivia;

    nTrivia = 0;

    divider();
    printf("            _     __\n");
    printf("|\\/| _  _|.(_    |_  _ |_ _\n");
    printf("|  |(_)(_||| \\/  |__| )|_| \\/\n");
    printf("             /             /\n\n");

    if (nElem == 0)
    {
        printf("No entries have been made.\n");
        return 0;
    }

    // Ask user which entry they would like to modify
    index = getWord(entry, "Which word would you like to modify?", nElem, 2);
    if (index == -1)
            return 0;

    printf("\n");

    // Ask user which element of the entry they would like to modify
    do 
    {   
        printf("What would you like to modify in this entry?:\n");
        printf("  1 %c%c Word\n", 196, 196);
        printf("  2 %c%c Trivia\n", 196, 196);
        printf("  3 %c%c End modification\n\n", 196, 196);

        option = rangeQuestion("Enter option", 3);
        printf("\n");

        if (option == 1) // Modify word
        {
            do 
            {   // Loop to make sure the new word is valid
                printf("Current Word: %s\n", entry[index].word);
                printf("New Word: ");
                getString(newWord, 20);
                printf("\n");

            } while (!validWord(entry, newWord, nElem));

            strcpy(entry[index].word, newWord);
            printf("Successfully modified a word!\n\n");
        }
        
        else if (option == 2) // Modify trivia
        {
            // Display list of trivia
            for (i = 0; i < entry[index].nTrivia; i++) 
                printf("Trivia %d: %s: %s\n", i + 1, entry[index].trivia[i].relation, entry[index].trivia[i].relationVal);
            printf("\n");

            index2 = rangeQuestion("Enter trivia number", entry[index].nTrivia);
            index2--; // - 1 to get index
            printf("\n");
            
            do 
            {   // Loop to make sure the new trivia is valid
                printf("Current Relation: %s\n", entry[index].trivia[index2].relation);
                printf("Current Relation value: %s\n\n", entry[index].trivia[index2].relationVal);

                printf("New Relation: ");
                getString(newTrivia.relation, 30);

                printf("New Relation Value: ");
                getString(newTrivia.relationVal, 30);
                printf("\n");
            } while (!validTrivia(entry[index], newTrivia, nTrivia));

            entry[index].trivia[index2] = newTrivia;
            printf("Successfully modified a trivia!\n\n");
        }
    } while (option != 3);

    // Ask user if they want to modify another entry
    ans = binaryQuestion("Modify another entry?");
    if (ans == 'y' || ans == 'Y')
        modifyEntry(entry, nElem);
    
    return 0;
}

/*  deleteWord allows the user to delete a word from the game. Deleting the word will delete
    the entire entry.

    @param entry - entryType array containing all the entries made
    @param word - word to be deleted, unless it is null
    @param nElem - address of the number of entries made

    @return 0 to exit the function
*/
int
deleteWord(entryType entry[],
           string20 word,
           int *nElem)
{
    int i, max, index, type;
    char ans;

    type = 2; // type 2 indicates that this function was called from deleteClue
    
    if (strcmp(word, "") == 0)
    {
        type = 1; // type 1 indidcates that this function was called from adminPhase
        divider();
        printf("\n __\n");
        printf("|  \\ _| _|_ _  |  | _  _ _|\n");
        printf("|__/(-|(-|_(-  |/\\|(_)| (_|\n\n\n");

        if (*nElem == 0)
        {
            printf("No entries have been made.\n");
            return 0;
        }

        index = getWord(entry, "Which word would you like to delete?", *nElem, 2);
        if (index == -1)
            return 0;
    }

    else
        index = searchWord(entry, word, *nElem);
    
    // For entries between 1 to 149, move succeeding entries one index up
    if (*nElem == 150) // Get the max between nElem and 149
        max = 149;
    else
        max = *nElem;
    
    for (i = index; i < max; i++)
        entry[i] = entry[i + 1];

    // For entry 150 (if it exists), individually reset each element
    if (*nElem == 150)
        resetEntry(&entry[149]);

    (*nElem)--;

    printf("Successfully deleted a word!\n\n");

    // Ask user if they want to delete another word
    if (type == 1)
    {
        ans = binaryQuestion("\nDelete another word?");
        if (ans == 'y' || ans == 'Y')
            deleteWord(entry, "", nElem);
    }
    
    return 0;
}

/*  deleteClue allows the user to delete clues from a chosen word. If all clues are deleted, the
    entire word is deleted.

    @param entry - entryType array containing all the entries made
    @param nElem - address of the number of entries made

    @return 0 to exit the function
*/
int
deleteClue(entryType entry[],
           int *nElem)
{
    int i, max, index, index2;
    char ans;

    ans = 'y';

    divider();
    printf(" __             __\n");
    printf("|  \\ _| _|_ _  /  |    _\n");
    printf("|__/(-|(-|_(-  \\__||_|(-\n\n\n");

    if (*nElem == 0)
    {
        printf("No entries have been made.\n");
        return 0;
    }
    
    index = getWord(entry, "Which word's clue/s would you like to delete?", *nElem, 2);
    if (index == -1)
            return 0;

    do
    {
        // Display trivia
        printf("\nObject: %s\n", entry[index].word);
        for (i = 0; i < entry[index].nTrivia; i++)
            printf("Trivia %d %c %s: %s\n", i + 1, 196, entry[index].trivia[i].relation, entry[index].trivia[i].relationVal);
        printf("\n");

        // Get index of trivia
        index2 = rangeQuestion("Enter trivia number", entry[index].nTrivia);
        printf("\n");

        // If there is only one trivia left, ask user if they want to proceed and delete the entire entry
        if (entry[index].nTrivia == 1) {
            printf("WARNING: Each entry must have at least one trivia. Deleting this will delete the entire word.\n");
            ans = binaryQuestion("Do you wish to continue?");
            printf("\n");
        }

        index2--; // subtract 1 to get actual index
        
        // Delete clue
        if (ans == 'y' || ans == 'Y') {
            // For trivia between 1 to 9, move succeeding trivia one index up    
            if (*nElem == 10) // Get max of 9 and *nElem
                max = 9;
            else
                max = entry[index].nTrivia;

            for (i = index2; i < max; i++)
                entry[index].trivia[i] = entry[index].trivia[i + 1];

            // For trivia 10 (if it exists), individually reset each element
            if (entry[index].nTrivia == 10)
            {
                strcpy(entry[index].trivia[9].relation, "");
                strcpy(entry[index].trivia[9].relationVal, "");
            }

            entry[index].nTrivia--;

            // Delete word if there are no more trivia left
            if (entry[index].nTrivia == 0)
                deleteWord(entry, entry[index].word, nElem);

            printf("Successfully deleted a trivia!\n\n");
        }

        ans = binaryQuestion("Delete another clue?");
    } while (ans == 'y' || ans == 'Y');
    
    // Ask user if they want to delete clues from another word
    ans = binaryQuestion("Delete clues from another word?");
    if (ans == 'y' || ans == 'Y')
        deleteClue(entry, nElem);

    return 0;
}

/*  viewEntries displays the current list of words added to the game. 

    @param entry - entryType array containing all the entries made
    @param nElem - number of entries made

    @return 0 to exit the function
*/
int
viewEntries(entryType entry[],
            int nElem)
{
    int i, j, invalid;
    char ch, bin;

    divider();
    printf("\n\\  /. _     |  | _  _ _| _\n");
    printf(" \\/ |(-\\)/  |/\\|(_)| (_|_)\n\n\n");

    if (nElem == 0)
    {
        printf("No entries have been made.\n");
        return 0;
    }
    
    viewWords(entry, nElem);
    i = 0; // reset

    do
    {   
        do
        {
            // Display clues for each word
            printf("\nEntry %d\n", i + 1);
            printf("  Object: %s\n\n", entry[i].word);

            for (j = 0; j < entry[i].nTrivia; j++)
                printf("  %s: %s\n", entry[i].trivia[j].relation, entry[i].trivia[j].relationVal);
            printf("\n");

          // Ask user to press N for Next, P for Previous, or X to exit the view
            invalid = 0; // reset
            printf("Press:\n");
            printf("  N %c%c Next Page\n", 196, 196);
            printf("  P %c%c Previous Page\n", 196, 196);
            printf("  X %c%c Exit View\n\n", 196, 196);

            printf("Enter Option: ");
            scanf("%c%c", &ch, &bin);

            if (ch != 'n' && ch != 'N' && ch != 'p' && ch != 'P' && ch != 'x' && ch != 'X')
            {
                printf("INVALID: Please input a valid character.\n");
                invalid = 1;
            }
            
            else if (i == 0 && (ch == 'p' || ch == 'P'))
            {
                printf("INVALID: This is already the first page.\n");
                invalid = 1;
            }
            
            else if (i == nElem - 1 && (ch == 'n' || ch == 'N'))
            {
                printf("INVALID: This is the last page.\n");
                invalid = 1;
            }
        
        } while (invalid);

        if (ch == 'n' || ch == 'N')
            i++;

        else if (ch == 'p' || ch == 'P')
            i--;
        
    } while ((ch != 'x' || ch != 'x') && i <= nElem);

    return 0;
}

/*  viewClues displays the list of clues under a word chosen by the user.

    @param entry - entryType array containing all the entries made
    @param nElem - number of entries made 

    @return 0 to exit the function
*/
int
viewClues(entryType entry[],
          int nElem)
{
    int i, index;
    char ans;

    divider();
    printf("             __\n");
    printf("\\  /. _     /  |    _ _\n");
    printf(" \\/ |(-\\)/  \\__||_|(-_)\n\n\n");

    if (nElem == 0)
    {
        printf("No entries have been made.\n");
        return 0;
    }

    index = getWord(entry, "Which word's clues would you like to view?", nElem, 2);
    if (index == -1)
            return 0;
    
    // Display trivia
    printf("\nObject: %s\n", entry[index].word);
    for (i = 0; i < entry[index].nTrivia; i++)
        printf("  %s: %s\n", entry[index].trivia[i].relation, entry[index].trivia[i].relationVal);
    printf("\n");
    
    // Ask user if they want to view clues from another word
    ans = binaryQuestion("View clues of another word?");
    if (ans == 'y' || ans == 'Y')
        viewClues(entry, nElem);

    return 0;
}

/*  exportFile allows the user to save the entries into a text file.

    @param entry - entryType array containing all the entries made
    @param nElem - number of entries made

    @return 0 to exit the function
*/
int
exportFile(entryType entry[], int nElem)
{
    int i, j;
    string30 fileName;
    FILE *fp;

    divider();
    printf(" __\n");
    printf("|_    _  _  _|_\n");
    printf("|__)(|_)(_)| |_\n");
    printf("     |\n\n");

    if (nElem == 0)
    {
        printf("No entries have been made.\n");
        return 0;
    }

    // Ask user for file name
    printf("Enter file name (e.g. filename.txt): ");
    getString(fileName, 30);

    fp = fopen(fileName, "w");

    // Transfer data from the array into the file
    for (i = 0; i < nElem; i++)
    {
        fprintf(fp, "Object: %s\n", entry[i].word);
        for (j = 0; j < entry[i].nTrivia; j++)
            fprintf(fp, "%s: %s\n", entry[i].trivia[j].relation, entry[i].trivia[j].relationVal);
        
        if (i < nElem - 1)
            fprintf(fp, "\n");
    }

    printf("Successfully exported a file!\n\n");

    fclose(fp);
    return 0;
}

/*  importFile allows the user to load data from a file into the game. 

    @param entry - entryType array containing all the entries made
    @param nElem - number of entries made
*/
int
importFile(entryType entry[], int *nElem)
{
    int i, j, k, index, option;
    char ch, ans;
    string30 fileName;
    string62 trivia;
    entryType newEntry;
    FILE *fp;

    resetEntry(&newEntry); // initialize newEntry

    divider();
    printf("\n| _  _  _  _|_\n");
    printf("|||||_)(_)| |_\n");
    printf("    |\n");

    if (*nElem == MAXWORDS)
    {
        printf("\nThe maximum of 150 entries has already been reached.\n");
        return 0;
    }

    do
    {
        option = 0;
        // Ask user for file name
        printf("\nEnter file name (e.g. filename.txt): ");
        getString(fileName, 30);
        printf("\n");

        if ((fp = fopen(fileName, "r")) == NULL)
        {
            printf("INVALID: %s does not exist.\n", fileName);
            printf("  1 %c%c Enter another file name\n", 196, 196);
            printf("  2 %c%c Go back to admin menu\n\n", 196, 196);

            option = rangeQuestion("Enter option", 2);
        } 
    } while (option == 1);

    if (option == 2)
        return 0;
    
    // Read file contents
    while (!feof(fp) && *nElem != MAXWORDS)
    {
        i = 0; // Trivia index, reset for each entry
        getStringFromFile(fp, newEntry.word, 8, 20); // read word, exclude the word "Object: "

        do
        {   
            getStringFromFile(fp, trivia, 0, 62); // read trivia pair

            if (strcmp(trivia, "") != 0)
            {
                k = 0; // reset for each trivia
                j = 0; // reset for relation

                do  
                {   // Get relation by splitting the trivia at the ':'
                    ch = trivia[k];
                    if (ch != ':')
                    {
                        newEntry.trivia[i].relation[j] = ch;
                        j++;
                    }
                    newEntry.trivia[i].relation[j] = '\0';
                    k++;
                } while (ch != ':');
                
                k++; // skip space
                j = 0; // reset for relation value

                do  
                {   // Get relation value
                    ch = trivia[k];
                    if (ch != '\0')
                    {
                        newEntry.trivia[i].relationVal[j] = ch;
                        j++;
                    }
                    newEntry.trivia[i].relationVal[j] = '\0';
                    k++;
                } while (ch != '\0');

                i++; // next trivia
            }
            newEntry.nTrivia = i;
        } while (strcmp(trivia, "") != 0);

        index = searchWord(entry, newEntry.word, *nElem);

        // If the new word already exists in the game, ask user which data they would like to keep
        if (index != -1)
        {
            printf("TEST 3\n");
            printf("\nThe word \"%s\" from the file already exists in game. Which data would you like to keep?\n\n", newEntry.word);

            // Display both data
            printf("1 %c%c Existing Data:\n", 196, 196);
            printf("     Object: %s\n", entry[index].word);
            for (i = 0; i < entry[index].nTrivia; i++)
                printf("     %s: %s\n", entry[index].trivia[i].relation, entry[index].trivia[i].relationVal);
            printf("\n");

            printf("2 %c%c New Data:\n", 196, 196);
            printf("     Object: %s\n", newEntry.word);
            for (i = 0; i < newEntry.nTrivia; i++)
                printf("     %s: %s\n", newEntry.trivia[i].relation, newEntry.trivia[i].relationVal);
            printf("\n");

            option = rangeQuestion("Enter number of choice", 2);
            
            // If user keeps the new data, overwrite the previous data
            if (option == 2)
                entry[index] = newEntry;
        } 
        
        // If the new word does not exist yet, add it to the end of the array
        else
        {
            entry[*nElem] = newEntry; 
            (*nElem)++;

            if (*nElem > 1) 
                sortAlphabetical(entry, *nElem);

            // If the max number of words is reached, any unloaded data will no longer be loaded
            if (*nElem == MAXWORDS)
                printf("WARNING: You have reached the max limit of 150 entries. Some entries may not have been added into the current list.\n\n");
        }
    }

    printf("Successfully imported a file!\n\n");

    if (*nElem < MAXWORDS)
    {
        ans = binaryQuestion("Import another file?");
        if (ans == 'y' || ans == 'Y')
            importFile(entry, nElem);
    }

    return 0;
}

/*  getStringFromFile reads a string, character by character, from a text file.  

    @param fp - file pointer to currently opened file
    @param string - where the received string will be stored
    @param start - string index where the function should start reading from
    @param length - maximum length of the type of string
    Pre-condition: Maximum length of any string to be read is 62
*/
void
getStringFromFile(FILE* fp, string62 string, int start, int length)
{
    int i;
    char ch;

    i = start;

    // Skip to starting character
    for (i = 0; i < start; i++)
        fscanf(fp, "%c", &ch);

    i = 0; // reset
    do
    {
        fscanf(fp, "%c", &ch);
        if (i == 0 && ch == '\n')
        {
            strcpy(string, "");
            i = length;
        }
            
        else if (ch != '\n')
        {
            string[i] = ch;
            i++;
            string[i] = '\0';
        }
    } while (i < length && ch != '\n');
}

/*  backToMainMenu allows the user to leave the admin phase and return to the main menu.

    @param entry - entryType array containing all the entries made
*/
void
backToMainMenu(entryType entry[], int *option, int nElem)
{
    int i;
    char ans;

    ans = 'y';

    if (nElem != 0)
    {
        divider();
        printf("\nWARNING: All unexported data will be deleted.\n\n");
        ans = binaryQuestion("Do you wish to proceed?");
    }
    
    if (ans == 'y' || ans == 'Y')
        for (i = 0; i < MAXWORDS; i++)
            resetEntry(&entry[i]);
    
    else
        *option = 0;
}