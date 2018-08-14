#include <iostream>
#include "seven.h"
#include "game.h"

int main(){
    Game game;
    int col;
    while(1){
        game.advanceGame();
        game.displayBoard();
        std::cout << "please enter the column number: ";
        std::cin >> col;
        game.placeToken(col);
        game.evaluateBoard();
    }
}
