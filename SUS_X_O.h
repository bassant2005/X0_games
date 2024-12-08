#ifndef _SUS_X_O_H
#define _SUS_X_O_H

#include "BoardGame_Classes.h"

class SUS_X_O_Board : public Board<char> {
public:
    char playerLetter = 'S';
    char opponentLetter = 'U';
    int scorePlayer = 0;
    int scoreOpponent = 0;

    SUS_X_O_Board(int r, int c) : Board<char>(r, c) {
        initializeBoard();
        srand(time(0));
    }

    void initializeBoard() override {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = ' ';
            }
        }
    }

    void display_board() override {
        cout << endl;
        int num = 1;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    cout << num;
                }
                else {
                    cout << board[i][j];
                }
                if (j < 2) cout << " | ";
                num++;
            }
            cout << endl;
            if (i < 2) cout << "--|---|---\n";
        }
        cout << endl;
    }

    bool is_valid_move(int position) override{
        int row = (position - 1) / 3;
        int col = (position - 1) % 3;
        return (position >= 1 && position <= 9 && board[row][col] != 'S' && board[row][col] != 'U');
}


    bool is_win(char currentPlayerSymbol) override {
        // Check rows, columns, and diagonals for "SUS"
        for (int i = 0; i < 3; i++) {
            if ((this->board[i][0] == 'S' && this->board[i][1] == 'U' && this->board[i][2] == 'S') ||
                (this->board[0][i] == 'S' && this->board[1][i] == 'U' && this->board[2][i] == 'S')) {
                return true;
            }
        }
        // Diagonals
        if ((this->board[0][0] == 'S' && this->board[1][1] == 'U' && this->board[2][2] == 'S') ||
            (this->board[0][2] == 'S' && this->board[1][1] == 'U' && this->board[2][0] == 'S')) {
            return true;
        }
        return false;
    }

    bool update_board(int n, char symbol) override {
        if (n < 1 || n > 9) return false;
        int row = (n - 1) / 3;
        int col = (n - 1) % 3;
        if (this->board[row][col] != 'S' && this->board[row][col] != 'U') {
            this->board[row][col] = symbol;
            return true;
        }
        return false;
    }


    bool is_draw() override {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'S' && board[i][j] != 'U') return false;
            }
        }
        return true;
    }


    void countSUS(int position, char currentPlayerLetter) {
        int row = (position - 1) / 3;
        int col = (position - 1) % 3;
        int n_moves = 0;

        if (board[row][0] == 'S' && board[row][1] == 'U' && board[row][2] == 'S') {
            n_moves++;
        }

        if (board[0][col] == 'S' && board[1][col] == 'U' && board[2][col] == 'S') {
            n_moves++;
        }

        if (row == col) {
            if (board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S') {
                n_moves++;
            }
        }

        if (row + col == 2) {
            if (board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S') {
                n_moves++;
            }
        }

        if (currentPlayerLetter == playerLetter) {
            scorePlayer += n_moves;
        }

        else {
            scoreOpponent += n_moves;
        }
    }
    void display_count(){
        cout << "The board is full!\n";
        cout << "Player S's Score (" << playerLetter << "): " << scorePlayer << "\n";
        cout << "Player U's Score (" << opponentLetter << "): " << scoreOpponent << "\n";

        if (scorePlayer > scoreOpponent) {
            cout << "Player S wins!\n";
        }
        else if (scorePlayer < scoreOpponent) {
            cout << "Player U wins!\n";
        }
        else {
            cout << "It's a draw!\n";
        }
    }
};

class SUS_X_O_AI_Player : public SUS_X_O_Board {

public:
    SUS_X_O_AI_Player(int r1, int c1, int r, int c) : SUS_X_O_Board(r1, c1) {}

