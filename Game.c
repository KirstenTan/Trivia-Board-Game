#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"

/*  gamePhase displays the game menu and game options. It is repeatedly displayed until the
    user exits to the main menu.

    @return 0 to exit the function
*/
int
gamePhase()
{
	// Declare and initialize variables
    int nElem = 0;
    int option, nRows, nCols, win, range, i, j;
    char ans;
    string15 fileStatus, dimensionStatus;
    entryType entry[MAXWORDS];
    entryType gameList[MAXROWS][MAXCOLS];

    divider();
    printf("  ___                  ___ _\n");
    printf(" / __|__ _ _ __  ___  | _ \\ |_  __ _ ___ ___\n");
    printf("| (_ / _` | '  \\/ -_) |  _/ ' \\/ _` (_-</ -_)\n");
    printf(" \\___\\__,_|_|_|_\\___| |_| |_||_\\__,_/__/\\___|\n");

    do
    {
        // Display menu
        divider();
        printf(" __\n");
        printf("/ _  _  _  _  |\\/| _ _\n");
        printf("\\__)(_||||(-  |  |(-| )|_|\n\n\n");

        printf("1 %c%c New Game\n", 196, 196);
        printf("2 %c%c Back to Main Menu\n\n", 196, 196);

        option = rangeQuestion("Enter option", 2);

        if (option == 1)
        {   
            // Reset values before each game
            for (i = 0; i < MAXWORDS; i++) 
                resetEntry(&entry[i]);

            for (i = 0; i < MAXROWS; i++)
                for (j = 0; j < MAXCOLS; j++)
                    resetEntry(&gameList[i][j]);

            nElem = 0;
            nRows = 0;
            nCols = 0;
            range = 1;
            strcpy(fileStatus, "X");

            do
            {   
                divider();
                printf("            __\n");
                printf("|\\ | _     / _  _  _  _\n");
                printf("| \\|(-\\)/  \\__)(_||||(-\n\n");

                strcpy(dimensionStatus, "X");

                do
                {   
                    displayGameSettings(fileStatus, dimensionStatus, &range);
                    ans = rangeQuestion("Enter option", range);

                    if (ans == 1)
                    {
                        if (nElem == MAXWORDS)
                        {
                            printf("\nOops! You have reached the maximum limit of 150 entries. You cannot add any more.\n\n");
                            divider();
                        }
                        else
                            importFileToGame(entry, &nElem, fileStatus);
                    }

                    else if (ans == 2)
                        getDimensions(entry, gameList, dimensionStatus, &nRows, &nCols, &nElem);
                    

                    else if (ans == 3)
                    {
                        playGame(gameList, nRows, nCols, &win);
                        gameOver(win);
                    }

                } while (ans != 3);
                
                // Ask if user wants to play again
                ans = binaryQuestion("Play again?");
            } while (ans == 'y' || ans == 'Y');
        }

    } while (option != 2);

    return 0;
}

/* displayGameSettings displays 3 game settings: Import File, Set Board Dimensions, and Start Game.
   the second and third options are only displayed when the previous option has been selected and
   completed.

   @param fileStatus - string that indicates if a file has been imported
   @param dimensionStatus - string that indicates if board dimensions have been set
   @param range - address that points to an integer that indicates what numbers are valid inputs
*/
void
displayGameSettings(string20 fileStatus, 
                   string20 dimensionStatus,
                   int *range)
{
    // Display prompt
    if (strcmp(fileStatus, "X") == 0 || strcmp(dimensionStatus, "X") == 0)
        printf("\nPlease do the following before starting a game!\n\n");
    else
        printf("Feel free to change any of the game settings before playing!\n\n");

    // Display game options
    printf("1 %c%c Import File\t\t%s\n", 196, 196, fileStatus);

    if (strcmp(fileStatus, "DONE") == 0)
    {
        printf("2 %c%c Set Board Dimensions\t%s\n", 196, 196, dimensionStatus);
        (*range)++;

        if (strcmp(dimensionStatus, "DONE") == 0)
        {
            printf("3 %c%c Start Game!\n", 196, 196);
            (*range)++;
        }
    }

    printf("\n");
}

