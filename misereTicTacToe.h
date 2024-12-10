#ifndef MISERETICTACTOE_H
#define MISERETICTACTOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

class MisereTicTacToe : public Board<char> {
private:
    bool isHardAI;
    char player1Symbol;
    char player2Symbol;


public:
    MisereTicTacToe() : Board(3, 3), isHardAI(false), player1Symbol('X'), player2Symbol('O') {
        initializeBoard();
    }

    void initializeBoard() override {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                board[i][j] = ' ';
            }
        }
        currentPlayerSymbol = 'X';  // First player starts as 'X'
    }

    void display_board() override {
        cout << '\n';
        int num = 1;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (board[i][j] == ' ')
                    cout << num;
                else
                    cout << board[i][j];
                if (j < columns - 1)
                    cout << " | ";
                num++;
            }
            cout << endl;
            if (i < rows - 1)
                cout << "--|---|---" << endl;
        }
        cout << endl;
    }

    bool is_valid_move(int pos) override {
        int row = (pos - 1) / columns;
        int col = (pos - 1) % columns;
        return (pos >= 1 && pos <= 9 && board[row][col] == ' ');
    }

    bool is_win(char currentPlayerSymbol) override {
        for (int i = 0; i < rows; ++i) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] == currentPlayerSymbol)
                return true;
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] == currentPlayerSymbol)
                return true;
        }
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == currentPlayerSymbol)
            return true;
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] == currentPlayerSymbol)
            return true;

        return false;
    }

    bool is_draw() override {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == ' ')
                    return false;
            }
        }
        return true;
    }

    bool update_board(int pos, char symbol) override {
        int row = (pos - 1) / columns;
        int col = (pos - 1) % columns;
        if (is_valid_move(pos)) {
            board[row][col] = symbol;
            return true;
        }
        return false;
    }

    void setPlayerSymbols(char player1Symbol, char player2Symbol) {
        this->player1Symbol = player1Symbol;
        this->player2Symbol = player2Symbol;
        currentPlayerSymbol = player1Symbol;
    }

    char get_current_symbol() {
        return currentPlayerSymbol;
    }

    void switch_turn() {
        if (currentPlayerSymbol == player1Symbol) {
            currentPlayerSymbol = player2Symbol;
        }
        else {
            currentPlayerSymbol = player1Symbol;
        }
    }

    bool update_board_auto(int row, int col, char symbol) {
        if (board[row][col] == ' ') { // Check if the position is empty
            board[row][col] = symbol;
            return true;
        }
        return false;
    }


    void easyAI(int& row, int& col) {
        srand(time(0));
        do {
            row = rand() % rows;
            col = rand() % columns;
        } while (board[row][col] != ' ');
    }

    void hardAI(char aiSymbol, char playerSymbol, int &row, int &col) {
    // Priority: 1) Make a move that forces the opponent to form 3 in a row (win condition for Misere rules)
    //           2) Avoid creating a row of 3 for the AI itself.

    // Search for a move that forces the user to make a row of 3
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == ' ') {
                // Temporarily mark AI's move
                board[i][j] = aiSymbol;

                // Check if this move forces the player to form a row of 3
                bool userForced = false;
                for (int x = 0; x < rows; ++x) {
                    for (int y = 0; y < columns; ++y) {
                        if (board[x][y] == ' ') {
                            // Temporarily mark player's move
                            board[x][y] = playerSymbol;
                            if (is_win(playerSymbol)) {
                                userForced = true; // If user would form 3 in a row here, this is a good move for AI
                            }
                            board[x][y] = ' '; // Undo player's move
                        }
                    }
                }

                // If this move forces the user to form 3 in a row, use it
                if (userForced) {
                    row = i;
                    col = j;
                    board[i][j] = ' '; // Undo AI's move for now
                    return;
                }

                board[i][j] = ' '; // Undo AI's move
            }
        }
    }

    // If no forcing move is found, avoid creating 3 in a row
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == ' ') {
                board[i][j] = aiSymbol; // Temporarily mark AI's move

                if (!is_win(aiSymbol)) { // If AI doesn't form a row of 3, this move is okay
                    row = i;
                    col = j;
                    board[i][j] = ' '; // Undo AI's temporary move
                    return;
                }

                board[i][j] = ' '; // Undo AI's move
            }
        }
    }

    // If no move avoids making a win for AI, fallback to random move as a last resort
    easyAI(row, col);
}


};

