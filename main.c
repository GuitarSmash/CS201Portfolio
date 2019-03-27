#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "wincheck.c"
//Breadth First Search for AI
//free memory through nested for loop
//negamax, minimax functions for AI


typedef struct pot { //structure of each hole in the board
    int x; //horizontal location of the pot
    int y; //vertical location of the pot
    int filled; //0 means not filled, 1 means filled by p1, 2 means filled by p2
    //int newlyPlace; //
} pot;

pot* newPot(){ //creation/attributes of each hole in the board
    pot* newpot = malloc(sizeof(pot)); //allocate memory for new hole
    newpot->filled = 0; //0 indicates blank space
    newpot->x = 0; //no x
    newpot->y = 0;//no y
    return newpot;
}

typedef struct board { //structure of the board dimensions
    int cols;
    int rows;
    pot*** map; //outline of the board
} board;

board* newBoard(int row, int column) { //function to allocate memory for a custom board size
    board *newboard = malloc(sizeof(board)); //allocate memory
    pot*** newmap = (pot***)malloc(row * sizeof(pot**));
    int rowCount, columnCount = 0;
    for (rowCount = 0; rowCount < row; rowCount++) {
        //board[rowCount] = (char*)malloc(column * sizeof(char));
        newmap[rowCount] = (pot**)malloc(column * sizeof(pot*));
    }
    for (rowCount = 0 ; rowCount < row ; rowCount++) {
        for (columnCount = 0 ; columnCount < column; columnCount++) {
            newmap[rowCount][columnCount] = newPot();
            newmap[rowCount][columnCount]->filled = 0;
            newmap[rowCount][columnCount]->x = rowCount;
            newmap[rowCount][columnCount]->y = columnCount;
        }
    }
    newboard->cols = column;
    newboard->rows = row;
    newboard->map = newmap;
    return newboard;
}

void freePot(pot *pot){ //free memory of board part
    free(pot);
}

void freeBoard(board* board){ //free memory of board and board size
    int i, j = 0;
    for(i = 0; i< board->rows; i++){
        for(j = 0; j< board->cols; j++){
            freePot(board->map[i][j]); //free every row and every column combination of board parts
        }
    }
    free(board->map); //free board dimensions
    free(board); //free board
}

void welcomeScreen();
char chooseGameMode();
void getCount(int*, int*);
void printBoard(board*, char, int, int, int, int);
char playerTurn(board*, int);//*******

int checkForWinner(board*, int, int);
void declareWinner(int, board*, int, int); //tally scores
int horizontal(board*, int);//
int vertical(board*, int);//
int diagonal(board*, int, int);//
int tieGame(board*);
int scoring(board*, int, int);
char rematchGame(int, int);

int main (int argc, char *argv[]) { //begin main************************************************************
    int row, column = 0; //count for 2D array
    int turnChange = 0; //player turn
    char gameMode; //single or multiplayer decision
    char choiceConfirm; //confirm game setup decision
    int playerOneScore = 0; //tally player 1 score
    int playerTwoScore = 0; //tally player 2 score
    int returning = 1;
    int mainMenu = 1;

    while (mainMenu != 0) {
        welcomeScreen(); //Welcome to Connect Four
        gameMode = chooseGameMode();//Choose Single or Multiplayer function. Character is assigned to gameMode
        while (returning != 0) {
            getCount(&row, &column); //****************rematch
            board *board = newBoard(row, column); //create board
            int pt = 0;

            if (gameMode == 'A') { //singleplayer
                //for (turnChange = 0; turnChange <= row * column && !completeGame; turnChange++) {
                printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore);
                while (pt == 0) { //******* % 2   X and
                    pt = playerTurn(board, (turnChange % 1) + 1);
                    printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore); //print initial board
                }
                if (pt == 1) {
                    playerOneScore++;
                }
                else if (pt == 2) {
                    playerTwoScore++;
                }
                declareWinner(pt, board, playerOneScore, playerTwoScore);
            }

            else if (gameMode == 'B') { //multiplayer (human)
                //for (turnChange = 0; turnChange <= row * column && !completeGame; turnChange++) {
                printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore);
                while (pt == 0) {
                    pt = playerTurn(board, (turnChange % 2) + 1); //******* % 2   X and O
                    turnChange++;
                    printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore); //print initial board
                    //puts("Cannot place piece. The column is full!\n"); //if the board reaches the top
                }
                if (pt == 1) {
                    playerOneScore++;
                }
                else if (pt == 2) {
                    playerTwoScore++;
                }
                declareWinner(pt, board, playerOneScore, playerTwoScore);
            }

            else if (gameMode == 'C') { //multiplayer (computer)
                //for (turnChange = 0; turnChange <= row * column && !completeGame; turnChange++) {
                printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore);
                while (pt == 0) { //******* % 2   X and O
                    pt = playerTurn(board, (turnChange % 2) + 1);
                    turnChange++;
                    printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore); //print initial board
                    //puts("Cannot place piece. The column is full!\n"); //if the board reaches the top
                }
                if (pt == 1) {
                    playerOneScore++;
                }
                else if (pt == 2) {
                    playerTwoScore++;
                }
                declareWinner(pt, board, playerOneScore, playerTwoScore);
            }
            rematchGame(returning, mainMenu);
        }
    }
    return 0;
}
//end main***********************************************************************************************


