#ifndef _PYRAMID_X_O_H
#define _PYRAMID_X_O_H

#include "BoardGame_Classes.h"

//main board logic
class Pyramid_X_O_Board : public Board<char> {
public:
    Pyramid_X_O_Board(int r, int c) : Board<char>(r, c) {
        initializeBoard();
    }

    void initializeBoard() override {
        int number = 1;
        //make all the cells empty
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                this->board[i][j] = ' ';
            }
        }
        //update the main cells only to be numbers
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
    //=>display the board content
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
        //check column
        if (this->board[0][2] == currentPlayerSymbol && this->board[1][2] == currentPlayerSymbol && this->board[2][2] == currentPlayerSymbol)
            return true;
        //check rows
        if (this->board[1][1] == currentPlayerSymbol && this->board[1][2] == currentPlayerSymbol && this->board[1][3] == currentPlayerSymbol)
            return true;
        if (this->board[2][0] == currentPlayerSymbol && this->board[2][1] == currentPlayerSymbol && this->board[2][2] == currentPlayerSymbol)
            return true;
        if (this->board[2][1] == currentPlayerSymbol && this->board[2][2] == currentPlayerSymbol && this->board[2][3] == currentPlayerSymbol)
            return true;
        if (this->board[2][2] == currentPlayerSymbol && this->board[2][3] == currentPlayerSymbol && this->board[2][4] == currentPlayerSymbol)
            return true;
        //check diagonals
        if (this->board[0][2] == currentPlayerSymbol && this->board[1][1] == currentPlayerSymbol && this->board[2][0] == currentPlayerSymbol)
            return true;
        if (this->board[0][2] == currentPlayerSymbol && this->board[1][3] == currentPlayerSymbol && this->board[2][4] == currentPlayerSymbol)
            return true;

        return false;
    }
    //=>update after ever move
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
    //check if the board is full
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

//=>back tracking logic for the ai player
class Pyramid_X_O_AI_Player : public Player<char> {
private:
    Board<char>* boardPtr;
public:
    Pyramid_X_O_AI_Player(Board<char>* board,string name, char symbol) : Player(name,symbol), boardPtr(board) {}

    int evaluateBoard() {
        // Check if the current player has won the game
        if (boardPtr->is_win(boardPtr->currentPlayerSymbol)) {
            return (boardPtr->currentPlayerSymbol == 'O') ? 100 : -100;
        }
        // Return 0 if no win condition is met
        return 0;
    }

    int minimax(int depth, bool isMaximizing) {
e
        int score = evaluateBoard();

        // If a terminal state is reached, return the score
        if (score == 100 || score == -100 || boardPtr->is_draw() || depth == 0) {
            return score;
        }

        if (isMaximizing) {
            // Maximizer's move (trying to maximize score for 'O')
            int bestScore = -1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    if (boardPtr->board[i][j] != 'X' && boardPtr->board[i][j] != 'O' && boardPtr->board[i][j] != ' ') {
                        // Make a temporary move
                        char temp = boardPtr->board[i][j];
                        boardPtr->board[i][j] = 'O';

                        boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';

                        bestScore = max(bestScore, minimax(depth - 1, false));

                        // Undo the move and restore the original state
                        boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';
                        boardPtr->board[i][j] = temp;
                    }
                }
            }
            return bestScore;
        }
        else {
            // Minimizer's move (trying to minimize score for 'X')
            int bestScore = 1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    if (boardPtr->board[i][j] != 'X' && boardPtr->board[i][j] != 'O' && boardPtr->board[i][j] != ' ') {
                        // Make a temporary move
                        char temp = boardPtr->board[i][j];
                        boardPtr->board[i][j] = 'X';

                        boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';
                        bestScore = min(bestScore, minimax(depth - 1, true));

                        // Undo the move and restore the original state
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

        // Iterate over all cells on the board
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
                if (boardPtr->board[i][j] != 'X' && boardPtr->board[i][j] != 'O' && boardPtr->board[i][j] != ' ') {
                    // Make a temporary move
                    char temp = boardPtr->board[i][j];
                    boardPtr->board[i][j] = 'O';

                    boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';

                    int moveScore = minimax(5, false);

                    // Undo the move
                    boardPtr->currentPlayerSymbol = (boardPtr->currentPlayerSymbol == 'X') ? 'O' : 'X';
                    boardPtr->board[i][j] = temp;

                    // Update the best move if the score is better
                    if (moveScore > bestScore) {
                        bestScore = moveScore;
                        bestMove = temp;
                    }
                }
            }
        }
        // Return the best move found
        return bestMove;
    }
};

class Pyramid_X_O_Random_Player : public Board<char> {
private:
    string name;
public:
    Pyramid_X_O_Random_Player(int r, int c,string name, char i) : Board(r, c) {}

    char getRandomMove() {
        vector<int> availableMoves;

        //check all the available Moves
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
        //get a random one
        int randomIndex = rand() % availableMoves.size();
        return availableMoves[randomIndex];
    }
};

//=> play with computer modes
void playGameWithComputer() {

    string player1_name;
    cout << "Enter Player X name: ";
    cin >> player1_name;
    Pyramid_X_O_Board board(3, 5);  // 3 rows and 5 columns for pyramid board
    Player player(player1_name, 'X');
    Pyramid_X_O_Random_Player randomPlayer(3, 5,"Random", 'O');
    Pyramid_X_O_AI_Player AiPlayer(&board,"AI", 'O');


    string level;
    cout << endl << "1. Random Computer Player" << endl;
    cout << "2. Smart AI Player" << endl;
    cout << "(1/2)=>";
    cin >> level;



    if (level == "1") {

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

            cout <<"computer is making a move..." << endl;
            position = randomPlayer.getRandomMove();
            board.update_board(position, 'O');

            cout << "Computer chose position: " << position << endl;

            if (board.is_win('O')) {
                cout << " computer wins!" << endl;
                break;
            }

            if (board.is_draw()) {
                cout << "It's a draw!" << endl;
                break;
            }
        }
    }
    else {

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

//=> play with player mode
void PlayWithPlayer() {
    Pyramid_X_O_Board board(3, 5);
    string player1_name, player2_name;
    cout << "Enter the name of player X: ";
    cin >> player1_name;

    cout << "Enter the name of player O: ";
    cin >> player2_name;

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

//=>play the game
void play_Pyramid_tictactoe() {
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
