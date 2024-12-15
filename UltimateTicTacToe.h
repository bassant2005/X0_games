#ifndef ULTIMATETICTACTOE_H
#define ULTIMATETICTACTOE_H

// Class for the Ultimate Tic Tac Toe board
class ultimate_X_O_Board : public Board<char> {
public:
    // Constructor to initialize the board and set the starting player
    ultimate_X_O_Board(int r, int c) : Board<char>(r, c) {
        initializeBoard();
        currentPlayerSymbol = 'X';
    }

    bool gameOver = false; // Flag to indicate if the game is over
    char mainboard[3][3]; // Main board that tracks the state of small boards
    char board[9][9]; // 9x9 grid representing the full Ultimate Tic Tac Toe board

    // Initialize the boards with empty values ('*')
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

    // Display the entire Ultimate Tic Tac Toe board
    void display_board() override {
        cout << "\nUltimate Tic Tac Toe Board:\n";

        // Loop through and display the small boards within the larger board
        for (int bigRow = 0; bigRow < 3; bigRow++) {
            for (int smallRow = 0; smallRow < 3; smallRow++) {
                for (int bigCol = 0; bigCol < 3; bigCol++) {
                    for (int smallCol = 0; smallCol < 3; smallCol++) {
                        int globalRow = bigRow * 3 + smallRow;
                        int globalCol = bigCol * 3 + smallCol;
                        char cell = board[globalRow][globalCol];

                        // Print each cell; replace '*' with a more visually clear character
                        cout << (cell == ' ' ? '*' : cell);
                        if (smallCol < 2) cout << " : "; // Separator within small boards
                    }
                    if (bigCol < 2) cout << "  ||  "; // Separator between small boards
                }
                cout << "\n";
            }
            if (bigRow < 2) {
                // Separator between rows of small boards
                cout << string(15, '=') << string(9, '=') << string(15, '=') << "\n";
            }
        }
        cout << "\n";
    }

    // Display the main 3x3 board that tracks the states of sub-boards
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

    // Check if a player has won on a 3x3 board
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

    // Check if the current player has won on the main board
    bool is_win(char currentPlayerSymbol) override {
        for (int i = 0; i < 3; ++i) {
            if (mainboard[i][0] != '*' && mainboard[i][0] == mainboard[i][1] && mainboard[i][1] == mainboard[i][2]) {
                return true;
            }
        }

        for (int i = 0; i < 3; ++i) {
            if (mainboard[0][i] != '*' && mainboard[0][i] == mainboard[1][i] && mainboard[1][i] == mainboard[2][i]) {
                return true;
            }
        }

        if (mainboard[0][0] != '*' && mainboard[0][0] == mainboard[1][1] && mainboard[1][1] == mainboard[2][2]) {
            return true;
        }
        if (mainboard[0][2] != '*' && mainboard[0][2] == mainboard[1][1] && mainboard[1][1] == mainboard[2][0]) {
            return true;
        }

        return false;
    }

    // Validate if a move is allowed
    bool is_valid_move(int largeRow, int largeCol, int smallRow, int smallCol) {
        if (mainboard[largeRow][largeCol] != '*') {
            return false; // Sub-board is already claimed
        }

        int globalRow = largeRow * 3 + smallRow;
        int globalCol = largeCol * 3 + smallCol;
        return board[globalRow][globalCol] == '*'; // Cell must be empty
    }

    // Check if the game is a draw
    bool is_draw() override {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (mainboard[i][j] == '*') return false; // Main board still has empty cells
            }
        }
        return !is_win(' '); // No winner and no empty cells
    }

    // Check if a 3x3 board is in a draw state
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
// Class for a random player in Ultimate Tic Tac Toe
class ultemate_X_O_Random_Player : public ultimate_X_O_Board, public Player<char> {
public:
    // Constructor to initialize the random player
    ultemate_X_O_Random_Player(int r1, int c1, string name, char symbol) : ultimate_X_O_Board(r1, c1), Player<char>(name, symbol) {
        srand(time(0)); // Seed the random number generator
    }