void welcomeScreen() {
    system("clear");
    printf("==============================================================================\n");
    printf("==============================================================================\n");
    printf("========================= Welcome to Connect 4 ===============================\n");
    printf("=================================================== By Jake Williams =========\n");
    printf("================================================================= V1.00 ======\n\n");
    return;
} //end void

char chooseGameMode() {
    char gameChoice = ' ';
    printf("Please choose a following game mode:\n"); //prints game mode options
    printf("\tSingleplayer: A \n");
    printf("\tMultiplayer (Human): B \n");
    printf("\tMultiplayer (Computer): C \n\n");
    printf("\tExit Game: Q\n\n");

    while (gameChoice != 'A' && gameChoice != 'B' && gameChoice != 'C') { //while loop to choose game mode
        printf("Game Mode: ");
        scanf(" %c", &gameChoice);
        gameChoice = toupper(gameChoice); //convert to uppercase
        while(getchar() != '\n'); //prevent infinite loop if string

        if (gameChoice == 'Q') {
            printf("Thank you for playing!\n");
            exit(0);
        }

        else if (gameChoice != 'A' && gameChoice != 'B' && gameChoice != 'C' && gameChoice != 'Q') { //if invalid character is chosen
            printf("Please choose a valid game mode!\n");
        }
    }
    printf("\n");
    return gameChoice;
} //end char

void getCount(int* row, int* column) {
    int rowInput, columnInput = 0;
    system("clear");
    printf("==============================================================================\n");
    printf("========================= Choose Your Board Size ========================");
    printf("\n==============================================================================\n");
    printf ("WARNING: Any board size smaller 4x4 will result in an impossible to win match!");
    printf("\n\t - Also, any arbitrarily large board will wrap around the \n\t   terminal and will cause formatting issues!\n");
    printf("\t - Stay below 44 rows, 44 columns to avoid formatting issues!\n\n\t");
    printf ("Note: The standard board size is 6x7 [6 rows, 7 columns].\n");

    while (rowInput <= 0) { //while loop for user to enter Connect 4 Row Size
        printf ("\tEnter number of rows: ");
        scanf ("%d", &rowInput);
        while(getchar() != '\n'); //prevent infinite loop if string

        if (rowInput <= 0) {
            printf ("\n\tEnter a valid number!! [must be greater than 0]\n");
        }
    }

    while (columnInput <= 0) { //while loop for user to enter Connect 4 Column Size
        printf ("\tEnter number of columns: ");
        scanf ("%d", &columnInput);
        while(getchar() != '\n'); //prevent infinite loop if string

        if (columnInput <= 0) {
            printf ("\n\tEnter a valid number!! [must be greater than 0]\n");
        }
    }
    printf("\n");
    *row = rowInput;
    *column = columnInput;
    return;
} //end void

