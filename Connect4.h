#ifndef _CONNECT4_H
#define _CONNECT4_H

#include "BoardGame_Classes.h"

class Connect4_Board : public Board<char> {
public:
    Connect4_Board(int i, int i1) : Board<char>(6, 7) {
        initializeBoard();
    }
    void initializeBoard() override {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                this->board[i][j] = '-';
            }
        }
    }

    bool update_board(int x, char symbol) override {
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
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] == currentPlayerSymbol && board[i][j + 1] == currentPlayerSymbol &&
                    board[i][j + 2] == currentPlayerSymbol && board[i][j + 3] == currentPlayerSymbol)
                    return true;
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 7; j++) {
                if (board[i][j] == currentPlayerSymbol && board[i + 1][j] == currentPlayerSymbol &&
                    board[i + 2][j] == currentPlayerSymbol && board[i + 3][j] == currentPlayerSymbol)
                    return true;
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] == currentPlayerSymbol && board[i + 1][j + 1] == currentPlayerSymbol &&
                    board[i + 2][j + 2] == currentPlayerSymbol && board[i + 3][j + 3] == currentPlayerSymbol)
                    return true;
            }
        }

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

    string getname() override{
        return name;
    }

    void undoMove(int column) {
        for (int row = 0; row < 6; row++) {
            if (boardPtr->board[row][column] != '-') {
                boardPtr->board[row][column] = '-';
                break;
            }
        }
    }

    int evaluateBoard() {
        if (boardPtr->is_win('O')) return 1000;
        if (boardPtr->is_win('X')) return -1000;
        return 0;
    }

    int minimax(int depth, bool isMaximizing) {
        int score = evaluateBoard();
        if (score == 1000 || score == -1000 || boardPtr->is_draw() || depth == 0) {
            return score;
        }

        if (isMaximizing) {
            int bestScore = -10000;
            for (int col = 0; col < 7; col++) {
                if (boardPtr->is_valid_move(col)) {
                    boardPtr->update_board(col, 'O');
                    this->symbol = (this->symbol == 'X') ? 'O' : 'X';
                    bestScore = std::max(bestScore, minimax(depth - 1, false));
                    this->symbol = (this->symbol == 'X') ? 'O' : 'X';
                    undoMove(col);
                }
            }
            return bestScore;
        }
        else {
            int bestScore = 10000;
            for (int col = 0; col < 7; col++) {
                if (boardPtr->is_valid_move(col)) {
                    boardPtr->update_board(col, 'X');
                    this->symbol = (this->symbol == 'X') ? 'O' : 'X';
                    bestScore = std::min(bestScore, minimax(depth - 1, true));
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

        for (int col = 0; col < 7; col++) {
            if (boardPtr->is_valid_move(col)) {
                boardPtr->update_board(col, 'O');
                this->symbol = (this->symbol == 'X') ? 'O' : 'X';
                int moveScore = minimax(5, false);
                this->symbol = (this->symbol == 'X') ? 'O' : 'X';
                undoMove(col);

                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestMove = col;
                }
            }
        }
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

    string getname() override{
        return name;
    }

    int getRandomMove() {
        vector<int> availableColumns;
        for (int col = 0; col < 7; col++) {
            if (boardPtr->is_valid_move(col)) {
                availableColumns.push_back(col);
            }
        }

        if (!availableColumns.empty()) {
            int column = availableColumns[rand() % availableColumns.size()];
            return column;
        }

        return -1;
    }
};

void playWithComputer() {
    int column;
    string level;
    string player1_name;
    Connect4_Board board(6, 7);
    cout << "Enter Player X name: ";
    cin >> player1_name;
    Player player(player1_name, 'X');
    Connect4_Random_Player randomPlayer(&board,"Random Computer Player", 'O');
    Connect4_MinMax_Player AiPlayer(&board,"Smart AI Player", 'O');

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

void playvsplayer() {
    Connect4_Board board(6, 7);
    string player1_name, player2_name;
    cout << "Enter the name of player X: ";
    cin >> player1_name;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "Enter the name of player O: ";
    cin >> player2_name;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

void playC4() {
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
