#include "Connect4.h"
#include "Pyramid_X_O.h"
#include "SUS_X_O.h"
#include "UltimateTicTacToe.h"
#include "Numerical_Tic_Tac_Toe.h"
#include "Words_Tic_Tac_Toe.h"
#include "misereTicTacToe.h"
#include "T5_5TicTacToe.h"
#include <iostream>
using namespace std;

template <typename T>
class GameManager {
public:
    void run();
};

//IMPLEMENTATION

template <typename T>
void GameManager<T>::run() {
    int gameChoice;
    bool play = true;

    while(play){
        cout << "Welcome to the Game Collection!" << endl;
        cout << "Choose a game:" << endl;
        cout << "1. Pyramid Tic-Tac-Toe" << endl;
        cout << "2. Four in a Row" << endl;
        cout << "3. 5x5 Tic-Tac-Toe" << endl;
        cout << "4. Words Tic-Tac-Toe" << endl;
        cout << "5. Numerical Tic-Tac-Toe" << endl;
        cout << "6. Misere Tic-Tac-Toe" << endl;
        cout << "7. Ultimate Tic-Tac-Toe" << endl;
        cout << "8. SUS Tic-Tac-Toe" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter the number of your choice => ";
        cin >> gameChoice;

        if (gameChoice == 1) {
            play_Pyramid_tictactoe();
        }
        else if (gameChoice == 2) {
            play_Connect4();
        }
        else if(gameChoice == 3){
            playFive();
        }
        else if (gameChoice == 4){
            play_Words_tic_tac_toe();
        }
        else if (gameChoice == 5){
            play_numerical_tic_tac_toe();
        }
        else if (gameChoice == 6){
            playMisere();
        }
        else if (gameChoice == 7){
            playU();
        }
        else if (gameChoice == 8){
            playSUS();
        }
        else if (gameChoice == 9){
            cout << "exit :(";
            play = false;
        }
        else {
            cout << "Invalid choice!" << endl;
        }
    }
}

////=> the main function////
int main() {
    GameManager<char> game;
    game.run();
}
