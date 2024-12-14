#ifndef T5_5TICTACTOE_H
#define T5_5TICTACTOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdlib>
using namespace std;

// Specialized 5x5 Board class
class TicTacToe5x5 : public Board<char> {
private:
    int scoreX; // To hold the score for 'X'
    int scoreO; // To hold the score for 'O'

public:
    TicTacToe5x5() : Board(5, 5) {}

    // Initialize the 5x5 board to empty spaces
    void initializeBoard() override {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                board[i][j] = ' ';
            }
        }
    }

    int getScore(char playerSymbol) {
        return (playerSymbol == 'X') ? scoreX : (playerSymbol == 'O') ? scoreO : 0;
    }

    // Display the current state of the 5x5 board
    void display_board() override {
        cout << '\n';
        int num = 1;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == ' ') {
                    cout << setw(2) << num;
                } else {
                    cout << setw(2) << board[i][j];
                }
                if (j < columns - 1) cout << " | ";
                num++;
            }
            cout << endl;
            if (i < rows - 1) cout << "---|----|----|----|----" << endl;
        }
        cout << endl;
    }

    // Check if a move is valid
    bool is_valid_move(int n) override {
        int row = (n - 1) / 5;
        int col = (n - 1) % 5;
        return row >= 0 && row < rows && col >= 0 && col < columns && board[row][col] == ' ';
    }

    // Update board with the player's move
    bool update_board(int n, char symbol) override {
        int row = (n - 1) / 5;
        int col = (n - 1) % 5;
        if (is_valid_move(n)) {
            board[row][col] = symbol;
            return true;
        }
        return false;
    }

    int takecount(int& count) {
        return count;
    }

    // Check for three-in-a-row(win condition)
    bool is_win(char currentPlayerSymbol) override {
        int count = 0;

        // Check horizontal rows for three-in-a-row
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 3; col++) {  // Only check for sequences starting in the first 3 columns
                if (board[row][col] == currentPlayerSymbol && board[row][col] == board[row][col+1] && board[row][col+1] == board[row][col+2]) {
                    count++;

                }
            }
        }

        // Check vertical columns for three-in-a-row
        for (int col = 0; col < 5; col++) {
            for (int row = 0; row < 3; row++) {  // Only check for sequences starting in the first 3 rows
                if (board[row][col] == currentPlayerSymbol && board[row][col] == board[row+1][col] && board[row+1][col] == board[row+2][col]) {
                    count++;
                }
            }
        }

        // Check diagonal (top-left to bottom-right) for three-in-a-row
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == currentPlayerSymbol && board[row][col] == board[row+1][col+1] && board[row+1][col+1] == board[row+2][col+2]) {
                    count++;
                }
            }
        }

        // Check diagonal (top-right to bottom-left) for three-in-a-row
        for (int row = 0; row < 3; row++) {
            for (int col = 2; col < 5; col++) {
                if (board[row][col] == currentPlayerSymbol && board[row][col] == board[row+1][col-1] && board[row+1][col-1] == board[row+2][col-2]) {
                    count++;
                }
            }
        }

        // Update the player's score based on the count
        if (currentPlayerSymbol == 'X') {
            scoreX = count;
        } else if (currentPlayerSymbol == 'O') {
            scoreO = count;
        }

        return count > 0;
    }

    // Check for a draw (no spaces left)
    bool is_draw() override {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == ' ') return false; // Empty space exists
            }
        }
        return true; // No spaces left, hence a draw
    }
};

class TicTacToe5x5_MinMax_Player : public Player<char> {
private:
    TicTacToe5x5* game;

public:
    TicTacToe5x5_MinMax_Player(TicTacToe5x5* board,string name, char symbol) : Player(name,symbol), game(board) {}

    string getname() override{
        return name;
    }

    int evaluateBoard() {
        return game->getScore(getsymbol()) - game->getScore(getsymbol() == 'X' ? 'O' : 'X');
    }

