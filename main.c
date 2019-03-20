#include <ctype.h> //for toupper
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//declarations
void welcomeScreen();
char chooseGameMode();
void chooseBoardSize (int*, int*);
char confirmOptions(char, int, int); //void
void buildBoard(char**, int, int);
int playerTurn(char**, int, int, int, const char*); //fix function
int singlePlayer();
int multiPlayer();
int multiPlayerComputer();

//---------------------------------MAIN-----------BEGIN-------------------------------------------------------------

int main(int argc, char *argv[]) { //begin main
    //PART 1: Choose Game Options___________________________________________________________________________________
    char gameMode; //char to choose single or multiplayer
    char choiceConfirm; //char to confirm board and mode settings
    const char* SHAPES = "XO"; //borrowed
    int rowSize, columnSize; //changed dynamically in chooseBoardSize

    while (choiceConfirm != 'Y') { //while loop to confirm game mode options before start
        welcomeScreen(); //welcome user
        gameMode = chooseGameMode(); //choose game mode a, b, or c
        chooseBoardSize(&rowSize, &columnSize); //choose board size
        confirmOptions(gameMode, rowSize, columnSize); //confirm game mode and board size
        choiceConfirm = confirmOptions(gameMode, rowSize, columnSize); //assign char to escape loop
    }

    //PART 2: Build The Board_______________________________________________________________________________________
    system("clear");
    printf("\n");
    char** connectFourBoard;
    buildBoard(connectFourBoard, rowSize, columnSize);
    printf("\n");

    //PART 3: Play the Game___________________________________________________________________________________

    int playTurn = 0;
    int finishGame = 0;

    for (playTurn = 0; playTurn < rowSize * columnSize && !finishGame; playTurn++) {
        //buildBoard(connectFourBoard, rowSize, columnSize); //print board
        while (!playerTurn(connectFourBoard, playTurn % 2, rowSize, columnSize, SHAPES)) { //*******
            buildBoard(connectFourBoard, rowSize, columnSize); //print initial board
            puts("Cannot place piece. The column is full!\n");
        }
    }
    buildBoard(connectFourBoard, rowSize, columnSize);

    printf("You made it to the end.");
    return 0;
} //end

//---------------------------------MAIN---END------------------------------------------------------------------------

void welcomeScreen() { //begin welcomeScreen
    printf("==============================================================================\n");
    printf("==============================================================================\n");
    printf("============================ Welcome to Connect 4 ============================\n");
    printf("==============================================================================\n");
    printf("==================================================================V0.01=======\n\n");
    return;
}//end

char chooseGameMode() { //*******begin chooseGameMode (?) void
    char gameChoice = ' ';
    printf("Please choose a following game mode:\n"); //prints game mode options
    printf("\tSingle-player: A \n");
    printf("\tMultiplayer (Human): B \n");
    printf("\tMultiplayer (Computer): C \n\n");

    while (gameChoice != 'A' && gameChoice != 'B' && gameChoice != 'C') { //while loop to choose game mode
        printf("Game Mode: ");
        scanf(" %c", &gameChoice);
        gameChoice = toupper(gameChoice); //convert to uppercase

        if (gameChoice != 'A' && gameChoice != 'B' && gameChoice != 'C') { //if invalid character is chosen
            printf("Please choose a valid game mode!\n");
        }
    }
    return gameChoice;
} //end

