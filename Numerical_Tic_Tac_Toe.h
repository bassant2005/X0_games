#ifndef UNTITLED_NUMERICAL_TIC_TAC_TOE_H
#define UNTITLED_NUMERICAL_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>
#include <set>

using namespace std;

class Numerical_Tic_Tac_Toe : public Board<int>{
private:
    set<int> usedNumbers;
    int lastRow;    // Stores the row of the last attempted move
    int lastCol;    // Stores the column of the last attempted move
    int lastNum;    // Stores the number of the last attempted move
    bool isPlayer1; // Indicates if the current player is Player 1

public:
    Numerical_Tic_Tac_Toe(): Board<int>(3,3){
        initializeBoard();
        currentPlayerSymbol = '1';
        usedNumbers.clear();
        srand(time(0));
    }
    void initializeBoard() override {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                this->board[i][j] = 0;
            }
        }
    }

    void display_board() override{
        cout << "\n";
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0)
                    cout << " - ";
                else
                    cout << " " << board[i][j] << " ";

                if (j < 2) cout << "|";
            }
            cout << "\n";
            if (i < 2) cout << "---|---|---\n";
        }
        cout << "\n";
    }

    bool is_valid_move(int move) override {
        PositionToRowCol(move);
        if (lastRow < 0 || lastRow >= 3 || lastCol < 0 || lastCol >= 3) {
            return false; // Invalid row or column
        }
        if (board[lastRow][lastCol] != 0) {
            return false; // Position already occupied
        }
        if (usedNumbers.find(lastNum) != usedNumbers.end()) {
            return false; // Number already used
        }
        if (isPlayer1 && (lastNum % 2 == 0 || lastNum < 1 || lastNum > 9)) {
            return false; // Player 1 must use odd numbers
        }
        if (!isPlayer1 && (lastNum % 2 != 0 || lastNum < 2 || lastNum > 8)) {
            return false; // Player 2 must use even numbers
        }
        return true; // Valid move
    }

    void PositionToRowCol(int move){
        lastRow = (move - 1) / 3;
        lastCol = (move - 1) % 3;
    }

    void setMove(int row, int col, int num, bool currentPlayer) {
        lastRow = row;
        lastCol = col;
        lastNum = num;
        isPlayer1 = currentPlayer;
    }

    void makeMove(int move) {
        if (is_valid_move(move)) {
            board[lastRow][lastCol] = lastNum;
            usedNumbers.insert(lastNum);
        } else {
            cout << "Invalid move. Try again.\n";
        }
    }

    bool is_win(char currentPlayerSymbol) override {
        // Check rows for sum = 15
        for (int i = 0; i < 3; i++) {
            if (board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0 &&
                board[i][0] + board[i][1] + board[i][2] == 15) {
                return true; // Row win
            }
        }

        // Check columns for sum = 15
        for (int i = 0; i < 3; i++) {
            if (board[0][i] != 0 && board[1][i] != 0 && board[2][i] != 0 &&
                board[0][i] + board[1][i] + board[2][i] == 15) {
                return true; // Column win
            }
        }

        // Check diagonals for sum = 15
        if (board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0 &&
            board[0][0] + board[1][1] + board[2][2] == 15) {
            return true; // Diagonal win (top-left to bottom-right)
        }

        if (board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0 &&
            board[0][2] + board[1][1] + board[2][0] == 15) {
            return true; // Anti-diagonal win (top-right to bottom-left)
        }

        return false; // No win found
    }

    bool is_draw() override{

    };



};

class Numerical_Tic_Tac_Toe_MinMax_Player : public Player<char>{

};

class Numerical_Tic_Tac_Toe_Random_Player : public Player<char>{

};

void PlayerVsComputer(){

}

void PlayerVsPlayer(){

}

void PlayNumTicTacToe(){

}

#endif