    int minimax(int depth, bool isMaximizing, int alpha, int beta) {
        char currentSymbol = isMaximizing ? getsymbol() : (getsymbol() == 'X' ? 'O' : 'X');

        if (game->is_win(getsymbol())) return 1000 - depth;
        if (game->is_win(currentSymbol)) return depth - 1000;
        if (game->is_draw() || depth <= 0) return evaluateBoard();

        if (isMaximizing) {
            int maxEval = -10000;
            for (int move = 1; move <= 25; ++move) {
                if (game->is_valid_move(move)) {
                    game->update_board(move, getsymbol());
                    int eval = minimax(depth - 1, false, alpha, beta);
                    undoMove(move);
                    maxEval = max(maxEval, eval);
                    alpha = max(alpha, eval);
                    if (beta <= alpha) break;
                }
            }
            return maxEval;
        } else {
            int minEval = 10000;
            for (int move = 1; move <= 25; ++move) {
                if (game->is_valid_move(move)) {
                    char opponentSymbol = getsymbol() == 'X' ? 'O' : 'X';
                    game->update_board(move, opponentSymbol);
                    int eval = minimax(depth - 1, true, alpha, beta);
                    undoMove(move);
                    minEval = min(minEval, eval);
                    beta = min(beta, eval);
                    if (beta <= alpha) break;
                }
            }
            return minEval;
        }
    }

    int getBestMove() {
        int bestMove = -1;
        int bestValue = -10000;
        for (int move = 1; move <= 25; ++move) {
            if (game->is_valid_move(move)) {
                game->update_board(move, getsymbol());
                int moveValue = minimax(5, false, -10000, 10000);
                undoMove(move);
                if (moveValue > bestValue) {
                    bestValue = moveValue;
                    bestMove = move;
                }
            }
        }
        return bestMove;
    }

    void undoMove(int move) {
        int row = (move - 1) / 5;
        int col = (move - 1) % 5;
        game->board[row][col] = ' ';
    }
};


int easyAIMove(TicTacToe5x5& game) {
    srand(time(0)); // Seed the random number generator with the current time
    int move;
    do {
        move = rand() % 25 + 1; // Random move between 1 and 25
    } while (!game.is_valid_move(move));
    return move;
}

bool isGameOver(TicTacToe5x5& game) {
    int count = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (game.board[i][j] == 'X' || game.board[i][j] == 'O') {
                count++;
            }
        }
    }
    if (count < 24) return false;

    return true;
}

void playerVsPlayer(TicTacToe5x5& game, Player<char>& player1, Player<char>& player2) {
    Player<char>* currentPlayer = &player1; // Start with player1

    while (!isGameOver(game)) { // 12 moves per player (24 total moves)
        game.display_board();
        int move;

        cout << currentPlayer->getname() << " (" << currentPlayer->getsymbol() << "), enter a move (1-25): ";
        cin >> move;

        if (game.update_board(move, currentPlayer->getsymbol())) {
            // Check for a win (Update scores inside is_win function)
            game.is_win(player1.getsymbol());
            game.is_win(player2.getsymbol());

            // Switch the current player
            currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
        } else {
            cout << "Invalid move! Try again." << endl;
        }
    }

    game.display_board();

    // Update scores and display
    int counterP1 = game.getScore(player1.getsymbol());
    int counterP2 = game.getScore(player2.getsymbol());
    cout << "Scores:\n" << player1.getname() << " (X) => " << counterP1 << "\n"
         << player2.getname() << " (O) => " << counterP2 << endl;

    if (counterP1 > counterP2) {
        cout << player1.getname() << " wins the game!\n";
    } else if (counterP2 > counterP1) {
        cout << player2.getname() << " wins the game!\n";
    } else {
        cout << "It's a tie...\n";
    }
}

void playerVsComputer(TicTacToe5x5& game, Player<char>& humanPlayer, Player<char>& computerPlayer) {
    char currentPlayer = humanPlayer.getsymbol();

    while (!isGameOver(game)) { // 12 moves per player
        game.display_board();
        if (currentPlayer == humanPlayer.getsymbol()) {
            int move;
            cout << "Your move " << humanPlayer.getname() << " (" << humanPlayer.getsymbol() << "), choose a position (1-25): ";
            cin >> move;

            if (game.update_board(move, humanPlayer.getsymbol())) {
                // Check for a win but continue the game
                game.is_win(humanPlayer.getsymbol());
            }
            else {
                cout << "Invalid move. Try again." << endl;
                continue;
            }
        } else {
            cout << computerPlayer.getname() <<" making a move...\n";
            int moveC = easyAIMove(game);
            if (game.update_board(moveC, computerPlayer.getsymbol())) {
                // Check for a win but continue the game
                game.is_win(computerPlayer.getsymbol());
            }
            cout << computerPlayer.getname() <<" chose position " << moveC << endl;

            // Check for a win but continue the game
            game.is_win(computerPlayer.getsymbol());
        }

        // Swap player turns
        currentPlayer = (currentPlayer == humanPlayer.getsymbol()) ? computerPlayer.getsymbol() : humanPlayer.getsymbol();
    }

    game.display_board();

    // Update scores and display
    int humanScore = game.getScore(humanPlayer.getsymbol());
    int computerScore = game.getScore(computerPlayer.getsymbol());
    cout << "Scores:\n"<< humanPlayer.getname() <<" -> " << humanScore << "\n"<< computerPlayer.getname() <<" -> " << computerScore << endl;

    // Declare the winner
    if (humanScore > computerScore) {
        cout << "Congratulations! "<< humanPlayer.getname() << " won the game." << endl;
    } else if (computerScore > humanScore) {
        cout << computerPlayer.getname() <<" wins this time." << endl;
    } else {
        cout << "It's a tie..." << endl;
    }
}