char getFilled(board *board, int x, int y){
    if(board->map[x][y]->filled == 1) return 'X';
    if(board->map[x][y]->filled == 2) return 'O';
    if(board->map[x][y]->filled == 0) return ' ';
}
board* setFilled(board *board, int x, int y, int fill){
    board->map[x][y]->filled = fill;
    //printf("FILLED, Filled %d, %d with %d. Proof: %d\n", x, y, fill, board->map[x][y]->filled);
    return board;
}

void printBoard(board *board, char gameMode, int row, int column, int playerOne, int playerTwo) {
    int rowCount, columnCount, line, label = 0;
    system("clear");//************************************
    printf("==============================================================================\n");
    if (gameMode == 'A') {
        printf("===== Game Mode: Singleplayer =============================================\n");
    }
    else if (gameMode == 'B') {
        printf("===== Game Mode: Multiplayer (Human) ======================================\n");
    }
    else if (gameMode == 'C') {
        printf("===== Game Mode: Multiplayer (Computer) ===================================\n");
    }
    printf("===== Board size: %d rows, %d columns =======================================\n", row, column);
    printf("===== Scoreboard (Player 1: [%d], Player 2: [%d]) ===========================\n", playerOne, playerTwo);
    printf("==============================================================================\n\n");

    for (rowCount = 0 ; rowCount < board->rows; rowCount++) {
        for (columnCount = 0 ; columnCount < board->cols; columnCount++) {
            printf("|");
            printf(" %c ", getFilled(board, rowCount, columnCount));
        }
        puts("|");
        for (line = 0; line < board->cols; line++) {
            if (line == 1) {
                printf("-----");
            }
            else {
                printf("----");
            }
        }
        printf("\n");
    }

    for (label = 1; label <= board->cols; label++) { //for loop to format numbers below columns correctly
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
    } //end for
    printf("\n"); //**board;
} //end void

char playerTurn(board* board, int turnChange) {
    int ro, co = 0;
    printf("Choose a column (from 1 to %d)", board->cols);
    printf(", Player %d: ", turnChange); // + 1

    while(1) {
        if (1 != scanf("%d", &co) || co < 1 || co > board->cols) {
            while(getchar() != '\n');
            puts ("\nCannot place piece. Out of bounds!");
            printf("Choose a column (from 1 to %d)", board->cols);//******
            printf(", Player %d: ", turnChange); // + 1
        }
        else {
            break;
        }
    }
    co--; //decrement because the count technically starts from 0, not 1

    for (ro = board->rows - 1; ro >= 0; ro--) { //*****
        //if(board->map[ro][co]->filled == 0 ) { //***
        if(getFilled(board, ro, co) == ' ') {
            board = setFilled(board, ro, co, turnChange);
            //board->full++;
            return checkForWinner(board, ro, co);
        }
    }
    return 0;
}

int checkForWinner(board* board, int row, int column) {
    if(horizontal(board, row) != 0) return horizontal(board, row);
    if(vertical(board, column) != 0) return vertical(board, column);
    if(diagonal(board, row, column) != 0) return diagonal(board, row, column);
    if(tieGame(board) != 0) return tieGame(board);
    return 0;
}

void declareWinner(int which, board *board, int playerOneScore, int playerTwoScore){
    if (which == 1 || which == 2) {
    printf("==============================================================================\n");
    printf("===== Congrats! Player %d has won! ========================================\n", which);
    printf("==============================================================================\n");
    printf("Current scores:\n\tPlayer One: %d\n\tPlayer Two: %d\n\n", playerOneScore, playerTwoScore);
}
else if (which == 3) {
    printf("The game is a tie! No winner!\n");
}
    freeBoard(board); //free memory used by the board when the game ends
}

int horizontal(board* board, int row) {
    int win = 0;
    int i = 0;

    for (i = 0; i < board->cols; i++) {
        if (board->map[row][i]->filled == 1) { //check player one
            win++;
            if (win >= 4) {
                return 1;
                break;
            }
        }
        else {
            win = 0;
        }
    }

    win = 0, i = 0;

    for (i = 0; i < board->cols; i++) {
        if (board->map[row][i]->filled == 2) { //check player two
            win++;
            if (win >= 4) {
                return 2;
                break;
            }
        }
        else {
            win = 0;
        }
    }
    return 0;
}

