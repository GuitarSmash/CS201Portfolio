#include <ctype.h> //for toupper
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//declarations
////////void delayStart(unsigned int);
void welcomeScreen();
char chooseGameMode();
void makeBoardSize (int*, int*);
void confirmOptions(char, int, int);
//void makeConnectBoard (char *connectboard);

int main(int argc, char *argv[]) { //begin main
    char gameMode;
    int rowSize, columnSize;
    //**board[rowSizecolumnSize];

    welcomeScreen();
    gameMode = chooseGameMode();
    makeBoardSize(&rowSize, &columnSize);
    confirmOptions(gameMode, rowSize, columnSize);
    //buildBoard();//********

    printf("You made it to the end.");
    return 0;
} //end

void welcomeScreen() { //begin welcomeScreen
    printf("==========================================================================\n");
    printf("==========================================================================\n");
    printf("========================== Welcome to Connect 4 ==========================\n");
    printf("==========================================================================\n");
    printf("==============================================================V0.01=======\n\n");
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

void makeBoardSize(int* rowSize, int* columnSize) { //begin makeBoardSize (?) void
    int row = 0;
    int column = 0;

    printf("\n====================================================================\n");
    printf("=========================================================================\n");
    printf ("Note: The standard board size is 7x6 [7 rows, 6 columns].\n");
    printf ("Please enter a board size.\n");

    while (row <= 0) { //while loop for user to enter Connect 4 Row Size
        printf ("\tNumber of rows: ");
        scanf ("%d", &row);

        if (row <= 0) {
            printf ("\n\tEnter a valid number!! [must be greater than 0]\n");
        }
    }

    while (column <= 0) { //while loop for user to enter Connect 4 Column Size
        printf ("\tNumber of columns: ");
        scanf ("%d", &column);

        if (column <= 0) {
            printf ("\n\tEnter a valid number!! [must be greater than 0]\n");
        }
    }
    *rowSize = row;
    *columnSize = column;

    return;
} //end


void confirmOptions(char gameChoice, int row, int column) { //begin confirmOptions
    //char chooseGameMode(char gameChoice);
    //int makeBoardSize(int rowSizer, int columnSizer);
    system("clear");
    printf("=========================================================================\n");
    printf("========================= Connect 4 ================================\n");
    printf("=========================================================================\n\n");

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
return;
}//end

void makeBoard() {

}
