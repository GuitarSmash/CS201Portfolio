# CS201Portfolio: Connect Four by Jake Williams
- The University of Alabama
- CS201: Data Structures and Algorithms
- Dr. Monica Anderson
- Spring 2019
- Due Date: March 31, 2019

Sources
----------------------------------------------------------------------------------------------------------------------------------------
- Connect Four game: Modified Print Function
  - https://codereview.stackexchange.com/questions/27446/connect-four-game
- Depth-first search: Modified Pseudocode
  - https://en.wikipedia.org/wiki/Depth-first_search
----------------------------------------------------------------------------------------------------------------------------------------

VIDEO TUTORIAL
----------------------------------------------------------------------------------------------------------------------------------------
- https://youtu.be/rgBT7dbH3nc
----------------------------------------------------------------------------------------------------------------------------------------

GENERAL USAGE NOTES
----------------------------------------------------------------------------------------------------------------------------------------
- Connect Four is a board game for up to 1 to 2 players. The game poses two opponents to alternately place shapes, either an 'X' or an 'O', within a board. Player 1 controls 'X', and Player 2 controls 'O'.
- The objective is to place four X's or four O's in a row on the board, whether the order be horizontal, vertical, or diagonal.
- The project was compiled in Ubuntu while using the Windows Subsystem for Linux.
- The project was created for college research purposes for The University of Alabama.
----------------------------------------------------------------------------------------------------------------------------------------

FEATURES
----------------------------------------------------------------------------------------------------------------------------------------
- ASCII Interface
- Three game modes:
  - Singleplayer
  - Multiplayer (Human)
  - Multiplayer (Computer)
- The user can dynamically modify board size to specific coordinates, but warnings are given where formatting issues can occur.
- The design implements a 2D array structure to construct the board dynamically.
- The score is tallied between consecutive matches, and it will be cleared once the user decides to stop playing the game.
- When playing multiplayer, the loser gets to go first on the next round.
- The computer (AI) functionality was created through a modified, unweighted iterative Depth-First Search algorithm.
  - The goal was to minimize runtime through linear functionality.
  - When the AI is faced with the issue of not locating a next best move, a rand function is performed to randomly choose a column to place the next piece.
----------------------------------------------------------------------------------------------------------------------------------------