int vertical(board* board, int column) {
    int win = 0;
    int i = 0;

    for (i = 0; i < board->rows; i++) { //->rows
        if (board->map[i][column]->filled == 1) { //check player one
            win++;
            if (win >= 4) {
                return 1;
                break;
            }
        }
        else {
            win = 0;
        }
    }
    win = 0, i = 0; //reinitialize

    for (i = 0; i < board->rows; i++) { //->rows
        if (board->map[i][column]->filled == 2) { //check player two
            win++;
            if (win >= 4) {
                return 2;
                break;
            }
        }
        else {
            win = 0;
        }
    }
    return 0;
}

int diagonal(board* board, int row, int column) {
    int win = 0;
    int i = 0;
    int j = 0;

    //for (i = 0; ((i + 1) < board->cols) && ((board->rows-i) > 0); i++) {
    //fix check: it is starting at the middle rather than the end

    for (j = 0; j < (board->cols - 3); j++) {
        for (i = 0; ((board->rows-i-1) >= 0) && ((i+j) < board->cols); i++) { //check front player 1
            //printf("Checking j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-1, i+j);
            if (board->map[board->rows-i-1][i+j]->filled == 1) {
                win++;
                //printf("Win\n");
                if (win >= 4) {
                    //printf("Winner\n");
                    return 1;
                    break;
                }

            }
            else win = 0;
        }
    }

    win = 0, i = 0, j = 0; //reinitialize
    //printf("Begin back scan\n");
    //start the scan from the end!!!!!!!!!
    for (j = 0; j < (board->cols - 3); j++) {
        for (i = 0; ((board->rows-i-1) >= 0) && ((i+j) < board->cols); i++) { //check back player 1
            //printf("Checking j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-1, board->cols-i-j-1);
            if (board->map[board->rows-i-1][board->cols-i-j-1]->filled == 1) {
                win++;
                //printf("Win\n");
                if (win >= 4) {
                    //printf("Winner\n");
                    return 1;
                    break;
                }

            }
            else win = 0;
        }
    }

    win = 0, i = 0, j = 0; //reinitialize


    for (j = 0; j < (board->cols - 3); j++) {
        for (i = 0; ((board->rows-i-1) >= 0) && ((i+j) < board->cols); i++) { //check front player 2
            //printf("Checking j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-1, i+j);
            if (board->map[board->rows-i-1][i+j]->filled == 2) {
                win++;
                //printf("Win\n");
                if (win >= 4) {
                    //printf("Winner\n");
                    return 2;
                    break;
                }

            }
            else win = 0;
        }
    }
    win = 0, i = 0, j = 0; //reinitialize

    for (j = 0; j < (board->cols - 3); j++) {
        for (i = 0; ((board->rows-i-1) >= 0) && ((i+j) < board->cols); i++) { //check back player 2
            //printf("Checking j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-1, board->cols-i-j-1);
            if (board->map[board->rows-i-1][board->cols-i-j-1]->filled == 2) {
                win++;
                //printf("Win\n");
                if (win >= 4) {
                    //printf("Winner\n");
                    return 2;
                    break;
                }

            }
            else win = 0;
        }
    }
    return 0;
}

int tieGame(board* board) {
    int tie = 0;
    int i = 0;

    for (i = 0; i < board->cols; i++) {
        if (board->map[0][i]->filled != 0) {
            tie++;
            if (tie == board->cols) {
                return 3; //tie game
            }
        }
        else tie = 0;
    }
return 0;
}

char rematchGame(int returning, int mainMenu) {
    char returner;
    char mainMenuReturner;
    while (returner != 'Y' && returner != 'N') {
        printf("Would you like to play another round (rematch)?\n\tYes [Y], No [N]: ");
        scanf(" %c", &returner);
        returner = toupper(returner);
        if (returner == 'Y') {
            returning = 1;
            return returning;
            break;
        }
        else if (returner == 'N') {
            printf("\tThank you for playing!\n");
            exit(0);
        }
        else if (returner == 'M') {
            mainMenu = 1;
            return mainMenu;
            break;
        }
        if (returner != 'Y' && returner != 'N') {
            printf("Please enter a valid character!\n");
        }
    }
    printf("\n");
}
