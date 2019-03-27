#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int checkForWinner(board*, int, int); //scans the horizontal, vertical, and diagonal functions for 4 in a row
void declareWinner(int, board*, int, int); //tally scores
int horizontal(board*, int);
int vertical(board*, int);
int diagonal(board*, int, int);
int tieGame(board*, int); //checks to see if the board has been completely filled

int checkForWinner(board* board, int row, int column) {
    if(horizontal(board, row) != 0) return horizontal(board, row); //scan horizontal
    if(vertical(board, column) != 0) return vertical(board, column); //scan vertical
    if(diagonal(board, row, column) != 0) return diagonal(board, row, column); //scan diagonal
    if(tieGame(board, column) != 0) return tieGame(board, column); //when board is full, tie!
    return 0;
}

void declareWinner(int which, board *board, int playerOneScore, int playerTwoScore){ //display player scores
    printf("==============================================================================\n");
    printf("===== Congrats! Player %d has won! ========================================\n", which);
    printf("==============================================================================\n");
    printf("Current scores:\n\tPlayer One: %d\n\tPlayer Two: %d\n\n", playerOneScore, playerTwoScore); //display new scores
    freeBoard(board); //free memory used by the board when the game ends
}

int horizontal(board* board, int row) { //horizontal check with board and row parameter
    int win = 0; //count for 4 in a row
    int i = 0; //for-loop, column iterator

    for (i = 0; i < board->cols; i++) { //if i is less than total of columns, increment
        if (board->map[row][i]->filled == 1) { //if column i is filled with an 'X', count the total number of X's
            win++;
            if (win >= 4) { //4 in a row found
                return 1; //return Player 1 victory
                break;
            }
        }
        else { //if 4 in a row is not detected, reset count to 0
            win = 0;
        }
    } //end for

    win = 0, i = 0; //re-initalize

    for (i = 0; i < board->cols; i++) { //if i is less than total of columns, increment
        if (board->map[row][i]->filled == 2) { ////if column i is filled with an 'O', count the total number of O's
            win++;
            if (win >= 4) { //4 in a row found
                return 2; //return Player 2 victory
                break;
            }
        }
        else { //if 4 in a row is not detected, reset count to 0
            win = 0;
        }
    } //end for
    return 0; //return 0, or ' '
}

int vertical(board* board, int column) { //horizontal check with board and column parameter
    int win = 0; //count for 4 in a column
    int i = 0; //for-loop, row iterator

    for (i = 0; i < board->rows; i++) { //if i is less than total of rows, increment for player 1
        if (board->map[i][column]->filled == 1) { //check player one
            win++;
            if (win >= 4) { //if 4 in a row, player 1 wins
                return 1;
                break;
            }
        }
        else { //if not 4 in a row, reset count to 0
            win = 0;
        }
    }
    win = 0, i = 0; //reinitialize

    for (i = 0; i < board->rows; i++) { //->rows
        if (board->map[i][column]->filled == 2) { //if i is less than total of rows, increment for player 2
            win++;
            if (win >= 4) {//if 4 in a row, player 2 wins
                return 2;
                break;
            }
        }
        else { //if not 4 in a row, reset count to 0
            win = 0;
        }
    }
    return 0; //return 0, or ' '
}

int diagonal(board* board, int row, int column) {
    int win = 0; //4 in a row counter
    int i = 0; //i, j are for loop iterators to navigate front and back diagonals
    int j = 0;

    for (j = 0; j < (board->cols - 3); j++) { // it is less than board->cols - 3, because it is impossible to get a forward diagonal on the last 3 columns
        for (i = 0; ((board->rows-i-1) >= 0) && ((i+j) < board->cols); i++) { //check front player 1
            //printf("Checking j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-1, i+j);
            if (board->map[board->rows-i-1][i+j]->filled == 1) { //check front diagonal indices for 'X'
                win++;
                //printf("Win\n");
                if (win >= 4) { //if 4 in a row, Player 1 wins
                    //printf("Winner\n");
                    return 1;
                    break;
                }

            }
            else win = 0; //if not 4 in a row, count goes back to 0
        }
    }

    win = 0, i = 0, j = 0; //reinitialize

    for (j = 0; j < (board->cols - 3); j++) { //this for loop is used to find the back diagonal of 'X's
        for (i = 0; ((board->rows-i-1) >= 0) && ((i+j) < board->cols); i++) { //check back player 1
            //printf("Checking j:%d, i:%d, map[%d][%d]\n",j,i, board->rows-i-1, board->cols-i-j-1);
            if (board->map[board->rows-i-1][board->cols-i-j-1]->filled == 1) { //j is subtract to decrement 2nd indices to traverse back
                win++;
                //printf("Win\n");
                if (win >= 4) { //if 4 in a row, Player 1 wins
                    //printf("Winner\n");
                    return 1;
                    break;
                }

            }
            else win = 0; //if not 4 in a row, count goes back to 0
        }
    }

    win = 0, i = 0, j = 0; //reinitialize


    for (j = 0; j < (board->cols - 3); j++) { //front diagonal check for player 2.... see front player 1 for function
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

    for (j = 0; j < (board->cols - 3); j++) { //back diagonal check for player 2.... see back player 1 for function
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
    return 0; //return 0, or ' '
}

int tieGame(board* board, int column) {
    int tie = 0;
    int i = 0;
return 0;
}
