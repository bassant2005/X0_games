#ifndef _PYRAMID_X_O_H
#define _PYRAMID_X_O_H

#include "BoardGame_Classes.h"

class Pyramid_X_O_Board : public Board<char> {
public:
    Pyramid_X_O_Board(int r, int c) : Board<char>(r, c) {
        initializeBoard();
    }

    void initializeBoard() override {
        int number = 1;

        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                this->board[i][j] = ' ';
            }
        }

        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                if ((i == 0 && (j == 0 || j == 1 || j == 3 || j == 4))
                    || (i == 1 && (j == 0 || j == 4))) {
                    continue;
                }
                this->board[i][j] = number + '0';
                number++;
            }
        }
    }

    void display_board() override {
        cout << endl;
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                cout << this->board[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool is_win(char currentPlayerSymbol) override {
        if (this->board[0][2] == currentPlayerSymbol && this->board[1][2] == currentPlayerSymbol && this->board[2][2] == currentPlayerSymbol)
            return true;
        if (this->board[1][1] == currentPlayerSymbol && this->board[1][2] == currentPlayerSymbol && this->board[1][3] == currentPlayerSymbol)
            return true;
        if (this->board[2][0] == currentPlayerSymbol && this->board[2][1] == currentPlayerSymbol && this->board[2][2] == currentPlayerSymbol)
            return true;
        if (this->board[2][1] == currentPlayerSymbol && this->board[2][2] == currentPlayerSymbol && this->board[2][3] == currentPlayerSymbol)
            return true;
        if (this->board[2][2] == currentPlayerSymbol && this->board[2][3] == currentPlayerSymbol && this->board[2][4] == currentPlayerSymbol)
            return true;

        if (this->board[0][2] == currentPlayerSymbol && this->board[1][1] == currentPlayerSymbol && this->board[2][0] == currentPlayerSymbol)
            return true;
        if (this->board[0][2] == currentPlayerSymbol && this->board[1][3] == currentPlayerSymbol && this->board[2][4] == currentPlayerSymbol)
            return true;

        return false;
    }

    bool update_board(int n ,char symbol) override {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
                if (this->board[i][j] == (n + '0')) {
                    this->board[i][j] = symbol;
                    return true;
                }
            }
        }
        return false;
    }

    bool is_draw() override {
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                if (this->board[i][j] != 'X' && this->board[i][j] != 'O' && this->board[i][j] != ' ') {
                    return false;
                }
            }
        }
        return true;
    }
};

class Pyramid_X_O_AI_Player : public Player<char> {
private:
    Board<char>* boardPtr;
public:
    Pyramid_X_O_AI_Player(Board<char>* board,string name, char symbol) : Player(name,symbol), boardPtr(board) {}

    string getname() override{
        return name;
    }

    int evaluateBoard() {
        if (boardPtr->is_win(boardPtr->currentPlayerSymbol)) {
            return (boardPtr->currentPlayerSymbol == 'O') ? 100 : -100;
        }
        return 0;
    }

    int minimax(int depth, bool isMaximizing) {
        int score = evaluateBoard();
        if (score == 100 || score == -100 || boardPtr->is_draw() || depth == 0) {
            return score;
        }

        if (isMaximizing) {
            int bestScore = -1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    if (boardPtr->board[i][j] != 'X' && boardPtr->board[i][j] != 'O' && boardPtr->board[i][j] != ' ') {
                        char temp = boardPtr->board[i][j];
                        boardPtr->board[i][j] = 'O';
                        boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';
                        bestScore = max(bestScore, minimax(depth - 1, false));
                        boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';
                        boardPtr->board[i][j] = temp;
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = 1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    if (boardPtr->board[i][j] != 'X' && boardPtr->board[i][j] != 'O' && boardPtr->board[i][j] != ' ') {
                        char temp = boardPtr->board[i][j];
                        boardPtr->board[i][j] = 'X';
                        boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';
                        bestScore = min(bestScore, minimax(depth - 1, true));
                        boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';
                        boardPtr->board[i][j] = temp;
                    }
                }
            }
            return bestScore;
        }
    }

    int getBestMove() {
        int bestScore = -1000;
        char bestMove = ' ';
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
                if (boardPtr->board[i][j] != 'X' && boardPtr->board[i][j] != 'O' && boardPtr->board[i][j] != ' ') {
                    char temp = boardPtr->board[i][j];
                    boardPtr->board[i][j] = 'O';

                    boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';

                    int moveScore = minimax(5, false);
                    boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';
                    boardPtr->board[i][j] = temp;

                    if (moveScore > bestScore) {
                        bestScore = moveScore;
                        bestMove = temp;
                    }
                }
            }
        }
        return bestMove;
    }
};