void playerVsAI(TicTacToe5x5& game, Player<char>& humanPlayer, TicTacToe5x5_MinMax_Player& aiPlayer) {
    char currentPlayer = humanPlayer.getsymbol();

    while (!isGameOver(game)) { // 12 moves per player
        game.display_board();
        if (currentPlayer == humanPlayer.getsymbol()) {
            int move;
            cout << "Your move " << humanPlayer.getname() << " (" << humanPlayer.getsymbol() << "), choose a position (1-25): ";
            cin >> move;

            if (game.update_board(move, humanPlayer.getsymbol())) {
                // Check for a win but continue the game
                game.is_win(humanPlayer.getsymbol());
            }
            else {
                cout << "Invalid move. Try again." << endl;
                continue;
            }
        } else {
            cout << aiPlayer.getname() <<" making a move...\n";
            int moveAI = aiPlayer.getBestMove();
            if (game.update_board(moveAI, aiPlayer.getsymbol())) {
                // Check for a win but continue the game
                game.is_win(aiPlayer.getsymbol());
            }
            cout << "AI chose position " << moveAI << endl;

            // Check for a win but continue the game
            game.is_win(aiPlayer.getsymbol());
        }

        // Swap player turns
        currentPlayer = (currentPlayer == humanPlayer.getsymbol()) ? aiPlayer.getsymbol() : humanPlayer.getsymbol();
    }

    game.display_board();

    // Update scores and display
    int humanScore = game.getScore(humanPlayer.getsymbol());
    int computerScore = game.getScore(aiPlayer.getsymbol());
    cout << "Scores:\n"<< humanPlayer.getname() <<" -> " << humanScore << "\n"<< aiPlayer.getname() <<" -> " << computerScore << endl;
    // Declare the winner
    if (humanScore > computerScore) {
        cout << "Congratulations! "<< humanPlayer.getname() << " won the game." << endl;
    } else if (computerScore > humanScore) {
        cout << aiPlayer.getname() <<" wins this time." << endl;
    } else {
        cout << "It's a tie..." << endl;
    }
}


// Updated Wrapper Function
void playFive() {
    TicTacToe5x5 game;
    game.initializeBoard();
    string player1_name, player2_name;
    string mode;
    cout << "Choose mode:\n1. Player vs Player\n2. Player vs Random Computer Player\n3. Player vs Smart AI Player\n=> ";
    cin >> mode;

    if (mode == "1") {
        cout << "Enter the name of player X: ";
        cin >> player1_name;
        cout << "Enter the name of player O: ";
        cin >> player2_name;
        Player<char> player1(player1_name,'X');
        Player<char> player2(player2_name,'O');
        playerVsPlayer(game, player1, player2);
    } else if (mode == "2") {
        cout << "Enter the name of player X: ";
        cin >> player1_name;
        Player<char> player1(player1_name,'X');
        Player<char> player2("Random Computer player",'O');
        playerVsComputer(game, player1, player2);
    } else if (mode == "3") {
        cout << "Enter the name of player X: ";
        cin >> player1_name;
        Player<char> player1(player1_name,'X');
        TicTacToe5x5_MinMax_Player aiPlayer(&game,"Smart AI Player", 'O');
        playerVsAI(game, player1, aiPlayer);
    } else {
        cout << "Invalid mode selected!" << endl;
    }
    cout << endl;
}

#endif // T5_5TICTACTOE_H
