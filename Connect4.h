#ifndef _CONNECT4_H
#define _CONNECT4_H

#include "BoardGame_Classes.h"

class Connect4_Board : public Board<char> {
public:
    Connect4_Board(int i, int i1) : Board<char>(6, 7) {
        initializeBoard();
    }

    void initializeBoard() override {
        //initialize Board cells with -'s
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                this->board[i][j] = '-';
            }
        }
    }

    bool update_board(int x, char symbol) override {
        //update the user input cell with the current symbol
        if (x < 0 || x >= 7) return false;
        for (int row = 5; row >= 0; row--) {
            if (board[row][x] == '-') {
                board[row][x] = symbol;
                return true;
            }
        }
        return false;
    }

    void display_board() override {
        cout << endl;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                cout << this->board[i][j] << ' ';
            }
            cout << endl;
        }
        cout << "1 2 3 4 5 6 7" << endl;
    }

    bool is_win(char currentPlayerSymbol) override {
        // Check horizontally for four in a row
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] == currentPlayerSymbol && board[i][j + 1] == currentPlayerSymbol &&
                    board[i][j + 2] == currentPlayerSymbol && board[i][j + 3] == currentPlayerSymbol)
                    return true;
            }
        }

        // Check vertically for four in a row
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 7; j++) {
                if (board[i][j] == currentPlayerSymbol && board[i + 1][j] == currentPlayerSymbol &&
                    board[i + 2][j] == currentPlayerSymbol && board[i + 3][j] == currentPlayerSymbol)
                    return true;
            }
        }

        // Check diagonally (bottom-left to top-right) for four in a row
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] == currentPlayerSymbol && board[i + 1][j + 1] == currentPlayerSymbol &&
                    board[i + 2][j + 2] == currentPlayerSymbol && board[i + 3][j + 3] == currentPlayerSymbol)
                    return true;
            }
        }

        // Check diagonally (top-left to bottom-right) for four in a row
        for (int i = 3; i < 6; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] == currentPlayerSymbol && board[i - 1][j + 1] == currentPlayerSymbol &&
                    board[i - 2][j + 2] == currentPlayerSymbol && board[i - 3][j + 3] == currentPlayerSymbol)
                    return true;
            }
        }
        return false;
    }

    bool is_draw() override {
        for (int j = 0; j < 7; j++) {
            if (this->board[0][j] == '-') return false;
        }
        return true;
    }

    bool is_valid_move(int col) override {
        return board[0][col] == '-';
    }
};

class Connect4_MinMax_Player : public Player<char> {
private:
    Board<char>* boardPtr;
public:
    Connect4_MinMax_Player(Board<char>* board,string name, char symbol) : Player<char>(name,symbol), boardPtr(board) {
        srand(time(0));
    }

    void undoMove(int column) {
        for (int row = 0; row < 6; row++) {
            // If a piece is found, remove it by setting it to '-'
            if (boardPtr->board[row][column] != '-') {
                boardPtr->board[row][column] = '-';
                break;
            }
        }
    }

    int evaluateBoard() {
        // Check if 'O' (maximizing player) has won
        if (boardPtr->is_win('O')) return 1000;
        // Check if 'X' (minimizing player) has won
        if (boardPtr->is_win('X')) return -1000;
        // Return 0 if no win condition is met
        return 0;
    }

    int minimax(int depth, bool isMaximizing) {

        int score = evaluateBoard();

        // If a terminal state is reached return the score
        if (score == 1000 || score == -1000 || boardPtr->is_draw() || depth == 0) {
            return score;
        }

        if (isMaximizing) {
            // Maximizing player's move (trying to maximize score for 'O')
            int bestScore = -10000;

            for (int col = 0; col < 7; col++) {
                // Check if the column is a valid move
                if (boardPtr->is_valid_move(col)) {
                    // Make a temporary move
                    boardPtr->update_board(col, 'O');

                    this->symbol = (this->symbol == 'X') ? 'O' : 'X';

                    // Recursively call minimax for the minimizing player's turn
                    bestScore = max(bestScore, minimax(depth - 1, false));

                    this->symbol = (this->symbol == 'X') ? 'O' : 'X';
                    undoMove(col);
                }
            }
            return bestScore;
        } else {
            // Minimizing player's move (trying to minimize score for 'X')
            int bestScore = 10000;

            // Iterate over all columns
            for (int col = 0; col < 7; col++) {
                if (boardPtr->is_valid_move(col)) {
                    // Make a temporary move
                    boardPtr->update_board(col, 'X');

                    this->symbol = (this->symbol == 'X') ? 'O' : 'X';

                    bestScore = min(bestScore, minimax(depth - 1, true));

                    // Undo the move and restore the original state
                    this->symbol = (this->symbol == 'X') ? 'O' : 'X';
                    undoMove(col);
                }
            }
            return bestScore;
        }
    }

