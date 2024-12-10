#ifndef UNTITLED_WORDS_TIC_TAC_TOE_H
#define UNTITLED_WORDS_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <fstream>
#include <set>
#include <string>

using namespace std;

set<string> dictionary;

class WordsTicTacToe_Board : public Board<char> {
public:
    WordsTicTacToe_Board(int i, int i1) : Board<char>(i, i1){
        initializeBoard();
        currentPlayerSymbol = 'X';
        load_dictionary();
    }
    void initializeBoard() override{
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = ' ';
            }
        }
    }
    void load_dictionary() {
        ifstream file("dic.txt");
        string word;
        while (file >> word) {
            transform(word.begin(), word.end(), word.begin(), ::toupper); // Convert word to uppercase
            dictionary.insert(word);
        }
        file.close(); // Ensure the file is properly closed
        cout << "Dictionary loaded with " << dictionary.size() << " words." << endl; // Debugging
    }
    void display_board() override {
        cout << "\n";
        int position = 1;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    cout << " " << position << " ";
                } else {
                    cout << " " << board[i][j] << " ";
                }
                if (j < 2) cout << "|";
                position++;
            }
            cout << "\n";
            if (i < 2) cout << "---|---|---\n";
        }
        cout << "\n";
    }
    bool is_win(char) override {
        // Check rows
        for (int i = 0; i < 3; i++) {
            string rowWord = string(1, board[i][0]) + board[i][1] + board[i][2];
            transform(rowWord.begin(), rowWord.end(), rowWord.begin(), ::toupper);
            if (dictionary.count(rowWord) > 0) return true;
        }

        // Check columns
        for (int i = 0; i < 3; i++) {
            string colWord = string(1, board[0][i]) + board[1][i] + board[2][i];
            transform(colWord.begin(), colWord.end(), colWord.begin(), ::toupper);
            if (dictionary.count(colWord) > 0) return true;
        }

        // Check diagonals
        string diag1 = string(1, board[0][0]) + board[1][1] + board[2][2];
        string diag2 = string(1, board[0][2]) + board[1][1] + board[2][0];
        transform(diag1.begin(), diag1.end(), diag1.begin(), ::toupper);
        transform(diag2.begin(), diag2.end(), diag2.begin(), ::toupper);

        if (dictionary.count(diag1) > 0 || dictionary.count(diag2) > 0) {
            return true;
        }

        return false;
    }
    bool is_valid_move(int move) override {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        return (move >= 1 && move <= 9 && board[row][col] == ' '); // Only check position
    }
    bool is_valid_letter(char letter){
        // Check if the letter is alphabetic and is a single character
        if (!isalpha(letter)) return false;

        return true;
    }
    bool update_board(int move, char letter) override {
        // Convert move (1-9) to row/col and place the letter
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        board[row][col] = letter;
        return true;
    }
    bool is_draw() override {
        // Check if the board is full
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') return false;

        return true;
    }
};

class WordsTicTacToe_Random_player : public Player<char>{
private:
    Board<char>* boardPtr;
public:
    WordsTicTacToe_Random_player(Board<char>* board, char symbol) : Player<char>(symbol), boardPtr(board){
    }
    int computer_random_move(){
        int move;
        char letter;
        // Randomly pick a position and a letter
        do {
            move = rand() % 9 + 1;
            letter = 'A' + (rand() % 26); // Random letter from A-Z
        } while (!boardPtr->is_valid_move(move) && !is_valid_letter(letter));

        boardPtr->update_board(move, letter);
        return move;

    }
    bool is_valid_letter(char letter){
        // Check if the letter is alphabetic and is a single character
        if (!isalpha(letter)) return false;

        return true;
    }
};

class WordsTicTacTOe_MinMax_Player : public Player<char>{
private:
    Board<char>* boardPtr;
public:
    WordsTicTacTOe_MinMax_Player(Board<char>* board, char symbol) : Player<char>(symbol), boardPtr(board) {}

