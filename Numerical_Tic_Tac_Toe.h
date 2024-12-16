#ifndef UNTITLED_NUMERICAL_TIC_TAC_TOE_H
#define UNTITLED_NUMERICAL_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
#include <set>

using namespace std;

// Set to track used numbers on the board
set<int> usedNumbers;

// Board class for Numerical Tic-Tac-Toe game
class NumericalTicTacToe_Board : public Board<int> {
public:
    // Constructor initializing board with 3x3 size
    NumericalTicTacToe_Board(int i, int i1) : Board<int>(i, i1) {
        initializeBoard();
        usedNumbers.clear(); // Clear any previously used numbers
    }

    // Initialize the board with empty cells (represented by 0)
    void initializeBoard() override {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                this->board[i][j] = 0; // 0 represents an empty cell
            }
        }
    }

    // Display the board to the user
    void display_board() override {
        cout << "\n";
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0)
                    cout << " * "; // Empty cell
                else
                    cout << " " << board[i][j] << " "; // Display the number

                if (j < 2) cout << "|"; // Display column separators
            }
            cout << "\n";
            if (i < 2) cout << "---|---|---\n"; // Display row separators
        }
        cout << "\n";
    }

    // Check if a player has won (sum of 15 in a row, column, or diagonal)
    bool is_win(int) override {
        // Check rows and columns for sum = 15
        for (int i = 0; i < 3; i++) {
            if (board[i][0] + board[i][1] + board[i][2] == 15 &&
                board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0) {
                return true;
            }
        }

        for (int i = 0; i < 3; i++) {
            if (board[0][i] + board[1][i] + board[2][i] == 15 &&
                board[0][i] != 0 && board[1][i] != 0 && board[2][i] != 0) {
                return true;
            }
        }

        // Check diagonals for sum = 15
        if (board[0][0] + board[1][1] + board[2][2] == 15 &&
            board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0) {
            return true;
        }
        if (board[0][2] + board[1][1] + board[2][0] == 15 &&
            board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0) {
            return true;
        }

        return false;
    }

    // Check if the game is a draw (no empty spaces left)
    bool is_draw() override {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (this->board[i][j] == 0) return false;  // There's an empty spot
            }
        }
        return true;  // No empty spots, it's a draw
    }

    // Check if a move is valid (position not occupied and within bounds)
    bool is_valid_move(int move) override {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        return (move >= 1 && move <= 9 && board[row][col] == 0); // Only check position
    }

    // Update the board with the given move and number
    bool update_board(int move, int num) override {
        if (!is_valid_move(move) || usedNumbers.count(num) > 0) return false;  // Make sure number isn't used
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        board[row][col] = num;
        usedNumbers.insert(num); // Track used number
        return true;
    }

    // Check if a number is valid for a player based on their symbol (odd or even)
    bool is_valid_number_for_player(int number, int playerSymbol) {
        if (usedNumbers.count(number) > 0)
            return false; // Check if number is already used

        if (playerSymbol == 1 && (number < 1 || number > 9 || number % 2 == 0))
            return false; // Player 1 uses odd numbers

        if (playerSymbol == 2 && (number < 1 || number > 9 || number % 2 != 0))
            return false; // Player 2 uses even numbers

        return true;
    }
};

// Random player AI for the Numerical Tic-Tac-Toe game
class NumericalTicTacToe_Random_Player : public Player<int> {
private:
    Board<int>* boardPtr;

public:
    NumericalTicTacToe_Random_Player(Board<int>* board, string name, int symbol) : Player<int>(name, symbol), boardPtr(board) {
        srand(static_cast<unsigned>(time(0))); // Seed random number generator
    }

    string getname() override {
        return name;
    }

    // Generate a random valid move for the AI player
    void getRandomMove() {
        int move, number;

        while (true) {
            move = rand() % 9 + 1;  // Random move (1-9)

            if (!boardPtr->is_valid_move(move)) continue; // Retry if the move is already occupied

            do {
                number = rand() % 9 + 1;  // Random number (1-9)
            } while (!is_valid_number(number));

            if (boardPtr->update_board(move, number)) break; // Update board once valid
        }
    }

    // Check if a number is valid for the player's symbol
    bool is_valid_number(int num) {
        if (usedNumbers.count(num) > 0) return false;  // Number already used
        if (symbol == 1 && (num < 1 || num > 9 || num % 2 == 0)) return false; // Player 1 uses odd
        if (symbol == 2 && (num < 1 || num > 9 || num % 2 != 0)) return false; // Player 2 uses even
        return true;
    }
};

