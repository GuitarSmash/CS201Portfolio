#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//GOALS: FIX DIAGONAL CHECKING AND BUFF AI
//The diagonal check might not be check at the top of the column (fixed)
//FIX AI SEG FAULT******************************* (fixed????)
//FIX AI REMOVING PIECE AT THE TOP ROW (might be fixed now)
//GIVE ERROR MESSAGE IF COLUMN IS FULL!!!!!!!!! (might be fixed now)
//FIX AI horizontal 4 in a row scan

//THE DIAGONAL IS WRAPPING!!!!!!! (Fixed)

//structures
typedef struct pot { //structure of each hole in the board
    int x; //horizontal location of the pot
    int y; //vertical location of the pot
    int filled; //0 means not filled, 1 means filled by p1, 2 means filled by p2
    struct pot* neighbor[5]; //array of pointers to the placed piece's children
} pot;

pot* newPot(){ //creation/attributes of each hole in the board
    pot* newpot = malloc(sizeof(pot)); //allocate memory for new hole
    newpot->filled = 0; //0 indicates blank space
    newpot->x = 0; //each x is assigned to 0 or ' ', by default
    newpot->y = 0;//each y is assigned to 0 or ' ', by default
    for (int i = 0; i < 5; i++) newpot->neighbor[i] = NULL; //assign neighbor pointers for AI
    return newpot;
}

//structure of board will contain columns, rows, and an outline of the map based around the row and column sizes
typedef struct board { //structure of the board dimensions(mapping of the board, as well as its row and columns)
    int cols;
    int rows;
    pot*** map;
} board;

//prototypes
board* newBoard(int, int);
void freePot(pot*);
void freeBoard(board*);
void welcomeScreen();
char chooseGameMode();
void getCount(int*, int*);
char getFilled(board*, int, int);
board* setFilled(board*, int, int, int);
void printBoard(board*, char, int, int, int, int);
void freeBoard(board*);
void welcomeScreen();
char chooseGameMode();
void getCount(int*, int*);
char playerTurn(board*, int);
char aiTurn(board*, int);
int checkForWinner(board*, int, int);
void declareWinner(int, board*, int, int);
int horizontal(board*, int);
int vertical(board*, int);
int diagonal(board*);
int tieGame(board*);
int scoring(board*, int, int);
int aiBehavior(board*);
char rematchGame(int);
int c4DFS(int);
void c4IterativeDFS(pot*, int, int*);


board* newBoard(int row, int column) { //function to allocate memory for a custom board size
    board *newboard = malloc(sizeof(board)); //allocate memory for board
    pot*** newmap = (pot***)malloc(row * sizeof(pot**)); //allocate memory for pot and its attributes

    int rowCount, columnCount = 0; //declarations to allocate memory for 2d array
    for (rowCount = 0; rowCount < row; rowCount++) {
        //board[rowCount] = (char*)malloc(column * sizeof(char));
        newmap[rowCount] = (pot**)malloc(column * sizeof(pot*));
    }

    //for every 2d array index, give it newPot attributes (filled condition, x, y, and neighbor pointers for AI functionality)
    //There are 5 neighbor pointers (left, bottom left, bottom, bottom right, and right pointers to help determine next best move)
    for (rowCount = 0 ; rowCount < row ; rowCount++) {
        for (columnCount = 0 ; columnCount < column; columnCount++) {
            newmap[rowCount][columnCount] = newPot(); //for each [row][column], it will contain the attributes of pot
            newmap[rowCount][columnCount]->filled = 0; //the [row][column] will initally be filled to 0, or in other words, blank
            newmap[rowCount][columnCount]->x = rowCount; //row indices are counted
            newmap[rowCount][columnCount]->y = columnCount; //column indices are counted

            //NEIGHBOR 1: LEFT
            newmap[rowCount][columnCount]->neighbor[0] = (columnCount == 0) ? NULL : newmap[rowCount][columnCount - 1];

            //NEIGHBOR 2: BOTTOM LEFT
            newmap[rowCount][columnCount]->neighbor[1] = ((rowCount == row - 1) || (columnCount == 0)) ? NULL : newmap[rowCount + 1][columnCount - 1];

            //NEIGHBOR 3: BOTTOM
            newmap[rowCount][columnCount]->neighbor[2] = (rowCount == row - 1) ? NULL : newmap[rowCount + 1][columnCount];

            //neighbor 4 [bottom right]
            newmap[rowCount][columnCount]->neighbor[3] = ((rowCount == row - 1) || (columnCount == column - 1)) ? NULL : newmap[rowCount + 1][columnCount + 1];

            //neighbor 5 [right]
            newmap[rowCount][columnCount]->neighbor[4] = (columnCount == column - 1) ? NULL : newmap[rowCount][columnCount + 1];
        }
    }
    newboard->cols = column; //total column count of the board is assigned to newboard
    newboard->rows = row; //total row count of the board is assigned to newboard
    newboard->map = newmap; //structure of newmap is assigned to map
    return newboard; //return newboard attributes
}