void chooseBoardSize(int* rowSize, int* columnSize) { //begin makeBoardSize (?) void
    int row = 0;
    int column = 0;

    printf("\n==============================================================================\n");
    printf("========================= Choose Your Board Size ========================");
    printf("\n==============================================================================\n");
    printf ("WARNING: Any board size smaller 4x4 will result in an impossible to win match!\n\t Also, any board size greater than 101x101 will cause a wrap \n\t around the terminal!\n\n");
    printf ("Note: The standard board size is 7x6 [7 rows, 6 columns].\n");
    printf ("Please enter a board size.\n");

    while (row <= 0) { //while loop for user to enter Connect 4 Row Size
        printf ("\nNumber of rows: ");
        scanf ("%d", &row);

        if (row <= 0) {
            printf ("\nEnter a valid number!! [must be greater than 0]\n");
        }
    }

    while (column <= 0) { //while loop for user to enter Connect 4 Column Size
        printf ("Number of columns: ");
        scanf ("%d", &column);

        if (column <= 0) {
            printf ("\n\tEnter a valid number!! [must be greater than 0]\n");
        }
    }
    *rowSize = row;
    *columnSize = column;

    return;
} //end


char confirmOptions(char gameChoice, int row, int column) { //begin confirmOptions  VOID
    //system("clear");
    printf("\n==============================================================================\n");
    printf("======================= Confirm Your Settings ==========================\n");
    printf("==============================================================================\n\n");

    if (gameChoice == 'A') {
        printf("Game Mode: Singleplayer\n");
    }
    else if (gameChoice == 'B') {
        printf("Game Mode: Multiplayer (Human)\n");
    }
    else if (gameChoice == 'C') {
        printf("Game Mode: Multiplayer (Computer)\n");
    }

    printf("Board Size: ");
    printf("%d", row);
    printf("x");
    printf("%d\n", column);
    char confirm;
    while (confirm != 'Y' && confirm != 'N') {
        printf("Confirm options? [Y/N]: ");
        scanf(" %c", &confirm);
        confirm = toupper(confirm); //convert to uppercase

        if (confirm != 'Y' && confirm != 'N') {
            printf("Please enter a valid character!\n");
        }
    }
    printf("\n");
    return confirm;
}//end

void buildBoard(char** connectFourBoard, int rowSize, int columnSize) {
    int build;
    connectFourBoard = (char **)malloc(rowSize * sizeof(int *));
    for (build = 0; build < rowSize; build++) {
        connectFourBoard[build] = (char *)malloc(columnSize * sizeof(int));
    }

    int xLength; //used to calculate and build rows
    int yLength; //used to calculate and build columns
    int count; //used to count each column
    int label; //used to label each column

    for (xLength = 0; xLength < rowSize; xLength++) {
        for (yLength = 0; yLength < columnSize; yLength++) {
            printf("|  %c", connectFourBoard[xLength][yLength]);

        }
        puts("|");
        //puts("-----------------------------");
        int line;
        for (line = 0; line < columnSize; line++) {
            if (line == 1) {
                printf("-----");
            }
            else {
                printf("----");
            }
        }
        printf("\n");
    }

    for (label = 1; label <= columnSize; label++) { //format numbers below columns correctly
        if (label == 1) { //if 1
            printf("  %d", label);
        }
        else if (label < 10) { //if 2 to 9
            printf("   %d", label);
        }
        else if (label >= 10 && label < 100) { //if 10 to 99
            printf("  %d", label);
        }
        else if (label >= 100) {//if 100 onwards (numbers will become too big for the boxes once it reaches 1000...)
            printf(" %d", label);
        }
    }
    printf("\n");

}

int playerTurn(char** connectFourBoard, int playerTurn, int rowSize, int columnSize, const char* SHAPES) { //borrowed
    int row = 0;
    int column = 0;

        printf("Choose a column (from 1 to %d)", columnSize);
        printf(", Player %d [Shape: %c]: ", playerTurn + 1, SHAPES[playerTurn]);

    while(1) {
        if (1 != scanf("%d", &column) || column < 1 || column > columnSize) {
            while(getchar() != '\n');
            puts ("Cannot place piece. Out of bounds!");
        }
        else {
            break;
        }
    }
    column--;

    for (row = rowSize - 1; row >= 0; row--) {
        if(connectFourBoard[row][column] == ' ') { //*******
            connectFourBoard[row][column] == SHAPES[playerTurn]; //******* place piece
            return 1;
        }
    }
return 0;
}
