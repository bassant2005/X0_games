#ifndef _BOARDGAME_CLASSES_H
#define _BOARDGAME_CLASSES_H

#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <ctime>
#include <vector>
using namespace std;

#pragma once

template <typename T>
class Board {
protected:
    int rows, columns;
    int n_moves = 0;

public:
    Board(int r, int c) : rows(r), columns(c) {
        board = new T*[rows];
        for (int i = 0; i < rows; ++i) {
            board[i] = new T[columns]();
        }
    }

    virtual bool is_valid_move(int n);
    virtual void initializeBoard();
    virtual void display_board();
    virtual bool is_win(T currentPlayerSymbol);
    virtual bool is_draw();
    virtual bool update_board(int n, T symbol);

    virtual ~Board() {
        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    T** board;
    char currentPlayerSymbol = ' ';
};

template <typename T>
class Player {
protected:
    string name;
    T symbol;
    Board<T>* boardPtr;

public:
    Player(string n, T symbol);
    Player(T symbol);
    void getmove(int& x) {
        cout << "Enter position: ";
        cin >> x;
    }
    virtual void getmove(int& x,int& y);
    T getsymbol(){
        return symbol;
    }

    virtual void setname(const string &playerName); // Setter for name
    virtual string getname(); // Getter for name
    void setBoard(Board<T>* b);
};

template <typename T>
void Board<T>::initializeBoard() {}

template <typename T>
void Board<T>::display_board() {}

template <typename T>
bool Board<T>::is_win(T currentPlayerSymbol) {}

template <typename T>
bool Board<T>::is_valid_move(int n) {}

template <typename T>
bool Board<T>::is_draw() {}

template <typename T>
bool Board<T>::update_board(int n, T symbol) {}

template class Board<char>;

template <typename T>
Player<T>::Player(string n, T symbol) : name(n), symbol(symbol), boardPtr(nullptr) {}

template <typename T>
Player<T>::Player(T symbol) : name("Computer"), symbol(symbol), boardPtr(nullptr) {}

template <typename T>
string Player<T>::getname() {
    return name;
}

template <typename T>
void Player<T>::setname(const string &playerName) {
    name = playerName;
}

template <typename T>
void Player<T>::setBoard(Board<T>* b) {
    boardPtr = b;
}

template <typename T>
void Player<T>:: getmove(int& x, int& y) {}

template class Player<char>;

#endif //_BOARDGAME_CLASSES_H
