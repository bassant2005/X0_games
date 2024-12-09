#ifndef ULTIMATETICTACTOE_H
#define ULTIMATETICTACTOE_H

#include "BoardGame_Classes.h"

class ultemate_X_O_Board : public Board<char> {
public:
    ultemate_X_O_Board(int r, int c) : Board<char>(r, c) {
        initializeBoard();
        currentPlayerSymbol = 'X';
    }

    bool gameOver = false;
    char mainboard[3][3];
    char board[9][9];

    void initializeBoard() override {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                mainboard[i][j] = '*';
            }
        }

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j] = '*';
            }
        }
    }

    void display_board() override {
        cout << "\nUltimate Tic Tac Toe Board:\n";

        // Display the Small Boards (9 small 3x3 grids)
        for (int bigRow = 0; bigRow < 3; bigRow++) {
            for (int smallRow = 0; smallRow < 3; smallRow++) {
                for (int bigCol = 0; bigCol < 3; bigCol++) {
                    for (int smallCol = 0; smallCol < 3; smallCol++) {
                        int globalRow = bigRow * 3 + smallRow;
                        int globalCol = bigCol * 3 + smallCol;
                        char cell = board[globalRow][globalCol];

                        // Print the cell, replace '*' with ' ' for better visual
                        cout << (cell == ' ' ? '*' : cell);
                        if (smallCol < 2) cout << " : ";  // Use colon (:) separator
                    }
                    if (bigCol < 2) cout << "  ||  ";  // Separate 3x3 blocks with "||"
                }
                cout << "\n";
            }
            if (bigRow < 2) {
                cout << string(15, '=') << string(9, '=') << string(15, '=') << "\n";
            }
        }
        cout << "\n";
    }

    void displayBoard() {
        cout << "\nMain Board states:\n\n";
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << " " << mainboard[i][j] << " ";
                if (j < 2) cout << "|";
            }
            cout << "\n";
            if (i < 2) cout << "---|---|---\n";
        }
    }

    bool checkWin(char board[3][3]) {
        for (int i = 0; i < 3; ++i) {
            if ((board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '*') ||
                (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '*')) {
                return true;
            }
        }

        if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '*') ||
            (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '*')) {
            return true;
        }
        return false;
    }

    bool is_win(char currentPlayerSymbol) override {
        for (int i = 0; i < 3; ++i) {
            if (mainboard[i][0] != '*' && mainboard[i][0] == mainboard[i][1] && mainboard[i][1] == mainboard[i][2]) {
                return true;
            }
        }

        // Check all columns for three consecutive
        for (int i = 0; i < 3; ++i) {
            if (mainboard[0][i] != '*' && mainboard[0][i] == mainboard[1][i] && mainboard[1][i] == mainboard[2][i]) {
                return true;
            }
        }

        // Check the two diagonals for three consecutive
        if (mainboard[0][0] != '*' && mainboard[0][0] == mainboard[1][1] && mainboard[1][1] == mainboard[2][2]) {
            return true;
        }
        if (mainboard[0][2] != '*' && mainboard[0][2] == mainboard[1][1] && mainboard[1][1] == mainboard[2][0]) {
            return true;
        }

        return false;
    }

    bool is_valid_move(int largeRow, int largeCol, int smallRow, int smallCol) {
        // Check if the sub-board is already claimed
        if (mainboard[largeRow][largeCol] != '*') {
            return false;
        }

        // Validate cell in the specific sub-board
        int globalRow = largeRow * 3 + smallRow;
        int globalCol = largeCol * 3 + smallCol;
        return board[globalRow][globalCol] == '*';
    }


    bool is_draw() override {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (mainboard[i][j] == '*') return false;
            }
        }
        return !is_win(' ');
    }

    bool checkDraw(char board[3][3]) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == '*') {
                    return false;
                }
            }
        }
        return true;
    }
};

class ultemate_X_O_Random_Player : public ultemate_X_O_Board {
public:
    ultemate_X_O_Random_Player(int r1, int c1) : ultemate_X_O_Board(r1, c1) {
        srand(time(0));
    }

    void getRandomMove(char sharedBoard[9][9], char sharedMainboard[3][3], char currentPlayerSymbol) {
        int largeRow, largeCol, smallRow, smallCol;

        while (true) {
            largeRow = rand() % 3;
            largeCol = rand() % 3;
            smallRow = rand() % 3;
            smallCol = rand() % 3;

            if (sharedMainboard[largeRow][largeCol] == '*') {
                int globalRow = largeRow * 3 + smallRow;
                int globalCol = largeCol * 3 + smallCol;
                if (sharedBoard[globalRow][globalCol] == '*') {
                    sharedBoard[globalRow][globalCol] = currentPlayerSymbol;
                    break;
                }
            }
        }

        // Check sub-board for win/draw and update mainboard
        char subBoard[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                subBoard[i][j] = sharedBoard[largeRow * 3 + i][largeCol * 3 + j];
            }
        }