void freePot(pot *pot){ //free memory of board slot
    free(pot);
}

void freeBoard(board* board){ //free memory of board and board size. Prevent memory leaks
    int i, j = 0;
    for(i = 0; i< board->rows; i++){
        for(j = 0; j< board->cols; j++){
            freePot(board->map[i][j]); //free every row and every column combination of board parts
        }
        free(board->map[i]);
    }
    free(board->map); //free board dimensions
    free(board); //free board
}

int main (int argc, char *argv[]) { //begin main************************************************************
    int row = 0; //used to help get user input row count
    int column = 0; //used to help get user input column count
    int turnChange = 0; //player turn
    char gameMode; //single or multiplayer decision
    int playerOneScore = 0; //tally player 1 score
    int playerTwoScore = 0; //tally player 2 score
    int returning = 1; //returning is used to loop any rematches
    time_t t; //time_t is used to randomize AI column selection if a column is full

    srand((unsigned)time(&t)); //srand function used in DFS for helping AI choose next best move

    welcomeScreen(); //Print welcome screen
    gameMode = chooseGameMode();//Choose Single or Multiplayer function. Character is assigned to gameMode

    while (returning != 0) { //while loop executes if rematch game is confirmed
        row = 0;
        column = 0; //count for user-input 2D array size
        getCount(&row, &column); //function to get dynamic count of board row and column
        board *board = newBoard(row, column); //create board

        int pt = 0; //pt is used to indicate which player wins the game

        if (gameMode == 'A') { //singleplayer
            printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore);//print initial board
            while (pt == 0) { //while no winner is found, execute player turn
                pt = playerTurn(board, (turnChange % 1) + 1); //modulus operator is used to help switch between 1 and 2
                printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore);
            }
            if (pt == 1) { //if player 1 wins, increment score
                playerOneScore++;
            }
            else if (pt == 2) { //if player 2 wins, increment score
                playerTwoScore++;
            }
            declareWinner(pt, board, playerOneScore, playerTwoScore); //call a function to print game results
        }

        else if (gameMode == 'B') { //multiplayer (human), SEE SINGLE PLAYER (ABOVE) FOR DOCUMENTATION
            printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore);//print initial board
            while (pt == 0) {
                pt = playerTurn(board, (turnChange % 2) + 1);
                turnChange++;
                printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore);
            }
            if (pt == 1) {
                playerOneScore++;
            }
            else if (pt == 2) {
                playerTwoScore++;
            }

            //turnChange = 0; //reset player turn back to player 1 for the next round

            declareWinner(pt, board, playerOneScore, playerTwoScore);
        }

        else if (gameMode == 'C') { //multiplayer (computer), SEE SINGLE PLAYER (ABOVE) FOR DOCUMENTATION
            printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore); //print initial board
            while (pt == 0) {
                if ((turnChange % 2) + 1 == 1) { //player turn
                    pt = playerTurn(board, (turnChange % 2) + 1);
                    turnChange++;
                }
                else if ((turnChange % 2) + 1 == 2) { //ai turn *
                    pt = aiTurn(board, (turnChange % 2) + 1);
                    turnChange++;
                }
                printBoard(board, gameMode, row, column, playerOneScore, playerTwoScore);
            } //end while
            if (pt == 1) { //if player one wins
                playerOneScore++;
            }
            else if (pt == 2) { //if AI wins
                playerTwoScore++;
            }
            declareWinner(pt, board, playerOneScore, playerTwoScore); //state the results
        }
        //turnChange = 0; //reset player turn back to player 1 for the next round
        rematchGame(returning); //offer rematch
    }
    return 0;
}
//end main***********************************************************************************************