class NumericalTicTacToe_MinMax_Player : public Player<int> {
private:
    Board<int>* boardPtr;

public:
    NumericalTicTacToe_MinMax_Player(Board<int>* board,string name, int symbol) : Player<int>(name,symbol), boardPtr(board) {}

    string getname() override{
        return name;
    }

    pair <int, int> minimax(int depth, bool isMaximizing, int alpha, int beta){
        int score = evaluate_board();

        // If the AI or the player has already won, return the score
        if (score == 10 || score == -10) {
            return make_pair(score, -1);
        }

        // If no more moves are left, return 0 for draw
        if (is_board_full()) {
            return make_pair(0, -1);
        }

        int bestScore = (isMaximizing) ? -1000 : 1000;
        int bestMove = -1;

        // Declare AI and player variables
        char aiPlayer = '2';
        char player = '1';

        for (int move = 1; move <= 9; move++) {
            // Check if the move is valid (position is empty and number not used)
            for (int num = (isMaximizing ? (aiPlayer == '1' ? 1 : 2) : (player == '1' ? 1 : 2));
                 num <= 9;
                 num += 2) {
                if (boardPtr->is_valid_move(move) && is_valid_number_for_player(num,isMaximizing ? aiPlayer : player)) {
                    // Make the move
                    boardPtr->update_board(move, num);

                    // Recursively call minimax for the next move
                    int currentScore = minimax(depth + 1, !isMaximizing, alpha, beta).first;

                    // Undo the move
                    undo_move(move, num);

                    // Update the best score and best move based on who is maximizing
                    if (isMaximizing) {
                        if (currentScore > bestScore) {
                            bestScore = currentScore;
                            bestMove = move;
                        }
                        alpha = max(alpha, bestScore);
                    } else {
                        if (currentScore < bestScore) {
                            bestScore = currentScore;
                            bestMove = move;
                        }
                        beta = min(beta, bestScore);
                    }

                    // Alpha-beta pruning
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }

        return make_pair(bestScore, bestMove);
    }
    bool is_valid_number_for_player(int number, int playerSymbol) {
        if (usedNumbers.count(number) > 0)
            return false; // Check if number is already used

        if (playerSymbol == 1 && (number < 1 || number > 9 || number % 2 == 0))
            return false; // Player 1 uses odd numbers

        if (playerSymbol == 2 && (number < 1 || number > 9 || number % 2 != 0))
            return false; // Player 2 uses even numbers

        return true;
    }
    int evaluate_board() {
        // Check if Player 2 (AI) or Player 1 (opponent) has won
        if (boardPtr->is_win(2)) return 10;  // AI win
        if (boardPtr->is_win(1)) return -10; // Opponent win
        return 0;  // Draw or ongoing game
    }
    void undo_move(int move, int num) {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        boardPtr->board[row][col] = 0; // Reset cell
        usedNumbers.erase(num);        // Remove number from used list
    }
    bool is_board_full() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (boardPtr->board[i][j] == 0) {
                    return false; // Empty space found
                }
            }
        }
        return true; // No empty spaces
    }
};