/*  importFileToGame allows the user to load data from a text file into the game. The data from the
    text file is transfered into 'entry'.

    @param entry - entryType array containing all loaded entries
    @param nElem - pointer to an integer representing the number of loaded entries
    @param fileStatus - string that indicates if a file has been imported 

    @return 0 to exit the function
    Pre-condition: max entries have not yet been reached
*/
int
importFileToGame(entryType entry[],
                 int *nElem,
                 string15 fileStatus)
{   
    int option;
    string30 fileName;
    FILE *fp;

    divider();
    printf("\nIMPORT FILE\n\n");

    do {
        option = 0; // Reset
        printf("Enter file name (e.g. filename.txt): ");
        getString(fileName, 30);

        if ((fp = fopen(fileName, "r")) == NULL) {
            printf("\nINVALID: %s does not exist.\n", fileName);
            printf("  1 %c%c Enter another file name\n", 196, 196);
            printf("  2 %c%c Go back to game menu\n\n", 196, 196);
            option = rangeQuestion("Enter option", 2);
        } 
    } while (option == 1);

    if (option == 2)
        return 0;
    
    processFile(fp, entry, nElem);
    strcpy(fileStatus, "DONE");

    printf("\nSuccessfully imported a file!\n");

    if (*nElem == MAXWORDS)
        printf("WARNING: You have reached the maximum limit of 150 entries. Some entries may not have been added.\n\n");
    
    fclose(fp);
    divider();

    return 0;
}

/*  processFile manages the reading of data from a text file. It ensures that the entries don't
    exceed the max limit and it groups entry content based on line spaces in the text file.

    @param fp - FILE pointer of text file to be read
    @param entry - entryType array containing all loaded entries
    @param nElem - pointer to an integer representing the number of loaded entries

    Pre-condition: text file has been opened and will be closed in another function
*/
void
processFile(FILE *fp, 
            entryType entry[],
            int *nElem) 
{
    string62 trivia;

    while (!feof(fp) && *nElem != MAXWORDS) 
    {
        getStringFromFile(fp, entry[*nElem].word, 8, 20);

        do 
        {
            getStringFromFile(fp, trivia, 0, 62);

            if (strcmp(trivia, "") != 0)
                splitTrivia(trivia, entry, nElem, &entry[*nElem].nTrivia);

        } while (strcmp(trivia, "") != 0);
        (*nElem)++;
    }
}

/*  splitTrivia splits a line of trivia from a text file into its relation and relation value
    and stores it in 'entry'.

    @param trivia - string of trivia to be split
    @param entry - entryType array containing all loaded entries
    @param nElem - address where number of loaded entries are stored
    @param i - address where current trivia index is stored
*/
void
splitTrivia(string62 trivia, 
           entryType entry[],
           int *nElem, 
           int *i)
{
    int k = 0, j = 0;
    char ch;

    // Get relation
    do
    {
        ch = trivia[k];
        if (ch != ':')
        {
            entry[*nElem].trivia[*i].relation[j] = ch;
            j++;
        }
        entry[*nElem].trivia[*i].relation[j] = '\0';
        k++;
    } while (ch != ':');
    
    k++; // Skip space
    j = 0; // Reset for relation value

    // Get relation value
    do
    {   
        ch = trivia[k];
        if (ch != '\0')
        {
            entry[*nElem].trivia[*i].relationVal[j] = ch;
            j++;
        }
        entry[*nElem].trivia[*i].relationVal[j] = '\0';
        k++;
    } while (ch != '\0');

    (*i)++; // Next trivia
}

/*  getDimensions allows the user to set the dimensions of the board. The minimum size is 1 row
    by 1 column and the maximum size is 15 rows by 15 columns.

    @param entry - entryType array containing all loaded entries
    @param fileStatus - string that indicates if a file has been imported
    @param dimensionStatus - string that indicates board dimensions have been set
    @param nRows - address where the row size will be stored
    @param nCols - address where the column size will be stored
    @param nElem - address where the number of loaded entries are stored
*/
void
getDimensions(entryType entry[], 
              entryType gameList[][MAXCOLS],
              string15 dimensionStatus,
              int *nRows,
              int *nCols,
              int *nElem)
{
    divider();
    printf("BOARD DIMENSIONS\n\n");
    
    *nRows = rangeQuestion("Enter number of rows (from 1 to 15)", 15); // Get number of rows
    *nCols = rangeQuestion("Enter number of cols (from 1 to 15)", 15); // Get number of columns

    if (*nRows * *nCols > *nElem) // Error message if number of entries is insufficient
    {
        printf("\nOops! There are not enough entries to generate a %d x %d board.\n", *nRows, *nCols);
        printf("Please import another file or enter smaller dimensions.\n");
    }
    
    else
        getGameList(entry, gameList, dimensionStatus, nElem, nRows, nCols);

    divider();
}