    void hardAI() {
        currentPlayerSymbol = playerLetter;
        int row, col;

        while (!is_draw()) {
            if (currentPlayerSymbol == playerLetter) {
                // Player's turn
                int position;
                cout << "Your turn (" << playerLetter << "). Choose a position (1-9): ";
                cin >> position;

                if (is_valid_move(position)) {
                    update_board(position, playerLetter);
                    countSUS(position, playerLetter);
                    display_board();
                }
                else {
                    cout << "Invalid move. Try again.\n";
                    continue;
                }
            }
            else { // Computer's turn
                bool moveMade = false;

                // Check if AI can win or block
                for (int i = 0; i < 3 && !moveMade; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        if (board[i][j] == ' ') {
                            board[i][j] = opponentLetter;
                            if (is_win(opponentLetter)) {  // win
                                row = i;
                                col = j;
                                board[i][j] = ' ';
                                moveMade = true;
                                break;
                            }
                            board[i][j] = ' ';

                            board[i][j] = playerLetter;
                            if (is_win(opponentLetter)) {
                                row = i;
                                col = j;
                                board[i][j] = ' ';
                                moveMade = true;
                                break;
                            }
                            board[i][j] = ' ';
                        }
                    }
                }

                if (!moveMade) {
                    do {
                        row = rand() % 3;
                        col = rand() % 3;
                    } while (board[row][col] != ' ');
                }

                cout << "AI is making a move..." << endl;
                int position = row * 3 + col + 1;
                cout << "AI choose " << position << endl;
                update_board(position, opponentLetter);
                countSUS(position, opponentLetter);

                cout << endl;
                display_board();
            }
            currentPlayerSymbol = (currentPlayerSymbol == playerLetter) ? opponentLetter : playerLetter;
        }
        display_count();
    }
};

class SUS_X_O_Random_Player : public SUS_X_O_Board {

public:
    SUS_X_O_Random_Player(int r1, int c1, int r, int c) : SUS_X_O_Board(r1, c1) {}

    void getRandomMove() {
        currentPlayerSymbol = playerLetter; // Start with the player

        while (!is_draw()) {
            if (currentPlayerSymbol == playerLetter) {
                int position;
                cout << "Your turn (" << playerLetter << "). Choose a position (1-9): ";
                cin >> position;

                if (is_valid_move(position)) {
                    update_board(position, playerLetter);
                    countSUS(position,playerLetter);
                    display_board();
                } else {
                    cout << "Invalid move. Try again.\n";
                    continue;
                }
            }
            else { // Computer's turn
                int position;
                do {
                    position = rand() % 9 + 1; // Random position between 1-9
                } while (!is_valid_move(position));

                cout << "Computer is making a move..." << endl;
                update_board(position, opponentLetter);
                countSUS(position, opponentLetter);  // Update score for the computer
                cout << "Computer choose " << position << endl;
                cout << endl;
                display_board();
            }
            currentPlayerSymbol = (currentPlayerSymbol == playerLetter) ? opponentLetter : playerLetter;
        }
        display_count();
    }
};

void PlayVSPlayer() {
    SUS_X_O_Board board(3, 3);  // 3 rows and 5 columns for pyramid board
    Player player1("Player 1", 'S');
    Player player2("Player 2", 'U');

    cout << "Player vs Player Mode!\n";
    cout << "Player 1 is '" << board.playerLetter << "' and Player 2 is '" << board.opponentLetter << "'.\n";

    board.display_board();

    board.currentPlayerSymbol = player1.getsymbol(); // Start with Player 1

    while (true) {

        int position;
        cout << "Player " << "(" << board.currentPlayerSymbol << "), choose a position (1-9): ";
        cin >> position;

        if (board.is_valid_move(position)) {

            board.update_board(position, board.currentPlayerSymbol);
            board.display_board();

            board.countSUS(position, board.currentPlayerSymbol); // Pass last move's position
            // Check if the board is full
            if (board.is_draw()) {
                board.display_count();
                break;
            }
            board.currentPlayerSymbol = (board.currentPlayerSymbol == board.playerLetter) ? board.opponentLetter :board.playerLetter;
        } else {
            cout << "Invalid move. Please try again.\n";
        }
    }
}

void playSUS() {
    SUS_X_O_Board board(3, 3);
    SUS_X_O_AI_Player AI( 3, 3, 3, 3);
    SUS_X_O_Random_Player random( 3, 3, 3, 3);

    cout << "Welcome to SuS Game!" << endl;
    cout << "Choose a game mode:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    cout << "(1/2) => ";

    int mode;
    cin >> mode;

    if (mode == 1) {
        PlayVSPlayer();
    }
    else if (mode == 2) {
        cout << "Player vs Computer Mode!\n";

        cout << "\nYou are 'S' " << "and the computer is 'U'" << ".\n";


        cout << "Choose AI difficulty:" << endl;
        cout << "1. Easy" << endl;
        cout << "2. Hard" << endl;
        cout << "=> ";

        int aiChoice;
        cin >> aiChoice;

        if (aiChoice == 1) {
            board.display_board();
            random.getRandomMove();
        }
        else if (aiChoice == 2) {
            board.display_board();
            AI.hardAI();
        }
        else {
            cout << "Invalid choice!" << endl;
        }
    }
    else {
        cout << "Invalid choice. Exiting...\n";
    }
}

#endif