class Pyramid_X_O_Random_Player : public Board<char> {
private:
    string name;
public:
    Pyramid_X_O_Random_Player(int r, int c,string name, char i) : Board(r, c) {}

    string getname(){
        return name;
    }

    char getRandomMove() {
        vector<int> availableMoves;
        for (int i = 1; i <= 9; i++) {
            int row = (i - 1) / 5;
            int col = (i - 1) % 5;
            if (this->board[row][col] != 'X' && this->board[row][col] != 'O') {
                availableMoves.push_back(i);
            }
        }

        if (availableMoves.empty()) {
            return -1;
        }

        int randomIndex = rand() % availableMoves.size();
        return availableMoves[randomIndex];
    }
};

void playGameWithComputer() {
    string player1_name;
    cout << "Enter Player X name: ";
    cin >> player1_name;
    Pyramid_X_O_Board board(3, 5);  // 3 rows and 5 columns for pyramid board
    Player player(player1_name, 'X');
    Pyramid_X_O_Random_Player randomPlayer(3, 5,"Random Computer Player", 'O');
    Pyramid_X_O_AI_Player AiPlayer(&board,"Smart AI Player", 'O');


    string level;
    cout << endl << "1. Random Computer Player" << endl;
    cout << "2. Smart AI Player" << endl;
    cout << "(1/2)=>";
    cin >> level;



    if (level == "1") {
        // Easy mode (random moves)
        while (!board.is_draw()) {
            board.display_board();
            int position;
            cout << player.getname() << " (X) enter your position :";
            cin >> position;

            if (!board.update_board(position, player.getsymbol())) {
                cout << "Invalid move. Try again." << endl;
                continue;
            }

            if (board.is_win('X')) {
                board.display_board();
                cout << player.getname() << " wins!" << endl;
                return;
            }

            if(board.is_draw()) {
                board.display_board();
                cout << "It's a draw!" << endl;
                break;
            }

            cout << randomPlayer.getname() <<" is making a move..." << endl;
            position = randomPlayer.getRandomMove();
            board.update_board(position, 'O');

            cout << "Computer chose position: " << position << endl;

            if (board.is_win('O')) {
                cout << randomPlayer.getname() <<" wins!" << endl;
                break;
            }

            if (board.is_draw()) {
                cout << "It's a draw!" << endl;
                break;
            }
        }
    } else {
        // Hard mode (backtracking)
        while (!board.is_draw()) {
            board.display_board();
            int position;
            cout << player.getname() << " (X) enter your position :";
            cin >> position;

            if (!board.update_board(position, player.getsymbol())) {
                cout << "Invalid move. Try again." << endl;
                continue;
            }

            if (board.is_win('X')) {
                board.display_board();
                cout << player.getname() <<" wins!" << endl;
                return;
            }

            if(board.is_draw()) {
                board.display_board();
                cout << "It's a draw!" << endl;
                break;
            }

            cout << AiPlayer.getname() <<" is making a move..." << endl;
            position = AiPlayer.getBestMove() - '0';

            board.update_board(position, 'O');

            if (board.is_win('O')) {
                board.display_board();
                cout << AiPlayer.getname() <<" wins!" << endl;
                return;
            }

            if(board.is_draw()) {
                board.display_board();
                cout << "It's a draw!" << endl;
                break;
            }
        }
    }
}

void PlayWithPlayer() {
    Pyramid_X_O_Board board(3, 5);  // 3 rows and 5 columns for pyramid board
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
        int position;
        cout << player1.getname() << " (X) enter your position :";
        cin >> position;

        if (!board.update_board(position, player1.getsymbol())) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        if (board.is_win(player1.getsymbol())) {
            board.display_board();
            cout << player1.getname() <<" wins!" << endl;
            return;
        }

        if(board.is_draw()) {
            board.display_board();
            cout << "It's a draw!" << endl;
            break;
        }

        board.display_board();
        cout << player2.getname() << " (O) enter your position :";
        cin >> position;

        if (!board.update_board(position, player2.getsymbol())) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        if (board.is_win(player2.getsymbol())) {
            board.display_board();
            cout << player2.getname() <<" wins!" << endl;
            return;
        }

        if(board.is_draw()) {
            board.display_board();
            cout << "It's a draw!" << endl;
            break;
        }
    }
}

void playPY() {
    int choice;
    cout << "Welcome to Pyramid Tic-Tac-Toe!" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    cout << "(1/2) => ";
    cin >> choice;

    if (choice == 1) {
        PlayWithPlayer();
    }
    else if (choice == 2) {
        playGameWithComputer();
    }
    else {
        cout << "Invalid choice. Exiting game." << endl;
    }
}

#endif
