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
    bool isHardAI; // Flag for hard AI mode.
    char player1Symbol; // Symbol for player 1.
    char player2Symbol; // Symbol for player 2.

public:
    MisereTicTacToe() : Board(3, 3), isHardAI(false), player1Symbol('X'), player2Symbol('O') {
        initializeBoard(); // Initialize board with empty cells.
    }

    void initializeBoard() override {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                board[i][j] = ' '; // Set all cells to empty.
            }
        }
        currentPlayerSymbol = 'X'; // Start with 'X'.
    }

    void display_board() override {
        cout << '\n';
        int num = 1; // Number each cell.
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                cout << (board[i][j] == ' ' ? to_string(num) : string(1, board[i][j]));
                if (j < columns - 1) cout << " | ";
                num++;
            }
            cout << endl;
            if (i < rows - 1) cout << "--|---|---" << endl; // Row separator.
        }
        cout << endl;
    }

    bool is_valid_move(int pos) override {
        int row = (pos - 1) / columns;
        int col = (pos - 1) % columns;
        return (pos >= 1 && pos <= 9 && board[row][col] == ' '); // Check valid position.
    }

    bool is_win(char currentPlayerSymbol) override {
        // Check rows, columns, and diagonals for a win.
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
        // Check if all cells are filled.
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == ' ') return false; // Empty cell found.
            }
        }
        return true; // No empty cells.
    }

    bool update_board(int pos, char symbol) override {
        int row = (pos - 1) / columns;
        int col = (pos - 1) % columns;
        if (is_valid_move(pos)) {
            board[row][col] = symbol; // Place the symbol.
            return true;
        }
        return false; // Invalid move.
    }

    void setPlayerSymbols(char player1Symbol, char player2Symbol) {
        this->player1Symbol = player1Symbol;
        this->player2Symbol = player2Symbol;
        currentPlayerSymbol = player1Symbol; // Start with player 1.
    }

    char get_current_symbol() {
        return currentPlayerSymbol; // Return current symbol.
    }

    void switch_turn() {
        // Alternate between players.
        currentPlayerSymbol = (currentPlayerSymbol == player1Symbol) ? player2Symbol : player1Symbol;
    }

    bool update_board_auto(int row, int col, char symbol) {
        if (board[row][col] == ' ') { // Check if cell is empty.
            board[row][col] = symbol; // Place symbol.
            return true;
        }
        return false; // Invalid move.
    }

    void easyAI(int& row, int& col) {
        srand(time(0)); // Randomize seed.
        do {
            row = rand() % rows;
            col = rand() % columns;
        } while (board[row][col] != ' '); // Find empty cell.
    }

    void hardAI(char aiSymbol, char playerSymbol, int &row, int &col) {
        // Prioritize forcing the opponent to win.
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = aiSymbol; // Simulate AI move.
                    bool userForced = false;
                    for (int x = 0; x < rows; ++x) {
                        for (int y = 0; y < columns; ++y) {
                            if (board[x][y] == ' ') {
                                board[x][y] = playerSymbol; // Simulate player move.
                                if (is_win(playerSymbol)) userForced = true;
                                board[x][y] = ' '; // Undo player move.
                            }
                        }
                    }
                    if (userForced) {
                        row = i;
                        col = j;
                        board[i][j] = ' '; // Undo AI move.
                        return;
                    }
                    board[i][j] = ' '; // Undo AI move.
                }
            }
        }

        // Avoid creating a win for AI.
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = aiSymbol; // Simulate AI move.
                    if (!is_win(aiSymbol)) { // Ensure no win.
                        row = i;
                        col = j;
                        board[i][j] = ' '; // Undo AI move.
                        return;
                    }
                    board[i][j] = ' '; // Undo AI move.
                }
            }
        }

        // Fallback to random move.
        easyAI(row, col);
    }
};

