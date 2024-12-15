#ifndef UNTITLED_WORDS_TIC_TAC_TOE_H
#define UNTITLED_WORDS_TIC_TAC_TOE_H
#include "BoardGame_Classes.h"
#include <iostream>
#include <fstream>
#include <set>
#include <string>

using namespace std;

set<string> dictionary;  // Dictionary of valid words

// Class representing the board for Words Tic Tac Toe
class WordsTicTacToe_Board : public Board<char> {
public:
    // Constructor to initialize the board with size and load dictionary
    WordsTicTacToe_Board(int i, int i1) : Board<char>(i, i1) {
        initializeBoard();   // Initialize the game board
        currentPlayerSymbol = 'X';  // Set the starting player symbol to 'X'
        load_dictionary();  // Load the dictionary from the file
    }

    // Initialize the board to be empty (all spaces)
    void initializeBoard() override {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = ' ';  // Set all positions to empty
            }
        }
    }

    // Load dictionary words from a file
    void load_dictionary() {
        ifstream file("dic.txt");  // Open the dictionary file
        string word;
        while (file >> word) {
            transform(word.begin(), word.end(), word.begin(), ::toupper);  // Convert word to uppercase
            dictionary.insert(word);  // Add word to dictionary set
        }
        file.close();  // Close the file after reading
        cout << "Dictionary loaded with " << dictionary.size() << " words." << endl;  // Debugging output
    }

    // Display the current state of the board
    void display_board() override {
        cout << "\n";
        int position = 1;  // To show the position numbers

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    cout << " " << position << " ";  // If empty, show position
                } else {
                    cout << " " << board[i][j] << " ";  // Otherwise show the symbol
                }
                if (j < 2) cout << "|";  // Print '|' to separate columns
                position++;  // Increment position number
            }
            cout << "\n";
            if (i < 2) cout << "---|---|---\n";  // Print horizontal separator
        }
        cout << "\n";
    }

    // Check if the current player has won the game by forming a valid word
    bool is_win(char) override {
        // Check rows for word formation
        for (int i = 0; i < 3; i++) {
            string rowWord = string(1, board[i][0]) + board[i][1] + board[i][2];
            transform(rowWord.begin(), rowWord.end(), rowWord.begin(), ::toupper);
            if (dictionary.count(rowWord) > 0) return true;  // If word found in dictionary, return true
        }

        // Check columns for word formation
        for (int i = 0; i < 3; i++) {
            string colWord = string(1, board[0][i]) + board[1][i] + board[2][i];
            transform(colWord.begin(), colWord.end(), colWord.begin(), ::toupper);
            if (dictionary.count(colWord) > 0) return true;
        }

        // Check diagonals for word formation
        string diag1 = string(1, board[0][0]) + board[1][1] + board[2][2];
        string diag2 = string(1, board[0][2]) + board[1][1] + board[2][0];
        transform(diag1.begin(), diag1.end(), diag1.begin(), ::toupper);
        transform(diag2.begin(), diag2.end(), diag2.begin(), ::toupper);

        if (dictionary.count(diag1) > 0 || dictionary.count(diag2) > 0) {
            return true;
        }

        return false;  // No valid word formed
    }

    // Check if a move is valid
    bool is_valid_move(int move) override {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        return (move >= 1 && move <= 9 && board[row][col] == ' ');  // Only valid if position is empty
    }

    // Validate that the letter is an alphabetic character
    bool is_valid_letter(char letter) {
        if (!isalpha(letter)) return false;  // Letter must be alphabetic
        return true;
    }

    // Update the board with the chosen move and letter
    bool update_board(int move, char letter) override {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        board[row][col] = letter;  // Place letter at the correct position
        return true;
    }

    // Check if the game is a draw (no empty spaces left)
    bool is_draw() override {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') return false;  // Found empty space, not a draw
        return true;  // No empty spaces left, it's a draw
    }
};

// Class for a random computer player, inheriting from Player
class WordsTicTacToe_Random_player : public Player<char> {
private:
    Board<char>* boardPtr;  // Pointer to the board
public:
    WordsTicTacToe_Random_player(Board<char>* board, string name, char symbol) : Player<char>(name, symbol), boardPtr(board) {}