    int getBestMove() {
        int bestScore = -10000;
        int bestMove = -1;

        // Iterate over all columns on the board
        for (int col = 0; col < 7; col++) {
            // Check if the column is a valid move
            if (boardPtr->is_valid_move(col)) {
                // Make a temporary move
                boardPtr->update_board(col, 'O');

                this->symbol = (this->symbol == 'X') ? 'O' : 'X';

                // Evaluate the move using minimax
                int moveScore = minimax(5, false);

                // Undo the move and restore the original state
                this->symbol = (this->symbol == 'X') ? 'O' : 'X';
                undoMove(col);

                // Update the best move if the score is better
                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestMove = col;
                }
            }
        }

        // Return the column index of the best move found
        return bestMove;
    }
};

class Connect4_Random_Player : public Player<char> {
private:
    Board<char>* boardPtr;
public:
    Connect4_Random_Player(Board<char>* board,string name, char symbol) : Player<char>(name,symbol), boardPtr(board) {
        srand(time(0));
    }
    //check all the available Moves
    int getRandomMove() {
        vector<int> availableColumns;
        for (int col = 0; col < 7; col++) {
            if (boardPtr->is_valid_move(col)) {
                availableColumns.push_back(col);
            }
        }
        //get a random one
        if (!availableColumns.empty()) {
            int column = availableColumns[rand() % availableColumns.size()];
            return column;
        }

        return -1;
    }
};

//=> play with computer modes
void playWithComputer() {
    int column;
    string level;
    string player1_name;
    Connect4_Board board(6, 7);

    cout << "Enter Player X name: ";
    cin >> player1_name;

    Player player(player1_name, 'X');
    Connect4_Random_Player randomPlayer(&board,"Computer", 'O');
    Connect4_MinMax_Player AiPlayer(&board,"AI", 'O');

    cout << endl << "1. Random Computer Player" << endl;
    cout << "2. Smart AI Player" << endl;
    cout << "(1/2)=>";
    cin >> level;

    if (level == "1") {
        while (!board.is_draw()) {
            board.display_board();
            cout << player.getname() << " (X) enter a column (1-7): ";
            cin >> column;

            if (!board.update_board(column - 1, 'X')) {
                cout << "Invalid move. Try again." << endl;
                continue;
            }

            if (board.is_win('X')) {
                board.display_board();
                cout << player.getname() << " wins!" << endl;
                break;
            }

            cout << randomPlayer.getname() << " is making a move..." << endl;
            int position = randomPlayer.getRandomMove();
            board.update_board(position, 'O');
            cout << "Computer chose position: " << position << endl;

            if (board.is_win('O')) {
                board.display_board();
                cout << randomPlayer.getname() <<" wins!" << endl;
                break;
            }
        }
    } else {
        while (!board.is_draw()) {
            board.display_board();
            cout << player.getname() << " enter a column (1-7): ";
            cin >> column;

            if (!board.update_board(column - 1, 'X')) {
                cout << "Invalid move. Try again.\n";
                continue;
            }

            if (board.is_win('X')) {
                board.display_board();
                cout << player.getname() << " wins!" << endl;
                break;
            }

            cout << AiPlayer.getname() <<"'s is making a move..." << endl;
            int bestMove = AiPlayer.getBestMove();
            board.update_board(bestMove, 'O');
            cout << "AI chose position: " << bestMove << endl;

            if (board.is_win('O')) {
                board.display_board();
                cout << AiPlayer.getname() << " wins!" << endl;
                break;
            }
        }
    }
}

//=> play with player mode
void playvsplayer() {
    Connect4_Board board(6, 7);
    string player1_name, player2_name;

    cout << "Enter the name of player X: ";
    cin >> player1_name;

    cout << "Enter the name of player O: ";
    cin >> player2_name;

    Player player1(player1_name, 'X');
    Player player2(player2_name, 'O');

    while (!board.is_draw()) {
        board.display_board();
        int column;
        cout << player1.getname() << " (X) enter a column (1-7): ";
        cin >> column;

        if (!board.update_board(column - 1, player1.getsymbol())) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        if (board.is_win(player1.getsymbol())) {
            board.display_board();
            cout << player1.getname() <<" wins!" << endl;
            return;
        }

        if (board.is_draw()) {
            board.display_board();
            cout << "It's a draw!" << endl;
            break;
        }

        board.display_board();
        cout << player2.getname() << " (O) enter a column (1-7): ";
        cin >> column;

        if (!board.update_board(column - 1, player2.getsymbol())) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        if (board.is_win(player2.getsymbol())) {
            board.display_board();
            cout << player2.getname() <<" wins!" << endl;
            return;
        }

        if (board.is_draw()) {
            board.display_board();
            cout << "It's a draw!" << endl;
            break;
        }
    }
}

//=>play the game
void play_Connect4() {
    int choice;
    cout << "Welcome to Connect 4!" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    cout << "(1/2) => ";
    cin >> choice;

    if (choice == 1) {
        playvsplayer();
    }
    else if (choice == 2) {
        playWithComputer();
    }
    else {
        cout << "Invalid choice. Exiting game." << endl;
    }
}
#endif