        if (checkWin(subBoard)) {
            sharedMainboard[largeRow][largeCol] = currentPlayerSymbol;
        } else if (checkDraw(subBoard)) {
            sharedMainboard[largeRow][largeCol] = 'D';
        }
    }
};

void PWithC() {
    ultemate_X_O_Board board(3, 3);
    ultemate_X_O_Random_Player randomPlayer(3, 3);

    int largePosition, smallPosition;
    int largeRow, largeCol, smallRow, smallCol;

    while (!board.gameOver) {
        board.display_board();

        // Player's turn
        cout << "Player X's turn.\n";
        cout << "Enter the large board position (1-9): ";
        cin >> largePosition;

        largeRow = (largePosition - 1) / 3;
        largeCol = (largePosition - 1) % 3;

        cout << "Enter the position (1-9) in the small board: ";
        cin >> smallPosition;

        smallRow = (smallPosition - 1) / 3;
        smallCol = (smallPosition - 1) % 3;

        if (!board.is_valid_move(largeRow, largeCol, smallRow, smallCol)) {
            cout << "Invalid move. Try again.\n";
            continue;
        }

        int globalRow = largeRow * 3 + smallRow;
        int globalCol = largeCol * 3 + smallCol;
        board.board[globalRow][globalCol] = 'X';

        // Check for a win or draw in the sub-board
        char subBoard[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                subBoard[i][j] = board.board[largeRow * 3 + i][largeCol * 3 + j];
            }
        }

        if (board.checkWin(subBoard)) {
            board.mainboard[largeRow][largeCol] = 'X';
        } else if (board.checkDraw(subBoard)) {
            board.mainboard[largeRow][largeCol] = 'D';
        }

        if (board.is_win('X')) {
            board.displayBoard();
            cout << "Player X wins the game!\n";
            break;
        }

        if (board.is_draw()) {
            board.displayBoard();
            cout << "The game is a draw!\n";
            break;
        }

        // Computer's turn
        board.currentPlayerSymbol = (board.currentPlayerSymbol == 'X') ? 'O' : 'X';
        randomPlayer.getRandomMove(board.board, board.mainboard, board.currentPlayerSymbol);

        if (board.is_win('O')) {
            cout << "Player O wins the game!\n";
            break;
        }

        if (board.is_draw()) {
            cout << "The game is a draw!\n";
            break;
        }
        board.currentPlayerSymbol = (board.currentPlayerSymbol == 'X') ? 'O' : 'X';
    }
}

void PWithP() {
    ultemate_X_O_Board board(3, 3);

    int largePosition, smallPosition;
    int largeRow, largeCol, smallRow, smallCol;

    while (!board.gameOver) {
        board.display_board();
        cout << "Player " << board.currentPlayerSymbol << "'s turn." << endl;
        cout << "Enter the large board position (1-9): ";
        cin >> largePosition;

        largeRow = (largePosition - 1) / 3;
        largeCol = (largePosition - 1) % 3;

        cout << "Enter the position (1-9) in the small board: ";
        cin >> smallPosition;

        smallRow = (smallPosition - 1) / 3;
        smallCol = (smallPosition - 1) % 3;

        if (!board.is_valid_move(largeRow, largeCol, smallRow, smallCol)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        int globalRow = largeRow * 3 + smallRow;
        int globalCol = largeCol * 3 + smallCol;
        board.board[globalRow][globalCol] = board.currentPlayerSymbol;

        char subBoard[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                subBoard[i][j] = board.board[largeRow * 3 + i][largeCol * 3 + j];
            }
        }

        if (board.checkWin(subBoard)) {
            board.mainboard[largeRow][largeCol] = board.currentPlayerSymbol;
            board.displayBoard();
            if (board.is_win(' ')) {
                cout << "Player " << board.currentPlayerSymbol << " wins the game!" << endl;
                board.gameOver = true;
                break;
            }
        } else if (board.checkDraw(subBoard)) {
            board.mainboard[largeRow][largeCol] = 'D';
            board.displayBoard();
        }

        if (board.is_win(' ')) {
            cout << "Player " << board.currentPlayerSymbol << " wins the game!" << endl;
            board.gameOver = true;
            break;
        }

        if (board.is_draw()) {
            cout << "The game is a draw!" << endl;
            board.gameOver = true;
            break;
        }
        board.currentPlayerSymbol = (board.currentPlayerSymbol == 'X') ? 'O' : 'X';
    }
}

void playU() {
    int choice;
    cout << "Welcome to Pyramid Tic-Tac-Toe!" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    cout << "(1/2) => ";
    cin >> choice;

    if (choice == 1) {
        PWithP();
    }
    else if (choice == 2) {
        PWithC();
    }
    else {
        cout << "Invalid choice. Exiting game." << endl;
    }
}

#endif