void play_vs_player() {
    NumericalTicTacToe_Board board(3, 3);  // Create a 3x3 board for the game
    string player1_name, player2_name;

    // Get player names
    cout << "Enter the name of player 1: ";
    cin >> player1_name;
    cout << "Enter the name of player 2: ";
    cin >> player2_name;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer

    // Create player objects (Player 1 uses odd numbers, Player 2 uses even numbers)
    Player<int> player1(player1_name, 1);  // Odd numbers
    Player<int> player2(player2_name, 2);  // Even numbers

    // Game loop
    while (!board.is_draw()) {  // Continue until the game is a draw
        board.display_board();  // Display current board

        // Player 1's turn
        int move, number;
        do {
            while(true) {
                cout << "Player " << player1.getname() << " (odd numbers) enter move (1-9): ";
                cin >> move;  // Get player 1's move
                if (cin.fail() || move < 1 || move > 9) {
                    cout << "Invalid move. Please enter a number between 1 and 9.\n";
                    cin.clear();                 // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                } else {
                    break;  // Valid input
                }
            }
            while(true) {
                cout << "Now enter your odd number: ";
                cin >> number;  // Get player 1's number
                if (cin.fail() || move < 1 || move > 9) {
                    cout << "Invalid move. Please enter a number between 1 and 9.\n";
                    cin.clear();                 // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                } else {
                    break;  // Valid input
                }
            }

            // Check if the move and number are valid
            if (!board.is_valid_move(move)) {
                cout << "Invalid move. Try again." << endl;
            } else if (!board.is_valid_number_for_player(number, player1.getsymbol())) {
                cout << "Invalid number for Player 1. Please enter an unused odd number between 1 and 9." << endl;
            }
        } while (!board.is_valid_move(move) || !board.is_valid_number_for_player(number, player1.getsymbol()));

        board.update_board(move, number);  // Update the board with player 1's move

        // Check if player 1 wins
        if (board.is_win(player1.getsymbol())) {
            board.display_board();
            cout << player1.getname() << " wins!" << endl;
            return;  // Exit the function if player 1 wins
        }

        // Check if the game is a draw
        if (board.is_draw()) {
            cout << "It's a draw!" << endl;
            return;
        }

        // Player 2's turn
        board.display_board();  // Display the updated board
        do {
            cout << "Player " << player2.getname() << " (even numbers) enter move (1-9): ";
            cin >> move;  // Get player 2's move

            cout << "Now enter your even number: ";
            cin >> number;  // Get player 2's number

            // Check if the move and number are valid
            if (!board.is_valid_move(move)) {
                cout << "Invalid move. Try again." << endl;
            } else if (!board.is_valid_number_for_player(number, player2.getsymbol())) {
                cout << "Invalid number for Player 2. Please enter an unused even number between 1 and 9." << endl;
            }
        } while (!board.is_valid_move(move) || !board.is_valid_number_for_player(number, player2.getsymbol()));

        board.update_board(move, number);  // Update the board with player 2's move

        // Check if player 2 wins
        if (board.is_win(player2.getsymbol())) {
            board.display_board();
            cout << player2.getname() << " wins!" << endl;
            return;  // Exit the function if player 2 wins
        }

        // Check if the game is a draw
        if (board.is_draw()) {
            cout << "It's a draw!" << endl;
            return;
        }
    }
}