    string getname() override {
        return name;  // Return player's name
    }

    // Generate a random move and letter for the computer
    int computer_random_move() {
        int move;
        char letter;
        do {
            move = rand() % 9 + 1;  // Random move from 1 to 9
            letter = 'A' + (rand() % 26);  // Random letter from A-Z
        } while (!boardPtr->is_valid_move(move) || !is_valid_letter(letter));  // Ensure it's a valid move

        boardPtr->update_board(move, letter);  // Apply the move
        return move;  // Return the chosen move
    }

    // Validate that the letter is alphabetic
    bool is_valid_letter(char letter) {
        if (!isalpha(letter)) return false;  // Letter must be alphabetic
        return true;
    }
};

// Class for a MinMax AI player
class WordsTicTacTOe_MinMax_Player : public Player<char> {
private:
    Board<char>* boardPtr;  // Pointer to the game board

public:
    // Constructor to initialize player with board, name, and symbol
    WordsTicTacTOe_MinMax_Player(Board<char>* board, string name, char symbol) : Player<char>(name, symbol), boardPtr(board) {}

    // Override to return the player's name
    string getname() override {
        return name;
    }

    // Check if a word formed in a line (row, column, or diagonal) is valid and assign score
    int score_word_formation(const string& line) {
        if (line.size() < 3) return 0;  // Skip lines shorter than 3 characters
        return dictionary.count(line) > 0 ? 10 : 0;  // Return 10 if valid word, else 0
    }

    // Evaluate the current board by checking rows, columns, and diagonals for valid words
    int evaluate_board() {
        int score = 0;

        // Check rows and columns for valid words
        for (int i = 0; i < 3; i++) {
            score += score_word_formation(string(1, boardPtr->board[i][0]) + boardPtr->board[i][1] + boardPtr->board[i][2]);
            score += score_word_formation(string(1, boardPtr->board[0][i]) + boardPtr->board[1][i] + boardPtr->board[2][i]);
        }

        // Check diagonals for valid words
        score += score_word_formation(string(1, boardPtr->board[0][0]) + boardPtr->board[1][1] + boardPtr->board[2][2]);
        score += score_word_formation(string(1, boardPtr->board[0][2]) + boardPtr->board[1][1] + boardPtr->board[2][0]);

        return score;  // Return the total score based on valid words
    }

    // Generate a list of all valid moves (empty board spots with letters A-Z)
    vector<pair<int, char>> generate_valid_moves() {
        vector<pair<int, char>> validMoves;
        for (int move = 1; move <= 9; ++move) {
            int row = (move - 1) / 3;
            int col = (move - 1) % 3;
            if (boardPtr->board[row][col] == ' ') {  // Check if the cell is empty
                for (char letter = 'A'; letter <= 'Z'; ++letter) {
                    validMoves.push_back({move, letter});  // Add all possible letter moves
                }
            }
        }
        return validMoves;  // Return the list of valid moves
    }

    // Undo a move by resetting the board cell
    void undo_move(int move) {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        boardPtr->board[row][col] = ' ';  // Reset the board cell to empty
    }

    // Minimax algorithm to evaluate the best move with alpha-beta pruning
    int minimax(int depth, bool isMaximizing, int alpha, int beta) {
        if (boardPtr->is_win('O')) return isMaximizing ? -10 : 10;  // Check for win condition
        if (depth >= 2) return evaluate_board();  // Stop if depth exceeds limit

        int bestScore = isMaximizing ? INT_MIN : INT_MAX;
        vector<pair<int, char>> validMoves = generate_valid_moves();

        // Iterate over each valid move
        for (const auto& [move, letter] : validMoves) {
            boardPtr->update_board(move, letter);  // Make the move
            int score = minimax(depth + 1, !isMaximizing, alpha, beta);  // Recursively call minimax
            undo_move(move);  // Undo the move

            if (isMaximizing) {
                bestScore = max(bestScore, score);
                alpha = max(alpha, bestScore);  // Alpha-beta pruning
            } else {
                bestScore = min(bestScore, score);
                beta = min(beta, bestScore);  // Alpha-beta pruning
            }

            if (beta <= alpha) break;  // Pruning if the search space is exhausted
        }
        return bestScore;  // Return the best score for the current board state
    }

