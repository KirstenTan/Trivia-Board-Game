/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts
learned. I have constructed the functions and their respective algorithms and corresponding code by myself. The
program was run, tested, and debugged by my own efforts. I further certify that I have not copied in part or whole or
otherwise plagiarized the work of other students and/or persons.

Kirsten Robyn Tan, DLSU ID# 12345563
*********************************************************************************************************/

#include "Game.c"
#include "Admin.c"
#include "functions.h"

/*  divider displays a line of characters to divided each page.
*/
void
divider()
{
    int i;
    printf("\n\n");
    for (i = 0; i < 130; i++)
        printf("%c", 236);

    printf("\n\n");
}

/*  getString gets a string input from the user.

    @param string - string where input will be stored
    @param length - max length of the type of string needed
*/
void
getString(string30 string,
          int length)
{
    int i = 0;
    char ch;

    do
    {
        scanf("%c", &ch);
        if (ch != '\n') 
        {
            string[i] = ch;
            i++;
        }
        string[i] = '\0';
    } while (i < length && ch != '\n');
}

/*  rangeQuestion asks the user a question and requires an integer answer within a given range.

    @param question - question to be asked
    @param max - max limit of the range
*/
int
rangeQuestion(string30 question,
              int max)
{
    int ans;
    char bin;

    do
    {
        printf("%s: ", question);
        scanf("%d%c", &ans, &bin);

        if (ans < 1 || ans > max)
            printf("INVALID: Please enter a number between 1 and %d.\n\n", max);
    } while (ans < 1 || ans > max);
    
    return ans;
}

/*  binaryQuestion asks the user a question and requires a yes or no answer.

    @param question - question to be asked
*/
int
binaryQuestion(string30 question)
{
    char ans, bin;

    do
    {
        printf("%s (Y/N): ", question);
        scanf("%c%c", &ans, &bin);

        if (ans != 'y' && ans!= 'Y' && ans != 'n' && ans != 'N') 
                printf("INVALID: Please try again.\n\n");
    } while (ans != 'y' && ans!= 'Y' && ans != 'n' && ans != 'N');

    return ans;
}

/*  resetEntry sets all the values of the given entryType to zero or null.

    @param entry - entryType to be reset
*/
void
resetEntry (entryType *entry)
{
    int i;

    strcpy(entry->word, "");
    for (i = 0; i < MAXTRIVIA; i++)
    {
        strcpy(entry->trivia[i].relation, "");
        strcpy(entry->trivia[i].relationVal, "");
    }
    entry->nTrivia = 0;
}

int
main()
{   
    int option;

    // Display welcome message
    divider();
    printf("\n");
    printf(" __   __  ___     ______     _______   ________       ___________  _______    __  ___      ___  __          __          ___\n");
    printf("|\"  |/  \\|  \"|   /    \" \\   /\"      \\ |\"      \"\\     (\"     _   \")/\"      \\  |\" \\|\"  \\    /\"  ||\" \\        /\"\"\\        |\"  |\n");
    printf("|'  /    \\:  |  // ____  \\ |:        |(.  ___  :)     )__/  \\\\__/|:        | ||  |\\   \\  //  / ||  |      /    \\       ||  |\n");
    printf("|: /'        | /  /    ) :)|_____/   )|: \\   ) ||        \\\\_ /   |_____/   ) |:  | \\\\  \\/. ./  |:  |     /' /\\  \\      |:  |\n");
    printf(" \\//  /\'     |(: (____/ //  //      / (| (___\\ ||        |.  |    //      /  |.  |  \\.    //   |.  |    //  __'  \\    _|  /\n");
    printf(" /   /  \\\\   | \\        /  |:  __   \\ |:       :)        \\:  |   |:  __   \\  /\\  |\\\\  \\   /    /\\  |\\  /   /  \\\\  \\  / |_/ )\n");
    printf("|___/    \\___|  \" _____/   |__|  \\___)(________/          \\__|   |__|  \\___)(__\\_|_)  \\__/    (__\\_|_)(___/    \\___)(_____/\n\n");
    
    // Display menu
    do
    {
        divider();
        printf("\n");
        printf(" __  __       _         __  __\n");
        printf("|  \\/  | __ _(_)_ __   |  \\/  | ___ _ __  _   _\n");
        printf("| |\\/| |/ _` | | '_ \\  | |\\/| |/ _ \\ '_ \\| | | |\n");
        printf("| |  | | (_| | | | | | | |  | |  __/ | | | |_| |\n");
        printf("|_|  |_|\\__,_|_|_| |_| |_|  |_|\\___|_| |_|\\__,_|\n");
        divider();

        printf("1 %c%c Game\n", 196, 196);
        printf("2 %c%c Admin\n", 196, 196);
        printf("3 %c%c Exit\n\n", 196, 196);

        option = rangeQuestion("Enter option", 3);

        switch (option)
        {
            case 1: gamePhase(); break;
            case 2: adminPhase(); break;
        }
    } while (option != 3);
    
    return 0;
}