    // Get the player's name
    string getname() override {
        return name;
    }

    // Generate a random valid move for the computer player
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

        // Check if the sub-board is won or drawn and update the main board
        char subBoard[3][3];

        // Extract the 3x3 sub-board based on the chosen largeRow and largeCol
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                subBoard[i][j] = sharedBoard[largeRow * 3 + i][largeCol * 3 + j];
            }
        }

        // Check if the sub-board has a win
        if (checkWin(subBoard)) {
            sharedMainboard[largeRow][largeCol] = currentPlayerSymbol; // Mark as won by current player
        } else if (checkDraw(subBoard)) {
            sharedMainboard[largeRow][largeCol] = 'D'; // Mark as a draw
        }
    }
};

void PWithC() {
    // Create an ultimate tic-tac-toe board with 3x3 grids
    ultimate_X_O_Board board(3, 3);

    // Get Player X's name from user input
    string player1_name;
    cout << "Enter Player X name: ";
    cin >> player1_name;

    // Create a human player for Player X and a random AI player for Player O
    Player player(player1_name, 'X');
    ultemate_X_O_Random_Player randomPlayer(3, 3, "Computer", 'O');

    // Declare variables for positions on the large and small boards
    int largePosition, smallPosition;
    int largeRow, largeCol, smallRow, smallCol;

    // Main game loop, continues until the game is over
    while (!board.gameOver) {
        // Display the current state of the board
        board.display_board();

        // Player's turn
        cout << player.getname() << "'s turn.\n";

        // Prompt the player to select a large board position (1-9)
        cout << "Enter the large board position (1-9): ";
        cin >> largePosition;

        // Convert the input into row and column indices for the large board
        largeRow = (largePosition - 1) / 3;
        largeCol = (largePosition - 1) % 3;

        // Prompt the player to select a position within the small board (1-9)
        cout << "Enter the position (1-9) in the small board: ";
        cin >> smallPosition;

        // Convert the input into row and column indices for the small board
        smallRow = (smallPosition - 1) / 3;
        smallCol = (smallPosition - 1) % 3;

        // Validate the player's move
        if (!board.is_valid_move(largeRow, largeCol, smallRow, smallCol)) {
            cout << "Invalid move. Try again.\n";
            continue; // Skip to the next iteration if the move is invalid
        }

        // Compute the global row and column indices on the ultimate board
        int globalRow = largeRow * 3 + smallRow;
        int globalCol = largeCol * 3 + smallCol;

        // Update the board with Player X's move
        board.board[globalRow][globalCol] = 'X';

        // Check for a win or draw in the sub-board where the move was made
        char subBoard[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                subBoard[i][j] = board.board[largeRow * 3 + i][largeCol * 3 + j];
            }
        }

        // If the sub-board has a win, mark it on the main board
        if (board.checkWin(subBoard)) {
            board.mainboard[largeRow][largeCol] = 'X';
        }
            // If the sub-board is a draw, mark it as 'D' on the main board
        else if (board.checkDraw(subBoard)) {
            board.mainboard[largeRow][largeCol] = 'D';
        }

        // Check if Player X has won the entire game
        if (board.is_win('X')) {
            board.displayBoard();
            cout << player.getname() << " wins the game!\n";
            break;
        }

        // Check if the game has ended in a draw
        if (board.is_draw()) {
            board.displayBoard();
            cout << "The game is a draw!\n";
            break;
        }

        // Switch to the computer's turn
        board.currentPlayerSymbol = (board.currentPlayerSymbol == 'X') ? 'O' : 'X';

        // Get a random move from the computer player
        randomPlayer.getRandomMove(board.board, board.mainboard, board.currentPlayerSymbol);

        // Check if the computer player has won the game
        if (board.is_win('O')) {
            cout << randomPlayer.getname() << " wins the game!\n";
            break;
        }

        // Check if the game has ended in a draw
        if (board.is_draw()) {
            cout << "The game is a draw!\n";
            break;
        }

        // Switch back to Player X's turn
        board.currentPlayerSymbol = (board.currentPlayerSymbol == 'X') ? 'O' : 'X';
    }
}