    // Find the best move for the AI by evaluating all valid moves
    pair<int, char> findBestMove() {
        int bestScore = INT_MIN;
        pair<int, char> bestMove = {-1, ' '};  // Initialize the best move
        vector<pair<int, char>> validMoves = generate_valid_moves();

        // Randomly choose a move 50% of the time
        if (rand() % 2 == 0) {
            return validMoves[rand() % validMoves.size()];  // Pick a random valid move
        }

        // Evaluate all valid moves using minimax to find the best move
        for (const auto& [move, letter] : validMoves) {
            boardPtr->update_board(move, letter);
            int score = minimax(0, false, INT_MIN, INT_MAX);  // Use minimax for evaluation
            undo_move(move);

            if (score > bestScore) {
                bestScore = score;
                bestMove = {move, letter};  // Update the best move
            }
        }
        return bestMove;  // Return the best move found
    }
};

void player_vs_player() {
    // Initialize the game board
    WordsTicTacToe_Board board(3, 3);

    string player1_name, player2_name;

    // Get player names
    cout << "Enter the name of player 1: ";
    cin >> player1_name;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "Enter the name of player 2: ";
    cin >> player2_name;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Create player objects with symbols 'X' and 'O'
    Player<char> player1(player1_name, 'X');
    Player<char> player2(player2_name, 'O');

    int move;
    char letter;

    // Display the initial game board
    board.display_board();

    // Game loop: continues until there is a winner or a draw
    while (!board.is_draw()) {

        // Player X's turn
        while (true) {  // Loop to ensure valid input
            cout << "Player " << player1.getname() << ", it's your turn!\n";
            cout << "Enter the letter you want to place (A-Z): ";
            cin >> letter;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Enter the position (1-9) for your move: ";
            cin >> move;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            letter = toupper(letter); // Ensure letter is uppercase

            // Check if the move is valid
            if (board.is_valid_move(move) && board.is_valid_letter(letter)) {
                board.update_board(move, letter);  // Update the board with the move
                board.display_board();  // Display the updated board

                // Check if Player X has won
                if (board.is_win(player1.getsymbol())) {
                    cout << "Player " << player1.getname() << " wins!" << endl;
                    return;  // End the game if Player X wins
                }
                break;  // Exit the loop for Player X's turn
            } else {
                cout << "Invalid move or letter. Please try again." << endl;
            }
        }

        // Check for a draw before Player O's turn
        if (board.is_draw()) {
            cout << "It's a draw!" << endl;
            return;  // End the game if it's a draw
        }

        // Player O's turn (similar to Player X's turn)
        while (true) {  // Loop to ensure valid input
            cout << "Player " << player2.getname() << ", it's your turn!\n";
            cout << "Enter the letter you want to place (A-Z): ";
            cin >> letter;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            letter = toupper(letter); // Ensure letter is uppercase
            cout << "Enter the position (1-9) for your move: ";
            cin >> move;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Check if the move is valid
            if (board.is_valid_move(move) && board.is_valid_letter(letter)) {
                board.update_board(move, letter);  // Update the board with the move
                board.display_board();  // Display the updated board

                // Check if Player O has won
                if (board.is_win(player2.getsymbol())) {
                    cout << "Player " << player2.getname() << " wins!" << endl;
                    return;  // End the game if Player O wins
                }
                break;  // Exit the loop for Player O's turn
            } else {
                cout << "Invalid move or letter. Please try again." << endl;
            }
        }
    }

    // Print draw message if the game ends in a draw
    cout << "It's a draw!" << endl;
}