//program greets user with welcome screen function
void welcomeScreen() {
    system("clear");
    printf("==============================================================================\n");
    printf("==============================================================================\n");
    printf("========================= Welcome to Connect 4 ===============================\n");
    printf("=================================================== By Jake Williams =========\n");
    printf("================================================================= V1.00 ======\n\n");
    return;
} //end void

//The user then uses a char Gamechoice (A, B, or C) to choose game mode. The selection is then returned to main.
char chooseGameMode() {
    char gameChoice = ' ';
    printf("Please choose a following game mode:\n"); //prints game mode options
    printf("\tSingleplayer: A \n");
    printf("\tMultiplayer (Human): B \n");
    printf("\tMultiplayer (Computer): C \n\n");
    printf("\tExit Game: Q\n\n");

    while (gameChoice != 'A' && gameChoice != 'B' && gameChoice != 'C') { //while loop to choose game mode
        printf("Game Mode: ");
        scanf(" %c", &gameChoice); //scan user input for character
        gameChoice = toupper(gameChoice); //convert to uppercase
        while(getchar() != '\n'); //prevent infinite loop if string

        if (gameChoice == 'Q') { //if user decides to not play the game
            printf("Thank you for playing!\n");
            exit(0);
        }

        else if (gameChoice != 'A' && gameChoice != 'B' && gameChoice != 'C' && gameChoice != 'Q') { //if invalid character is chosen
            printf("Please choose a valid game mode!\n");
        }
    }
    printf("\n");
    return gameChoice; //return character choice
} //end char

//void function to determine the dynamix board size input by the user. The row and columns are saved through pointers
void getCount(int* row, int* column) { //void function to get user input of dynamic board size
    int rowInput = 0;
    int columnInput = 0; //ints to store custom row and custom columns sizes
    system("clear");
    printf("==============================================================================\n");
    printf("========================= Choose Your Board Size ========================");
    printf("\n==============================================================================\n");
    printf ("WARNING: Any board size smaller 4x4 will result in an impossible to win match!");
    printf("\n\t - Also, any arbitrarily large board will wrap around the \n\t   terminal and will cause formatting issues!\n");
    printf("\t - Stay below 44 rows, 44 columns to avoid formatting issues!\n\t");
    printf("\n\t - ALSO, in multiplayer, the loser of a round will\n\t   go FIRST in the next round!!\n\n\t");
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
    *row = rowInput; //assign integer input to parameter pointer
    *column = columnInput; //assign integer input to parameter pointer
    return;
} //end void

//char function to choose what to fill a board index with --  a character (piece)
char getFilled(board *board, int x, int y){
    if(board->map[x][y]->filled == 1) return 'X'; //player1
    if(board->map[x][y]->filled == 2) return 'O'; //player2
    if(board->map[x][y]->filled == 0) return ' '; //assigned blank by default
} //end char


//board* function to fill the board index with the chosen, returned character from getFilled(board*, int ,int)
board* setFilled(board *board, int x, int y, int fill){
    board->map[x][y]->filled = fill; //the piece is now placed
    //printf("FILLED, Filled %d, %d with %d. Proof: %d\n", x, y, fill, board->map[x][y]->filled);
    return board;
} //end board*