void playerVsPlayer(MisereTicTacToe &game) {
    char player1Symbol = 'X', player2Symbol = 'O';

    game.setPlayerSymbols(player1Symbol, player2Symbol);

    while (!game.is_win(player1Symbol) && !game.is_win(player2Symbol) && !game.is_draw()) {
        game.display_board();
        int pos;
        cout << "Player " << (game.get_current_symbol() == player1Symbol ? "1" : "2")
             << ", enter your move (1-9): ";
        cin >> pos;

        if (game.update_board(pos, game.get_current_symbol())) {
            game.switch_turn();
        } else {
            cout << "Invalid move! Try again.\n";
        }
    }

    game.display_board();
    if (game.is_win(player1Symbol)) {
        cout << "Player 2 Wins!\n";
    }
    else if (game.is_win(player2Symbol)) {
        cout << "Player 1 Wins!\n";
    }
    else {
        cout << "It's a Draw.\n";
    }
}

void playerVsComputer(MisereTicTacToe &game) {
    char playerSymbol = 'X', aiSymbol = 'O';

    game.setPlayerSymbols(playerSymbol, aiSymbol);

    while (!game.is_win(playerSymbol) && !game.is_win(aiSymbol) && !game.is_draw()) {
        int row, col;
        game.display_board();
        if (game.get_current_symbol() == playerSymbol) {
            int pos;
            cout << "Player, enter your move (1-9): ";
            cin >> pos;

            if (game.update_board(pos, playerSymbol)) {
                game.switch_turn();
            } else {
                cout << "Invalid move! Try again.\n";
            }
        } else {
            game.easyAI(row, col);
            game.update_board_auto(row, col, aiSymbol);
            game.switch_turn();
            cout << "Computer move to (" << row * 3 + col + 1 << ").\n";
        }
    }

    if (game.is_win(playerSymbol)) {
        game.display_board();
        cout << "Computer wins!\n";
    }
    else if (game.is_win(aiSymbol)) {
        game.display_board();
        cout << "Player wins!\n";
    }
    else {
        game.display_board();
        cout << "It's a Draw.\n";
    }
}

void playerVsAI(MisereTicTacToe &game) {
    char playerSymbol = 'X', aiSymbol = 'O';

    game.setPlayerSymbols(playerSymbol, aiSymbol);

    while (!game.is_win(playerSymbol) && !game.is_win(aiSymbol) && !game.is_draw()) {
        game.display_board();
        if (game.get_current_symbol() == playerSymbol) {
            int pos;
            cout << "Player, enter your move (1-9): ";
            cin >> pos;

            if (game.update_board(pos, playerSymbol)) {
                game.switch_turn();
            } else {
                cout << "Invalid move! Try again.\n";
            }
        } else {
            int row, col;
            game.hardAI(aiSymbol, playerSymbol, row, col);
            game.update_board_auto(row, col, aiSymbol);
            game.switch_turn();
            cout << "AI move to (" << row * 3 + col + 1 << ").\n";
        }
    }

    if (game.is_win(playerSymbol)) {
        game.display_board();
        cout << "AI wins!\n";
    }
    else if (game.is_win(aiSymbol)) {
        game.display_board();
        cout << "Player wins!\n";
    }
    else {
        cout << "It's a Draw.\n";
    }
}

void playMI() {
    MisereTicTacToe game;
    cout << "Welcome to Misere Tic Tac Toe!" << endl;
    cout << "Choose a mode:\n1. Player vs Player\n2. Player vs Easy Computer\n3. Player vs Hard AI\n=> ";
    string mode;
    cin >> mode;

    if (mode == "1") {
        playerVsPlayer(game);
    }
    else if (mode == "2") {
        playerVsComputer(game);
    }
    else if (mode == "3") {
        playerVsAI(game);
    }
    else {
        cout << "Invalid choice, Exiting...\n";
    }
}

#endif // MISERETICTACTOE_H