void player_vs_computer() {
    // Initialize the game board and players
    WordsTicTacToe_Board board(3, 3);
    string player1_name;
    WordsTicTacToe_Random_player random_player(&board, "Random Computer Player", 'O');
    WordsTicTacTOe_MinMax_Player ai_player(&board, "Smart AI Player", 'O');

    // Get Player 1's name
    cout << "Enter Player 1 name: ";
    cin >> player1_name;
    Player<char> player1(player1_name, 'X');  // Player 1: X, controlled by human

    bool is_player_turn = true;
    int move;
    char letter;
    string level;

    // Choose difficulty level
    cout << endl << "Choose difficulty level:" << endl;
    cout << "1. Random Computer Player" << endl;
    cout << "2. Smart AI Player" << endl;
    cout << "(1/2) => ";
    cin >> level;

    // If Random Computer Player is selected
    if (level == "1") {
        while (!board.is_draw() && !board.is_win('X') && !board.is_win('O')) {
            board.display_board();

            if (is_player_turn) {
                // Player's turn
                cout << "Player " << player1.getname() << ", it's your turn!\n";
                cout << "Enter the letter you want to place (A-Z): ";
                cin >> letter;
                letter = toupper(letter); // Ensure the letter is uppercase

                cout << "Enter the position (1-9) for your move: ";
                cin >> move;

                // Validate and update the board
                if (board.is_valid_move(move) && board.is_valid_letter(letter)) {
                    board.update_board(move, letter);

                    // Check for win or draw after Player's move
                    if (board.is_win(player1.getsymbol())) {
                        board.display_board();
                        cout << player1.getname() << " wins!" << endl;
                        return;
                    }

                    if (board.is_draw()) {
                        cout << "It's a draw!" << endl;
                        return;
                    }
                } else {
                    cout << "Invalid move. Please try again." << endl;
                }
                is_player_turn = false;
            } else {
                // Computer Random Player's turn
                cout << random_player.getname() << "'s turn...\n";
                random_player.computer_random_move(); // Computer makes a random move

                // Check for win or draw after Computer's move
                if (board.is_win(random_player.getsymbol())) {
                    board.display_board();
                    cout << random_player.getname() << " wins!" << endl;
                    return;
                }

                if (board.is_draw()) {
                    cout << "It's a draw!" << endl;
                    return;
                }
                is_player_turn = true;
            }
        }
    } else {
        // If Smart AI Player is selected
        while (!board.is_draw() && !board.is_win('X') && !board.is_win('O')) {
            board.display_board();

            if (is_player_turn) {
                // Player's turn
                cout << "Player " << player1.getname() << ", it's your turn!\n";
                cout << "Enter the letter you want to place (A-Z): ";
                cin >> letter;
                letter = toupper(letter); // Ensure the letter is uppercase

                cout << "Enter the position (1-9) for your move: ";
                cin >> move;

                // Validate and update the board
                if (board.is_valid_move(move) && board.is_valid_letter(letter)) {
                    board.update_board(move, letter);

                    // Check for win or draw after Player's move
                    if (board.is_win(player1.getsymbol())) {
                        board.display_board();
                        cout << player1.getname() << " wins!" << endl;
                        return;
                    }

                    if (board.is_draw()) {
                        cout << "It's a draw!" << endl;
                        return;
                    }
                } else {
                    cout << "Invalid move. Please try again." << endl;
                }
                is_player_turn = false;
            } else {
                // AI's turn (Smart AI Player)
                cout << ai_player.getname() << " is thinking...\n";
                auto [bestMove, bestLetter] = ai_player.findBestMove();  // AI finds best move
                board.update_board(bestMove, bestLetter);

                // Check for win or draw after AI's move
                if (board.is_win(ai_player.getsymbol())) {
                    board.display_board();
                    cout << ai_player.getname() << " wins!" << endl;
                    return;
                }

                if (board.is_draw()) {
                    cout << "It's a draw!" << endl;
                    return;
                }
                is_player_turn = true;
            }
        }
    }
}

void play_Words_tic_tac_toe() {
    int choice;
    // Display menu and get user's choice
    cout << "\nWelcome to Words Tic Tac Toe!" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    cout << "(1/2) => ";
    cin >> choice;
    // Start the selected game mode
    if (choice == 1) {
        player_vs_player();
    }
    else if (choice == 2) {
        player_vs_computer();
    }
    else {
        cout << "Invalid choice. Exiting game." << endl;
    }
}

#endif
