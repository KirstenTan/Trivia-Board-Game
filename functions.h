#ifndef functions
#define functions

#define MAXWORDS 150
#define MAXTRIVIA 10
#define MAXROWS 15
#define MAXCOLS 15

typedef char string15[16];
typedef char string20[21];
typedef char string30[31];
typedef char string62[63];

struct triviaTag {
	string30 relation,
			 relationVal;
};

typedef struct triviaTag triviaType;

struct entryTag {
	string20 	word;
	triviaType 	trivia[10];
	int			nTrivia;
};

typedef struct entryTag entryType;


// Main.c
void divider();
void getString(string30, int);
int rangeQuestion(string30, int);
int binaryQuestion(string30);
void resetEntry(entryType*);



// Admin.c
int adminPhase();

// General functions
int getWord(entryType[], string62, int, int);
void viewWords(entryType[], int);
int searchWord(entryType[], string20, int);

// Add Word functions
int addWord(entryType[], int *);
int validWord(entryType[], string20, int);
void sortAlphabetical(entryType[], int);
void convertLower(string20, string20);

// Add Trivia functions
int addTrivia(entryType[], int, int);
int validTrivia(entryType, triviaType, int);

// Modify Entry functions
int modifyEntry(entryType[], int);

// Delete functions
int deleteWord(entryType[], string20, int*);
int deleteClue(entryType[], int*);

// View functions
int viewEntries(entryType[], int);
int viewClues(entryType[], int);

// File functions
int exportFile(entryType[], int);
int importFile(entryType[], int*);
void getStringFromFile(FILE*, string62, int, int);

void backToMainMenu(entryType[], int*, int);



// Game.c
int gamePhase();
void displayGameSettings(string20, string20, int*);

// Import file functions
int importFileToGame(entryType[], int*, string15);
void processFile(FILE*, entryType[], int*);
void splitTrivia(string62, entryType[], int*, int *);

// Board set-up functions
void getDimensions(entryType[], entryType[][MAXCOLS], string15, int*, int*, int*);
void getGameList(entryType[], entryType[][MAXCOLS], string15, int*, int*, int*);
void removeWord(entryType[], int*, int);
int isLetterUnique(entryType[], char, int);
int swapLetters(entryType[][MAXCOLS], entryType, int, int, int, int);

// Actual game functions
void displayBoard(entryType[][MAXCOLS], int, int);
int playGame(entryType[][MAXCOLS], int, int, int*);
int searchLetter(entryType[][MAXCOLS], int, int, int, char);  
void checkGuess(entryType[][MAXCOLS], string20, int*, int, int, int);
void gameOver(int);

#endif 