void PWithP() {
    string player1_name, player2_name;

    // Prompt players to enter their names
    cout << "Enter the name of player 1: ";
    cin >> player1_name;
    cout << "Enter the name of player 2: ";
    cin >> player2_name;

    // Initialize the game board and players
    ultimate_X_O_Board board(3, 3); // 3x3 grid for the ultimate Tic-Tac-Toe game
    Player player1(player1_name, 'X'), player2(player2_name, 'O'); // Assign 'X' and 'O' to players

    int largePosition, smallPosition;
    int largeRow, largeCol, smallRow, smallCol;

    // Game loop continues until a win or draw
    while (!board.gameOver) {
        board.display_board(); // Display the current state of the board

        // Display whose turn it is
        if (board.currentPlayerSymbol == 'X') {
            cout << "Player " << player1.getname() << "'s turn." << endl;
        }
        else {
            cout << "Player " << player2.getname() << "'s turn." << endl;
        }

        // Prompt the player to enter the large board position (1-9)
        cout << "Enter the large board position (1-9): ";
        cin >> largePosition;

        // Calculate the row and column on the large board
        largeRow = (largePosition - 1) / 3;
        largeCol = (largePosition - 1) % 3;

        // Prompt for the small board position (1-9)
        cout << "Enter the position (1-9) in the small board: ";
        cin >> smallPosition;

        // Calculate the row and column on the small board
        smallRow = (smallPosition - 1) / 3;
        smallCol = (smallPosition - 1) % 3;

        // Check if the move is valid
        if (!board.is_valid_move(largeRow, largeCol, smallRow, smallCol)) {
            cout << "Invalid move. Try again." << endl;
            continue; // Ask the player to try again if the move is invalid
        }

        // Update the global board with the player's move
        int globalRow = largeRow * 3 + smallRow;
        int globalCol = largeCol * 3 + smallCol;
        board.board[globalRow][globalCol] = board.currentPlayerSymbol;

        // Create a sub-board for the current small board
        char subBoard[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                subBoard[i][j] = board.board[largeRow * 3 + i][largeCol * 3 + j];
            }
        }

        // Check for a win or draw on the current small board
        if (board.checkWin(subBoard)) {
            board.mainboard[largeRow][largeCol] = board.currentPlayerSymbol; // Mark the large board as won
            board.displayBoard(); // Display the updated board

            // Check if the overall game is won
            if (board.is_win(' ')) {
                if (board.currentPlayerSymbol == 'X') {
                    cout << "Player " << player1.getname() << " wins the game!" << endl;
                }
                else {
                    cout << "Player " << player2.getname() << " wins the game!" << endl;
                }
                board.gameOver = true; // End the game
                break;
            }
        }
        else if (board.checkDraw(subBoard)) {
            board.mainboard[largeRow][largeCol] = 'D'; // Mark the small board as a draw
            board.displayBoard(); // Display the updated board
        }

        // Check if the overall game has a winner
        if (board.is_win(' ')) {
            if (board.currentPlayerSymbol == 'X') {
                cout << "Player " << player1.getname() << " wins the game!" << endl;
            }
            else {
                cout << "Player " << player2.getname() << " wins the game!" << endl;
            }
            board.gameOver = true; // End the game
            break;
        }

        // Check if the overall game is a draw
        if (board.is_draw()) {
            cout << "The game is a draw!" << endl;
            board.gameOver = true; // End the game
            break;
        }

        // Switch the current player
        board.currentPlayerSymbol = (board.currentPlayerSymbol == 'X') ? 'O' : 'X';
    }
}

void playU() {
    int choice;
    // Display menu and get user's choice
    cout << "Welcome to ultimate Tic-Tac-Toe!" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    cout << "(1/2) => ";
    cin >> choice;
    // Start the selected game mode
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