//void function to display game mode statistics while playing the game
//the function also prints the board format accordingly based on the user-input row and column sizes
void printBoard(board *board, char gameMode, int row, int column, int playerOne, int playerTwo) { //function to print dynamic board
    int rowCount, columnCount, line, label = 0;
    //system("clear");//clear screen***************
    printf("==============================================================================\n");
    if (gameMode == 'A') { //if singleplayer, print singleplayer title
        printf("===== Game Mode: Singleplayer =============================================\n");
    }
    else if (gameMode == 'B') { //if multiplayer (human), print multiplayer (human) title
        printf("===== Game Mode: Multiplayer (Human) ======================================\n");
    }
    else if (gameMode == 'C') { //if multiplayer (computer), print multiplayer (computer) title
        printf("===== Game Mode: Multiplayer (Computer) ===================================\n");
    }
    printf("===== Board size: %d rows, %d columns =======================================\n", row, column);
    printf("===== Scoreboard (Player 1: [%d], Player 2: [%d]) ===========================\n", playerOne, playerTwo);
    printf("==============================================================================\n\n"); //ui printing

    //Note: The print design was initally inspired from an external source, but the board had to be configured to work with a 2D array structure rather than a singular array
    //Source: https://codereview.stackexchange.com/questions/27446/connect-four-game
    for (rowCount = 0 ; rowCount < board->rows; rowCount++) { //for rowCount is less than the actual board's rows
    for (columnCount = 0 ; columnCount < board->cols; columnCount++) { //for columnCount is less than the acutal board's columns
    printf("|"); //print divider separating the pieces
    printf(" %c ", getFilled(board, rowCount, columnCount)); //print blank character pieces initally
}
puts("|"); //print last divider on the row
for (line = 0; line < board->cols; line++) { //for line less than board's actual column count, line will format the lining between each row
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
printf("\n");
} //end void

//char function to call aiBehavior in order for the AI to determine its next best move
//then, once it finds an optimal blank spot, it will place its piece, and then checkForWinner will check to see if the move results in victory

char playerTurn(board* board, int turnChange) { //char function  to let player choose a column to place next piece
    int ro, co = 0; //integers to help navigate where the piece will be placed
    int retry = 0;
    while (retry != 1) {
        printf("Choose a column (from 1 to %d)", board->cols);
        printf(", Player %d: ", turnChange); // turnChange indicates which player goes next. Refer back to main() for modulus operation
        while(1) { //while will execute until it is broken out of
            if (1 != scanf("%d", &co) || co < 1 || co > board->cols) { //if the player does not choose a valid column on the board
                while(getchar() != '\n'); //while will check to make sure that an invalid will not crash the program
                puts ("\nCannot place piece. Out of bounds!");
                printf("Choose a column (from 1 to %d)", board->cols);//board->cols == total number of columns
                printf(", Player %d: ", turnChange); //error message to prompt user to choose a correct column
            }
            else { //if the player chooses a valid column, escape the while loop
                break;
            }
        }
        co--; //decrement because the count technically starts from 0, not 1

        for (ro = board->rows - 1; ro >= 0; ro--) { //for loop to check that the bottom of the board is empty, if not, move up and check again
            //if(board->map[ro][co]->filled == 0 ) { //***
            if(getFilled(board, ro, co) == ' ') { //is the spot at the bottom blank?
                retry = 1;
                board = setFilled(board, ro, co, turnChange); //if so, place the player piece within the board spot
                return checkForWinner(board, ro, co); //check to see if the player won after placing the new piece
            }
            else if (getFilled(board, 0, co) != ' ') { //its printing off based on the amount of columns
                printf("\nThe column is full! Choose a different column!\n");
                //return checkForWinner(board, ro, co);
                retry = 0;
                break;
            }
        }
    }

    return 0;
}//end char

char aiTurn(board* board, int turnChange) {
    int columnChoice = aiBehavior(board);
    int ro;

    //*************fix checking

    for (ro = board->rows - 1; ro >= 0; ro--) { //for loop to check that the bottom of the board is empty, if not, move up and check again
        //if(board->map[ro][co]->filled == 0 ) { //***
        if (getFilled(board, ro, columnChoice) == ' ') { //is the spot at the bottom blank?
            board = setFilled(board, ro, columnChoice, turnChange); //if so, place the player piece within the board spot
            return checkForWinner(board, ro, columnChoice); //check to see if the player won after placing the new piece
        }
    }

    return 0;
}//end char

//each time the player or ai makes a move, the checkForWinner function is called to call all functions that could possibly end the game
int checkForWinner(board* board, int row, int column) { //function to call winner check functions
    if(horizontal(board, row) != 0) return horizontal(board, row); //check if horizontal win
    if(vertical(board, column) != 0) return vertical(board, column); //check if vertical win
    if(diagonal(board) != 0) return diagonal(board); //check if diagonal win
    if(tieGame(board) != 0) return tieGame(board); //check if tie
    return 0;
}//end int

//void function to print ui that shows which player wins the round
void declareWinner(int which, board *board, int playerOneScore, int playerTwoScore){ //function to print player scores before end of match
    if (which == 1 || which == 2) { //if winner is player 1 or player 2, print the winner
        printf("==============================================================================\n");
        printf("===== Congrats! Player %d has won! ========================================\n", which);
        printf("==============================================================================\n");
        printf("Current scores:\n\tPlayer One: %d\n\tPlayer Two: %d\n\n", playerOneScore, playerTwoScore);
    }
    else if (which == 3) { //if neither player wins by filling up the board
        printf("The game is a tie! No winner!\n");
    }
    freeBoard(board); //free memory used by the board when the game ends
}

//int function to scan the board for four of the same character in a row consecutively, ends game if true for, == 1 (X), == 2 (O)
int horizontal(board* board, int row) {
    int win = 0; //win counter... if win increments to four, we have detected four in a row
    int i = 0; //iterator

    for (i = 0; i < board->cols; i++) { //increment i to board column size
        if (board->map[row][i]->filled == 1) { //check if player one shape (X) is at specific index
            win++; //if so, increment the win counter
            if (win >= 4) { //if 4 in a row is achieved
                return 1;//return player 1 victory
                break;
            }
        }
        else {//if not 4 in a row, reset counter
            win = 0;
        }
    }

    win = 0, i = 0; //reinitialize counter and iterator

    for (i = 0; i < board->cols; i++) { //see previous loop, but only difference is that it is a check for 4 in a row for P2 (O)
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
}//end int

//int function to scan the board for four of the same character in a column consecutively, ends game if true for, == 1 (X), == 2 (O)
int vertical(board* board, int column) {
    int win = 0; //win counter
    int i = 0; //iterator

    for (i = 0; i < board->rows; i++) { //for when i is less than the total board's rows //**** SHOULD BE =
    if (board->map[i][column]->filled == 1) { //if the specific position contains 'X'
    win++; //increment win counter
    if (win >= 4) { //if 4 in a row are detected
        return 1; //player 1 wins
        break;
    }
}
else {
    win = 0; //if there is not 4 in a row, reset counter to 0
}
}
win = 0, i = 0; //reinitialize

for (i = 0; i < board->rows; i++) { //see previous loop, but only difference is that it is a check for 4 in a row for P2 (O)
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
}//end int

//int function to scan for front diagonal and back diagonals
int diagonal(board* board) {
    int winFront1 = 0; //win counter
    int winBack1 = 0;
    int winFront2 = 0;
    int winBack2 = 0;
    int i = 0;//iterator
    int j = 0;//iterator
    int k = 0;

    //Check Front: Player 1 (Working)
    //we compare j to board columns minus 3, because if we compare to minus 2,
    //it would be impossible to obtain a diagonal 4 in a row, because we go too far along the board

    //LOOP 1: Forward-Right Check (WORKING)
    for (j = 0; j < (board->cols - 3); j++) {
        for (i = 0; ((board->rows-i-1) >= 0) && ((i+j) < board->cols); i++) { //check front  right player 1 ((board->rows-i-1) >= 0)******* WORKING
            //printf("Check sides j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-1, i+j);

            if (board->map[board->rows-i-1][i+j]->filled == 1) { //if the bottom-left child of placed piece is the same 'X'
            //if (board->map[board->rows-i-1][i+j]->filled == 1)
            winFront1++; //increment
            //printf("Win\n");
            if (winFront1 >= 4) { //if 4 in a row, player 1 wins
                //printf("Winner\n");
                return 1;
                break;
            }
        }
        else winFront1 = 0; //if not 4 in a row, restart counter

        if (board->map[board->rows-i-1][i+j]->filled == 2) { //if the bottom-left child of placed piece is the same 'O'
            //if (board->map[board->rows-i-1][i+j]->filled == 1)
            winFront2++; //increment
            //printf("Win\n");
            if (winFront2 >= 4) { //if 4 in a row, player 1 wins
                //printf("Winner\n");
                return 2;
                break;
            }
        }
        else winFront2 = 0; //if not 4 in a row, restart counter
    }//end inner for
        winFront1 = 0; //redeclare to 0.... if you dont, it will wrap to next diagonal and count is as part of the four consecutive pieces
        winFront2 = 0;
    }//end outer for

winFront1 = 0, winBack1 = 0, winFront2 = 0, winBack2 = 0, i = 0, j = 0, k = 0; //reinitialize

k = 1; //increment k to one, as we will need to find the front forward diagonals above the bottom left index of the board

//LOOP 2: Forward-up(WORKING)
for (j = 0; j < (board->rows - 4); j++) {
    for (i = 0; ((board->rows-i-k-1) >= 0) && ((i) < board->cols); i++) { //check front up player 1 ((board->rows-i-1) >= 0)******* WORKING
        //printf("Check up j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-k-1, i);

        if (board->map[board->rows-i-k-1][i]->filled == 1) { //Check forward up for Player 1
        //if (board->map[board->rows-i-1][i+j]->filled == 1)
        winFront1++; //increment
        //printf("Win\n");
        if (winFront1 >= 4) { //if 4 in a row, player 1 wins
            //printf("Winner\n");
            return 1;
            break;
        }
    }
    else winFront1 = 0; //if not 4 in a row, restart counter

    if (board->map[board->rows-i-k-1][i]->filled == 2) { //Check forward up for Player 2
    //if (board->map[board->rows-i-1][i+j]->filled == 1)
    winFront2++; //increment
    //printf("Win\n");
    if (winFront2 >= 4) { //if 4 in a row, player 1 wins
        //printf("Winner\n");
        return 2;
        break;
    }
}
else winFront2 = 0; //if not 4 in a row, restart counter
}//end inner for

k++; //increment k to move up the board
winFront1 = 0; //redeclare to 0.... if you dont, it will wrap to next diagonal and count is as part of the four consecutive pieces
winFront2 = 0;
}//end outer for


winFront1 = 0, winBack1 = 0, winFront2 = 0, winBack2 = 0, i = 0, j = 0, k = 0; //reinitialize


//Check Back-left diagonal
//start the scan from the end!!!!!!!!!
//for (k = 0; k < board->rows - 3; k++){ //for loop iterator to check diagonals above the bottom left index
//LOOP 3: Back-left(WORKING)
for (j = 0; j < (board->cols - 3); j++) { //see previous loop for logic
    for (i = 0; ((board->rows-i-k-1) >= 0) && ((i+j) < board->cols); i++) { //check back-left player 1
        //printf("Checking back-left j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-k-1, board->cols-i-j-1);
        //    if (board->rows-i-1 = 0 && i+j = (board->cols-i-j-1 = )) {
        //        winFront1 = 0;
        //    }

        if (board->map[board->rows-i-k-1][board->cols-i-j-1]->filled == 1) { //Check back-left player 1
        winBack1++;
        //printf("Win\n");
        if (winBack1 >= 4) {
            //printf("Winner\n");
            return 1;
            break;
        }

    }
    else winBack1 = 0; //restart counter

    if (board->map[board->rows-i-k-1][board->cols-i-j-1]->filled == 2) { //Check back-left player 2
    winBack2++;
    //printf("Win\n");
    if (winBack2 >= 4) {
        //printf("Winner\n");
        return 2;
        break;
    }

}
else winBack2 = 0; //restart counter
} //end inner for loop
winBack1 = 0;//redeclare to 0.... if you dont, it will wrap to next diagonal and count is as part of the four consecutive pieces
winBack2 = 0;
}//end outer for loop

winFront1 = 0, winBack1 = 0, winFront2 = 0, winBack2 = 0, i = 0, j = 0, k = 0; //reinitialize

k = 1;

//LOOP 4: Back-up
for (j = 0; j < (board->rows - 4); j++) { //see previous loop for logic
    for (i = 0; ((board->rows-i-k-1) >= 0) && ((i+j) < board->cols); i++) { //check back-left player 1
        //printf("Checking back-up j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-k-1, board->cols-i-1);
        //    if (board->rows-i-1 = 0 && i+j = (board->cols-i-j-1 = )) {
        //        winFront1 = 0;
        //    }
        if (board->map[board->rows-i-k-1][board->cols-i-1]->filled == 1) { //check back-up Player 1
        //printf("Win\n");
        if (winBack1 >= 4) {
            ////printf("Winner\n");
            return 1;
            break;
        }

    }
    else winBack1 = 0; //restart counter

    if (board->map[board->rows-i-k-1][board->cols-i-1]->filled == 2) { //check back-up Player 2
    winBack2++;
    //printf("Win\n");
    if (winBack2 >= 4) {
        //printf("Winner\n");
        return 2;
        break;
    }

}
else winBack2 = 0; //restart counter

}
k++;
winBack1 = 0;//redeclare to 0.... if you dont, it will wrap to next diagonal and count is as part of the four consecutive pieces
winBack2 = 0;
}//end for

winFront1 = 0, winBack1 = 0, winFront2 = 0, winBack2 = 0, i = 0, j = 0, k = 0; //reinitialize

return 0;
} //end int diagonal

//int function to check whether the iterators 'tie' and 'i' can detect whether every index on the top row --
//can detect whether each column contains a character that is not blank ('X' or 'O')
//if so, iterate tie, and if tie equals the amount of columns, the board is considered full.
//if no 4 in a row is detected on the last turn, the game is tied -- no winner.
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
} //end int

//char function to let the user use char 'rematchGame' to decide whether he wants to play another match in the same game mode.
//the function makes sure input is properly converted to uppercase, while avoiding possible crashing issues if a strange value is entered
char rematchGame(int returning) {
    char returner;
    while (returner != 'Y' && returner != 'N') {
        printf("Would you like to play another round (rematch)?\n\tYes [Y], No [N]: ");
        scanf(" %c", &returner);
        returner = toupper(returner);
        if (returner == 'Y') {
            returning = 1;
            return returning;
            break;
        }
        else if (returner == 'N') { //quit game after rejecting rematch
            printf("=======================================================================\n");
            printf("===== Thank You For Playing! ============================================\n");
            printf("===========================================================================\n");
            exit(0);
        }
        if (returner != 'Y' && returner != 'N') { //check to make sure a valid charaxter is entered
            printf("Please enter a valid character!\n");
        }
    }
    printf("\n");
}//end char

//int function to determine the AI behavior.
//the function collaborates with the iterative DFS function to determine what is the next best move based on --
//the pointer attributes of the current piece and the previous piece placed
int aiBehavior(board* board) {
    //LAST THING: Let AI check horizontal and vertical pieces, iterative, check last piece placed
    //row and column will be used to pinpoint where the piece is placed, and then we will evaluate its neighbor (children)

    int nbm = 0; //next best move
    int nbm_column = -1; //next best move column
    int i = 0; //iterator for rows
    pot* piece; //pot containing the structure attributes of each hole in the board
    int c = 0;//replaces longest streak if bigger
    int j = 0; //iterator for cols
    int k = 0; //iterator for neighbor pointers
    int p = 0;

    for (j = 0; j < board->cols; j++) { //for when j is less than the board's column size
    for (i = 0; i < board->rows; i++) { //search a column, and go down until you find the first filled piece
        if (board->map[i][j]->filled == 1){// || board->map[i][j]->filled == 2) { //if the first piece found is X or O******** causes seg fault
            break;
        }
    }

    if (i == board->rows) continue; //if i becomes equivalent to the total number of rows, continue

    p = board->map[i][j]->filled; //p contains the integer value of filled state (0 if ' ', 1 if 'X', 2 if 'O')
    if (p == 2) { //if P is 'O'
    for (k = 0; k < 5; k++) { //iterate k and assign the children neighbors
        piece = board->map[i][j]->neighbor[k]; //j is a pot pointer
        if (piece == NULL) continue;
        if (piece->filled == 2) {
            c = 1;
            c4IterativeDFS(piece, k, &c); //pot* piece, k, &c
            if (board->map[0][j]->filled != 0) {//************************
                c4IterativeDFS(piece, k, &c);
            }//***********************************************
            else if (board->map[0][j]->filled == 0) { //**** = 0
                if (c > nbm) {
                    nbm = c; //c replaces next best move
                    if (k == 0 || k == 1) nbm_column = (j == board->cols - 1) ? j : j + 1; //left and bottom left children
                    else if (k == 2) nbm_column = j; //bottom child
                    else nbm_column = (j == 0) ? j : j - 1; //thus, the next best column is updated, bottom right and right child
                } //end innest if
            }
        }
    } //end inner for

    if (i != 0) { //previou
        pot* prev = board->map[i - 1][j];
        for (k = 0; k < 5; k++) {
            piece = prev->neighbor[k];
            if (piece == NULL) continue;
            if (piece->filled == 2) {
                c = 1;
                c4IterativeDFS(piece, k, &c);
                if (board->map[0][j]->filled != 0) {//************************
                    c4IterativeDFS(piece, k, &c);
                }//***********************************************
                else if (board->map[0][j]->filled == 0) { //****** = 0
                    if (c > nbm) {
                        nbm = c;
                        if (k == 0 || k == 1) nbm_column = (j == board->cols - 1) ? j : j + 1;
                        else if (k == 2) nbm_column = j;
                        else nbm_column = (j == 0) ? j : j - 1; //thus, the next best column is updated
                    }
                }
            }
        }
    }
}//end middle if
} //end inner if

if (nbm_column == -1) {//if a next best move cannot be located, we will randomize the column selection to allow it to continue functioning
    nbm_column = rand() % board->cols; //
    while (board->map[0][nbm_column]->filled != 0) { //while loop to make sure the AI is not placing in a full column
        nbm_column = rand() % board->cols; //if the column is full, randomly select a new column
    }
}
return nbm_column; //return next best move column
//play at saved column
}//end int function//

//DFS Pseudocode Source of Inspiration: https://en.wikipedia.org/wiki/Depth-first_search

void c4IterativeDFS(pot* piece, int j, int* c) { //seg fault reading in j and c
    if (piece == NULL) return; //BASE CASE

    pot* child = piece->neighbor[j]; //from the perspective of the placed piece's neighbor, we will determine if it the shapes match.

    if (piece->filled == 2) { //if the piece is 'O'
    *(c) = *(c)+1; //iterate ****
    c4IterativeDFS(child, j, c); //rerun the iterative process
}
}//end void