void playerVsPlayer(MisereTicTacToe &game) {
    // Prompt for player names and create Player objects
    string name1, name2;
    cout << "player1 enter your name: ";
    cin >> name1;
    Player player1(name1, 'X');
    cout << "player2 enter your name: ";
    cin >> name2;
    Player player2(name2, 'O');

    // Set the player symbols in the game
    game.setPlayerSymbols(player1.getsymbol(), player2.getsymbol());

    // Game loop: continues until a win or draw condition is met
    while (!game.is_win(player1.getsymbol()) &&
           !game.is_win(player2.getsymbol()) &&
           !game.is_draw()) {

        // Display the current state of the board
        game.display_board();

        int pos; // Variable for the player's move

        // Indicate whose turn it is
        if (game.get_current_symbol() == player1.getsymbol()) {
            cout << "Player " << player1.getname() << "'s turn." << endl;
        } else {
            cout << "Player " << player2.getname() << "'s turn." << endl;
        }

        // Prompt the player to enter their move
        cout << " enter your move (1-9): ";
        cin >> pos;

        // Attempt to update the board with the player's move
        if (game.update_board(pos, game.get_current_symbol())) {
            // Switch turns if the move is valid
            game.switch_turn();
        } else {
            // Notify the player if the move is invalid
            cout << "Invalid move! Try again.\n";
        }
    }

    // Display the final state of the board
    game.display_board();

    // Check and display the game result
    if (game.is_win(player1.getsymbol())) {
        cout << player2.getname() << " Wins!\n";
    } else if (game.is_win(player2.getsymbol())) {
        cout << player1.getname() << " Wins!\n";
    } else {
        cout << "It's a Draw.\n";
    }
}

void playerVsComputer(MisereTicTacToe &game) {
    // Get the player's name and set player and computer symbols
    string name;
    cout << "enter your name: ";
    cin >> name;
    Player player(name, 'X');
    game.setPlayerSymbols(player.getsymbol(), 'O');

    // Game loop: runs until a win or draw occurs
    while (!game.is_win(player.getsymbol()) && !game.is_win('O') && !game.is_draw()) {
        int row, col; // Variables for computer's move
        game.display_board(); // Show the board

        if (game.get_current_symbol() == player.getsymbol()) {
            // Player's turn
            int pos;
            cout << player.getname() << " enter your move (1-9): ";
            cin >> pos;

            // Update the board if move is valid, else ask again
            if (game.update_board(pos, player.getsymbol())) {
                game.switch_turn(); // Switch turn if move is valid
            } else {
                cout << "Invalid move! Try again.\n";
            }
        } else {
            // Computer's turn (easy AI)
            game.easyAI(row, col); // Compute random valid move
            game.update_board_auto(row, col, 'O'); // Place computer's move
            game.switch_turn(); // Switch turn
            cout << "Computer moves to (" << row * 3 + col + 1 << ").\n"; // Announce move
        }
    }

    // Display the result of the game
    if (game.is_win(player.getsymbol())) {
        game.display_board();
        cout << "Computer wins!\n";
    } else if (game.is_win('O')) {
        game.display_board();
        cout << player.getname() << " wins!\n";
    } else {
        game.display_board();
        cout << "It's a Draw.\n";
    }
}

void playerVsAI(MisereTicTacToe &game) {
    // Initialize the player's name and set symbols for player and AI
    string name;
    cout << "enter your name: ";
    cin >> name;
    Player player(name, 'X');
    game.setPlayerSymbols(player.getsymbol(), 'O');

    // Game loop: continues until there's a win or draw
    while (!game.is_win(player.getsymbol()) && !game.is_win('O') && !game.is_draw()) {
        game.display_board(); // Display the current board state

        if (game.get_current_symbol() == player.getsymbol()) {
            // Player's turn
            int pos;
            cout << player.getname() << " enter your move (1-9): ";
            cin >> pos;

            // Validate and update the board if the move is valid
            if (game.update_board(pos, player.getsymbol())) {
                game.switch_turn(); // Switch turn after a valid move
            } else {
                cout << "Invalid move! Try again.\n";
            }
        } else {
            // AI's turn (hard difficulty using Minimax or similar logic)
            int row, col;
            game.hardAI('O', player.getsymbol(), row, col); // Compute the best move
            game.update_board_auto(row, col, 'O'); // Apply AI's move
            game.switch_turn(); // Switch turn
            cout << "AI moves to (" << row * 3 + col + 1 << ").\n"; // Announce AI's move
        }
    }

    // Display the result of the game
    if (game.is_win(player.getsymbol())) {
        game.display_board();
        cout << "AI wins!\n";
    } else if (game.is_win('O')) {
        game.display_board();
        cout << player.getname() << " wins!\n";
    } else {
        cout << "It's a Draw.\n";
    }
}

void playMisere() {
    MisereTicTacToe game;
    // Display menu and get user's choice
    cout << "Welcome to Misere Tic Tac Toe!" << endl;
    cout << "Choose a mode:\n1. Player vs Player\n2. Player vs Easy Computer\n3. Player vs Hard AI\n=> ";
    string mode;
    cin >> mode;

    // Start the selected game mode
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