/*  getGameList determines the list of playable entries from the entryType array 'entry' and
    stores it in another entryType array 'gameList'.

    @param entry - entryType array containing all loaded entries
    @param gameList - 2D entryType array contaning playable entries
    @param dimensionStatus - string that indicates if board dimensions have been set
    @param nElem - address where number of loaded entries are stored
    @param nRows - address where row size is stored
    @param nCols - address where column size is stored
    Pre=condition: Assum nElem is greater than or equal to nRows times nCols
*/
void
getGameList(entryType entry[],
            entryType gameList[][MAXCOLS],
            string15 dimensionStatus,
            int *nElem,
            int *nRows,
            int *nCols)
{
    int i, j, k, index, invalid, nElemBoard, nElemRow, unsuccessful;
    int indices[MAXWORDS] = {0};
    time_t t1;
    entryType rowEntries[MAXWORDS];

    nElemBoard = *nElem;
    unsuccessful = 0;

    srand((unsigned) time (&t1));

    for (i = 0; i < *nElem; i++)
        indices[i] = i;

    for (i = 0; i < MAXWORDS; i++)
        rowEntries[i] = entry[i];

    // Get entry for each row
    i = 0;
    while (i < *nRows && !unsuccessful)
    {   
        // For each row, set possible row entries equal to possible board entries
        for (k = 0; k < nElemBoard; k++)
            rowEntries[k] = entry[k];
        nElemRow = nElemBoard;

        // Get entry for each column in current row
        j = 0;
        while (j < *nCols && !unsuccessful)
        {
            do
            {   
                invalid = 0; // reset
                index = indices[rand() % nElemRow]; // Generate random index
                    
                if (isLetterUnique(gameList[i], entry[index].word[0], j) != -1) // If word is unique, but letter is not unique...
                    invalid = swapLetters(gameList, entry[index], i, j, *nRows, *nCols);

                else
                    gameList[i][j] = entry[index];

                // Remove chosen word from the range of possible words for the board
                if (!invalid)
                    removeWord(entry, &nElemBoard, index);

                // Remove all generated words from the range of possible words for the current row
                removeWord(rowEntries, &nElemRow, index);

                // If there are no more possible words that could fill up the board, display error message
                if (invalid && (nElemBoard == 0 || nElemRow == 0))
                {
                    printf("\nOops! There are not enough unique entries to fill the board.\n");
                    printf("Please import another file or enter smaller dimensions.\n");
                    strcpy(dimensionStatus, "X");
                    unsuccessful = 1;
                }
            } while (invalid && (nElemBoard != 0 || nElemRow != 0) && !unsuccessful);
            j++;
        }
        i++;
    }

    if (!invalid)
        strcpy(dimensionStatus, "DONE");
}

/*  removeWord removes a given entry from an entryType array.

    @param list - entryType array that will be modified
    @param nElem - address where number of elements in 'list' are stored
    @param index - index of element to be deleted from 'list'
    Pre-condition: assume valid index
*/
void
removeWord(entryType list[],
           int *nElem,
           int index)
{
    entryType temp;

    temp = list[index];
    list[index] = list[*nElem - 1];
    list[*nElem - 1] = temp;
    (*nElem) --;
}

/*  swapLetters tries to swap an invalid entry from the current row to that of another row, to
    to maximize the entries' cell assignments.

    @param gameList - 2D entryType array containing playable entries
    @param curEntry - current entry that needs to be swapped
    @param curRow - current row number
    @param curCol - current column number
    @param nRows - row size
    @param nCols - column size

    @return 0 if swapped properly
    @return 1 if unable to swap
*/
int
swapLetters(entryType gameList[][MAXCOLS], 
            entryType curEntry,
            int curRow,
            int curCol,
            int nRows,
            int nCols)
{
    int i, j, swapped;

    i = 0;
    j = 0;
    swapped = 0;

    // Find which row the current letter can be placed in
    while (!swapped && i < curRow)
    {
        if (isLetterUnique(gameList[i], curEntry.word[0], nCols) == -1)
        {   
            // Find which letter from that row can be placed in the current row
            while (!swapped && j < nCols)
            {
                if (isLetterUnique(gameList[curRow], gameList[i][j].word[0], nCols) == -1)
                {
                    gameList[curRow][curCol] = gameList[i][j];
                    gameList[i][j] = curEntry;
                    swapped = 1;
                }
                j++;
            }
        }
        i++;
    }

    return !swapped;
}

/*  isLetterUnique determines if the first letter of a given word is equal to the first letter of
    any word in an entryType array.

    @param gamelist - entryType array containing the words a single row of the board
    @param letter - first letter of the word to be compared
    @param nElem - number of elements in the current row

    @return i if the first letter is NOT unique
    @return -1 if the first letter is unique
*/
int
isLetterUnique(entryType gameList[],
               char letter,
               int nElem)
{
    int i;

    for (i = 0; i < nElem; i++)
        if (gameList[i].word[0] == letter)
            return i;

    return -1;
}