    int score_word_formation(const string& line) {
        if (line.size() < 3) return 0; // No need to check shorter lines
        return dictionary.count(line) > 0 ? 10 : 0;
    }

    int evaluate_board() {
        int score = 0;

        // Check rows and columns for words
        for (int i = 0; i < 3; i++) {
            score += score_word_formation(string(1, boardPtr->board[i][0]) + boardPtr->board[i][1] + boardPtr->board[i][2]);
            score += score_word_formation(string(1, boardPtr->board[0][i]) + boardPtr->board[1][i] + boardPtr->board[2][i]);
        }

        // Check diagonals for words
        score += score_word_formation(string(1, boardPtr->board[0][0]) + boardPtr->board[1][1] + boardPtr->board[2][2]);
        score += score_word_formation(string(1, boardPtr->board[0][2]) + boardPtr->board[1][1] + boardPtr->board[2][0]);

        return score;
    }

    vector<pair<int, char>> generate_valid_moves() {
        vector<pair<int, char>> validMoves;
        for (int move = 1; move <= 9; ++move) {
            int row = (move - 1) / 3;
            int col = (move - 1) % 3;
            if (boardPtr->board[row][col] == ' ') { // Check empty cell
                for (char letter = 'A'; letter <= 'Z'; ++letter) {
                    validMoves.push_back({move, letter});
                }
            }
        }
        return validMoves;
    }

    void undo_move(int move) {
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;
        boardPtr->board[row][col] = ' '; // Reset the board cell
    }