void play_with_computer() {
    NumericalTicTacToe_Board board(3, 3);  // Create a 3x3 board for the game
    string player1_name;

    // Create AI players for easy and hard levels
    NumericalTicTacToe_Random_Player randomPlayer(&board, "Random Computer Player", 2);  // AI for easy level
    NumericalTicTacToe_MinMax_Player aiPlayer(&board, "Smart AI Player", 2);  // AI for hard level

    // Get Player 1's name
    cout << "Enter Player 1 name: ";
    cin >> player1_name;
    Player<int> player1(player1_name, 1);  // Player 1: Odd numbers, controlled by human
    bool isPlayerTurn = true;  // Track whose turn it is

    // Choose difficulty level
    string level;
    cout << endl << "Choose difficulty level:" << endl;
    cout << "1. Computer Random Player" << endl;
    cout << "2. AI Smart Player" << endl;
    cout << "(1/2) => ";
    cin >> level;

    if (level == "1") {  // Easy level (Random AI)
        cout << "Player "<< player1.getname() <<" is Odd numbers (1, 3, 5, 7, 9), and "<< randomPlayer.getname() <<" is Even numbers (2, 4, 6, 8)." << endl;

        // Game loop
        while (!board.is_draw() && !board.is_win(1) && !board.is_win(2)) {
            board.display_board();  // Display the current board
            int move, number;
            bool validMove = false;

            if(isPlayerTurn) {  // Player 1's turn
                do {
                    // Get Player 1's move
                    while(true) {
                        cout << "Player " << player1.getname() << " (odd numbers) enter move (1-9): ";
                        cin >> move;
                        if (cin.fail() || move < 1 || move > 9) {
                            cout << "Invalid move. Please enter a number between 1 and 9.\n";
                            cin.clear();                 // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                        } else {
                            break;  // Valid input
                        }
                    }
                    while(true) {
                        cout << "Now enter your odd number: ";
                        cin >> number;
                        if (cin.fail() || move < 1 || move > 9) {
                            cout << "Invalid move. Please enter a number between 1 and 9.\n";
                            cin.clear();                 // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                        } else {
                            break;  // Valid input
                        }
                    }

                    // Validate move and number
                    if (!board.is_valid_move(move)) {
                        cout << "Invalid move. Try again." << endl;
                    } else if (!board.is_valid_number_for_player(number, player1.getsymbol())) {
                        cout << "Invalid number for Player 1. Please enter an unused odd number between 1 and 9." << endl;
                    } else {
                        validMove = true;
                    }
                } while (!validMove);

                board.update_board(move, number);  // Update the board with Player 1's move

                if (board.is_win(player1.getsymbol())) {  // Check for win
                    board.display_board();
                    cout << player1.getname() <<" wins!" << endl;
                    return;
                }

                isPlayerTurn = false;  // Switch to Computer Random Player turn
            } else {  // Computer's (Random Player) turn
                cout << randomPlayer.getname() <<" is calculating its move!" << endl;
                randomPlayer.getRandomMove();  // Get the random move from AI

                if (board.is_win(randomPlayer.getsymbol())) {  // Check if AI wins
                    board.display_board();
                    cout << randomPlayer.getname() <<" wins!" << endl;
                    return;
                }

                isPlayerTurn = true;  // Switch back to Player 1's turn
            }

            if (board.is_draw()) {  // Check if it's a draw
                board.display_board();
                cout << "It's a draw!" << endl;
                return;
            }
        }
    } else {  // Hard level (MinMax AI)
        cout << "Player "<< player1.getname() <<" is Odd numbers (1, 3, 5, 7, 9), and "<< aiPlayer.getname() <<" is Even numbers (2, 4, 6, 8)." << endl;

        // Game loop
        while (!board.is_draw() && !board.is_win(1) && !board.is_win(2)) {
            board.display_board();  // Display the current board

            if (isPlayerTurn) {  // Player 1's turn
                int move, number;
                bool validMove = false;
                do {
                    // Get Player 1's move
                    while(true) {
                        cout << "Player " << player1.getname() << " (odd numbers) enter move (1-9): ";
                        cin >> move;
                        if (cin.fail() || move < 1 || move > 9) {
                            cout << "Invalid move. Please enter a number between 1 and 9.\n";
                            cin.clear();                 // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                        } else {
                            break;  // Valid input
                        }
                    }
                    while(true) {
                        cout << "Now enter your odd number: ";
                        cin >> number;
                        if (cin.fail() || move < 1 || move > 9) {
                            cout << "Invalid move. Please enter a number between 1 and 9.\n";
                            cin.clear();                 // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                        } else {
                            break;  // Valid input
                        }
                    }

                    // Validate move and number
                    if (!board.is_valid_move(move)) {
                        cout << "Invalid move. Try again." << endl;
                    } else if (!board.is_valid_number_for_player(number, player1.getsymbol())) {
                        cout << "Invalid number for Player 1. Please enter an unused odd number between 1 and 9." << endl;
                    } else {
                        validMove = true;
                    }
                } while (!validMove);

                board.update_board(move, number);  // Update the board with Player 1's move

                if (board.is_win(player1.getsymbol())) {  // Check for win
                    board.display_board();
                    cout << player1.getname() <<" wins!" << endl;
                    return;
                }

                isPlayerTurn = false;  // Switch to AI turn
            } else {  // AI's turn
                cout << aiPlayer.getname() <<" is calculating its move!" << endl;

                // AI computes the best move using Minimax
                int bestMove = -1, bestNumber = -1, bestScore = -1000;
                for (int move = 1; move <= 9; ++move) {
                    for (int num = 2; num <= 9; num += 2) {  // Check valid moves for AI (even numbers)
                        if (board.is_valid_move(move) && board.is_valid_number_for_player(num, 2)) {
                            board.update_board(move, num);  // Make the move

                            int moveScore = aiPlayer.minimax(0, false, -1000, 1000).first;  // Minimax evaluation

                            // Update best move if better score found
                            if (moveScore > bestScore) {
                                bestScore = moveScore;
                                bestMove = move;
                                bestNumber = num;
                            }

                            aiPlayer.undo_move(move, num);  // Undo the move
                        }
                    }
                }

                // AI makes the best move
                if (bestMove != -1 && bestNumber != -1) {
                    board.update_board(bestMove, bestNumber);

                    if (board.is_win(2)) {  // Check if AI wins
                        board.display_board();
                        cout << aiPlayer.getname() <<" wins!" << endl;
                        return;
                    }
                }

                isPlayerTurn = true;  // Switch back to Player 1's turn
            }

            if (board.is_draw()) {  // Check if it's a draw
                board.display_board();
                cout << "It's a draw!" << endl;
                return;
            }
        }
    }
}

void play_numerical_tic_tac_toe() {
    int choice;
    // Display menu and get user's choice
    cout << "\nWelcome to Numerical Tic Tac Toe!" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    cout << "(1/2) => ";
    cin >> choice;
    // Start the selected game mode
    if (choice == 1) {
        cout << "Player 1 is Odd numbers (1, 3, 5, 7, 9), and Player 2 is Even numbers (2, 4, 6, 8)." << endl;
        play_vs_player();
    }
    else if (choice == 2) {
        play_with_computer();
    }
    else {
        cout << "Invalid choice. Exiting game." << endl;
    }
}

#endif