/*  displayBoard displays the game board with the dimensions nRows x nCols and cell values
    dictated by the first letter of each word in gameList.

    @param gameList - 2D entryType array containing all the playable entries
    @param nRows - row size
    @param nCols - column size
*/
void
displayBoard(entryType gameList[][MAXCOLS],
             int nRows,
             int nCols)
{
    int i, r, c;

    // Top line
    printf("\n%c", 218);
    for (i = 0; i < nCols - 1; i++)
        printf("%c%c%c%c", 196, 196, 196, 194);
    printf("%c%c%c%c\n", 196, 196, 196, 191);

    for (r = 0; r < nRows; r++)
    {
        // Second line + first letter
        for (c = 0; c < nCols; c++)
            printf("%c %c ", 179, gameList[r][c].word[0]);
        printf("%c\n", 179);

        if (r < nRows - 1)
        {
            // Connecting line
            printf("%c", 195);
            for (i = 0; i < nCols - 1; i++)
                printf("%c%c%c%c", 196, 196, 196, 197);
            printf("%c%c%c%c\n", 196, 196, 196, 180);
        }
    }

    // Bottom line
    printf("%c", 192);
    for (i = 0; i < nCols - 1; i++)
        printf("%c%c%c%c", 196, 196, 196, 193);
    printf("%c%c%c%c\n\n", 196, 196, 196, 217);
}

/*  playGame starts the actual game. It repeatedly asks the user to guess a word given a random trivia
    until the user has correctly guessed one word from each row on the board.

    @param gameList - 2D entryType array containing all the playable entries
    @param nRows - row size
    @param nCols - column size
    @param win - address where the integer representation of the result of the game is stored

    @return 0 to exit the function
*/
int
playGame(entryType gameList[][MAXCOLS],
         int nRows,
         int nCols,
         int *win)
{
    int i = 0;
    int j, index, incorrect, remainingLetters;
    char letter, bin;
    time_t t1;
    string20 guess;
    
    srand((unsigned) time (&t1));

    do
    {
        remainingLetters = nCols;
        do
        {   
            incorrect = 0; // reset
            do
            {   // Loop to get valid letter
                divider();
                printf(" __        \n");
                printf("|__)| _   |\n");
                printf("|   |(_|\\/.\n");
                printf("        /\n");

                displayBoard(gameList, nRows, nCols);

                // Ask user for letter
                printf("*Enter 0 to leave the game.\n\n");
                printf("Choose a letter from row %d: ", i + 1);
                scanf("%c%c", &letter, &bin);

                if (letter == '0')
                {
                    *win = 0;
                    return 0;
                }
                    
                // Search if letter exists in the current row
                j = isLetterUnique(gameList[i], letter, nCols);   
                if (j == -1)
                    printf("INVALID: Please enter a letter from row %d.\n\n", i + 1);

            } while (j == -1);

            // Display random trivia
            srand((unsigned) time (&t1));
            index = rand() % gameList[i][j].nTrivia;

            printf("\nFIRST LETTER: %c\n", gameList[i][j].word[0]);
            printf("CLUE: %s: %s\n\n", gameList[i][j].trivia[index].relation, gameList[i][j].trivia[index].relationVal);
            printf("YOUR GUESS: ");
            getString(guess, 20);

            checkGuess(gameList, guess, &incorrect, nCols, i, j);
            remainingLetters--;

        } while (incorrect && remainingLetters > 0);

        i++;
    } while ((remainingLetters > 0 || (!incorrect && remainingLetters == 0)) && i < nRows);

    if (incorrect && remainingLetters == 0)
        *win = 0;
    else
        *win = 1;

    return 0;
}

/*  checkGuess checks if the user's guess matches the actual word and updates the board display
    accordingly. Incorrect guesses are marked with *, while correct guesses remain as is and
    mark unattempted guesses with -.

    @param gameList - 2D entryType array containing playable entries
    @param guess - string containing user's guess
    @param incorrect - address of integer representing the correctness of a user's guess
    @param nCols - column size
    @param i - row index of guessed entry
    @param j - column index of guessed entry
*/
void
checkGuess(entryType gameList[][MAXCOLS],
           string20 guess,
           int *incorrect,
           int nCols,
           int i,
           int j)
{
    if (strcmp(gameList[i][j].word, guess) == 0)
    {
        printf("\nYou got it right!\n");
        strcpy(gameList[i][j].word, "*"); // Mark cell with *
    }
        
    else
    {
        printf("\nSorry, that's incorrect! The right answer was %s. Try another question!\n", gameList[i][j].word);
        strcpy(gameList[i][j].word, "-"); // Mark cell with -
        *incorrect = 1;
    }
}

/*  gameOver displays a win/loss message based on the result of the game.

    @param win - integer representing the result of the game
*/
void
gameOver(int win)
{
    divider();
    if (win == 1)
    {
        printf("\n\\_/_          _  _ |\n");
        printf(" |(_)|_|  \\)/(_)| ).\n\n");
    }

    else
    {
        printf("\n\\_/_      | _  _|_|\n");
        printf(" |(_)|_|  |(_)_)|_.\n\n");
    }
}