    int minimax(int depth, bool isMaximizing, int alpha, int beta) {
        if (boardPtr->is_win('O')) return isMaximizing ? -10 : 10;
        if (boardPtr->is_win('O') || depth >= 2) return evaluate_board(); // Limit depth to 2

        int bestScore = isMaximizing ? INT_MIN : INT_MAX;
        vector<pair<int, char>> validMoves = generate_valid_moves();

        for (const auto& [move, letter] : validMoves) {
            boardPtr->update_board(move, letter);
            int score = minimax(depth + 1, !isMaximizing, alpha, beta);
            undo_move(move);

            if (isMaximizing) {
                bestScore = max(bestScore, score);
                alpha = max(alpha, bestScore);
            } else {
                bestScore = min(bestScore, score);
                beta = min(beta, bestScore);
            }

            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return bestScore;
    }
    pair<int, char> findBestMove() {
        int bestScore = INT_MIN;
        pair<int, char> bestMove = {-1, ' '};
        vector<pair<int, char>> validMoves = generate_valid_moves();

        // If we decide to search using minimax (hard mode), but limit depth
        if (rand() % 2 == 0) {  // 50% random move
            return validMoves[rand() % validMoves.size()]; // Pick random
        }

        for (const auto& [move, letter] : validMoves) {
            boardPtr->update_board(move, letter);
            int score = minimax(0, false, INT_MIN, INT_MAX);
            undo_move(move);

            if (score > bestScore) {
                bestScore = score;
                bestMove = {move, letter};
            }
        }
        return bestMove;
    }
};

void player_vs_player(){
    WordsTicTacToe_Board board(3,3);
    Player<char> player1('X');
    Player<char> player2('O');

    int move;
    char letter;
    board.display_board();
    while (!board.is_draw()){
        // Player X's turn
        cout << "Player " << player1.getsymbol() << ", it's your turn!\n";
        cout << "Enter the letter you want to place (A-Z): ";
        cin >> letter;
        letter = toupper(letter); // Ensure the letter is uppercase

        cout << "Enter the position (1-9) for your move: ";
        cin >> move;
        if(board.is_valid_move(move) && board.is_valid_letter(letter)){
            board.update_board(move,letter);
            board.display_board();

            if (board.is_win(player1.getsymbol())) {
                board.display_board();
                cout << "Player 1 wins!" << endl;
                return;
            }

            if (board.is_draw()) {
                cout << "It's a draw!" << endl;
                return;
            }
        }else{
            cout << "Invalid move. Please try again." << endl;
        }
        // Player O's turn
        cout << "Player " << player2.getsymbol() << ", it's your turn!\n";
        cout << "Enter the letter you want to place (A-Z): ";
        cin >> letter;
        letter = toupper(letter); // Ensure the letter is uppercase

        cout << "Enter the position (1-9) for your move: ";
        cin >> move;
        if(board.is_valid_move(move) && board.is_valid_letter(letter)){
            board.update_board(move,letter);
            board.display_board();

            if (board.is_win(player1.getsymbol())) {
                board.display_board();
                cout << "Player 2 wins!" << endl;
                return;
            }

            if (board.is_draw()) {
                cout << "It's a draw!" << endl;
                return;
            }
        }else{
            cout << "Invalid move. Please try again." << endl;
        }

    }
}
void player_vs_computer(){
    WordsTicTacToe_Board board(3, 3);
    WordsTicTacToe_Random_player random_player(&board,'O');
    WordsTicTacTOe_MinMax_Player ai_player(&board,'O');
    Player<char> player1('X');  // Player 1: Odd numbers, controlled by human
    bool is_player_turn = true;
    int move;
    char letter;
    string level;
    cout << endl << "Choose difficulty level:" << endl;
    cout << "1. Computer Random Player" << endl;
    cout << "2. AI Smart Player" << endl;
    cout << "(1/2) => ";
    cin >> level;
    if(level == "1"){
        while (!board.is_draw() && !board.is_win('X') && !board.is_win('O')) {
            board.display_board();
            if(is_player_turn) {
                // Player's turn
                cout << "Player " << player1.getsymbol() << ", it's your turn!\n";
                cout << "Enter the letter you want to place (A-Z): ";
                cin >> letter;
                letter = toupper(letter); // Ensure the letter is uppercase

                cout << "Enter the position (1-9) for your move: ";
                cin >> move;
                if (board.is_valid_move(move) && board.is_valid_letter(letter)) {
                    board.update_board(move, letter);

                    if (board.is_win(player1.getsymbol())) {
                        board.display_board();
                        cout << "Player 1 wins!" << endl;
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
            }else {
                // Computer Random Player's turn
                cout << "Computer's turn...\n";
                random_player.computer_random_move();

                if (board.is_win(random_player.getsymbol())) {
                    board.display_board();
                    cout << "Computer Random Player wins!" << endl;
                    return;
                }

                if (board.is_draw()) {
                    cout << "It's a draw!" << endl;
                    return;
                }
                is_player_turn = true;
            }
        }
    }else{
        while (!board.is_draw() && !board.is_win('X') && !board.is_win('O')) {
            board.display_board();
            if(is_player_turn) {
                // Player's turn
                cout << "Player " << player1.getsymbol() << ", it's your turn!\n";
                cout << "Enter the letter you want to place (A-Z): ";
                cin >> letter;
                letter = toupper(letter); // Ensure the letter is uppercase

                cout << "Enter the position (1-9) for your move: ";
                cin >> move;
                if (board.is_valid_move(move) && board.is_valid_letter(letter)) {
                    board.update_board(move, letter);

                    if (board.is_win(player1.getsymbol())) {
                        board.display_board();
                        cout << "Player 1 wins!" << endl;
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
            }else {
                // AI's Turn
                cout << "AI is thinking...\n";
                auto [bestMove, bestLetter] = ai_player.findBestMove();
                board.update_board(bestMove, bestLetter);
                if (board.is_win(ai_player.getsymbol())) {
                    board.display_board();
                    cout << "AI wins!" << endl;
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
    cout << "\nWelcome to Words Tic Tac Toe!" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    cout << "(1/2) => ";
    cin >> choice;

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
