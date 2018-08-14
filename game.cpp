#include <iostream>
#include <cstdlib>

#include "game.h"

Game::Game(){
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++){
            board[i][j] = -1;
        }
        positions[i] = 0;
    }
    score = 0;
    turn = 1;
}

void Game::displayBoard(){

    for(int row = 6; row >= 0; row--){
        for(int col = 0; col < 7; col++){
            std::cout << "|";
            if(board[row][col] == -1)
                std::cout << " ";
            else if(board[row][col] == -3)
                std::cout << "?";
            else 
                std::cout << board[row][col];

        }
        std::cout << "|";
        std::cout << std::endl;
        
    }
    std::cout << "---------------" << std::endl;
    std::cout << "|0|1|2|3|4|5|6|" << std::endl;
}

int Game::nextNum(){
    return rand()%7+1;
}

void Game::advanceGame(){
    currentToken = nextNum();
    std::cout << std::endl;
    for(int i = 0; i < 7; i++){
        std::cout << positions[i] << " ";
    }
    if(turn % 10 == 0){
        upRow();
    }
    turn++;
    std::cout << std::endl;
    std::cout << "CURRENT TOKEN: " << currentToken << "   ";
    std::cout << "CURRENT SCORE: " << score<< std::endl;
}

void Game::upRow(){
    for(int row = 7; row >= 1; row--){
        for(int col = 0; col < 7; col++){
            board[row][col] = board[row-1][col];
        }
    }
    for(int col = 0; col < 7; col++){
        board[0][col] = -3;
        positions[col]++;
    }
}

void Game::placeToken(int col){
    int row = positions[col];
    positions[col]++;
    board[row][col] = currentToken;
    multiplier = 1; //for calculating score
}

void Game::evaluateBoard(){
    for(int num = 1; num <= 7; num++){
        if(evaluateRow(num)){
            multiplier++;
            evaluateBoard();
            return;
        }
    }
}

bool Game::evaluateRow(int currentChecking){
    int flag = -1 ;

    int sequenceLength = 0;
    //std::cout << "Checking " << currentChecking << "..." << std::endl;
    for (int row = 0; row < 7; row++){
        flag = -1;
        sequenceLength = 0;
        for (int col = 0; col < 7; col++){
            if( board[row][col] == currentChecking && sequenceLength == (currentChecking - 1) && col == 6 ){
                //std::cout << "case 1" << std::endl;
                board[row][col] = -1;
                positions[col]--;
                score += currentChecking*multiplier;
                std::cout << "BANG!!! +" << currentChecking*multiplier << std::endl;
                displayBoard();
                return true;
            }else if(col == 6 && sequenceLength == (currentChecking-1) && flag != -1 && board[row][col] != -1){
                board[row][flag] = -1;
                positions[flag]--;
                score += currentChecking*multiplier;
                std::cout << "BANG!!! +" << currentChecking*multiplier << std::endl;
                displayBoard();
                return true;
            }else if(flag != -1 && sequenceLength == currentChecking && board[row][col] == -1){
                //already found a sequence of desired length w/ token inside 
                //std::cout << "case 2" << std::endl;
                board[row][flag] = -1;
                positions[flag]--;
                score += currentChecking*multiplier;
                std::cout << "BANG!!! +" << currentChecking*multiplier << std::endl;
                displayBoard();
                return true;
            }else{//cannot eliminate, keep checking
                if(board[row][col] == currentChecking){
                    //std::cout << "case 3" << std::endl;
                    sequenceLength++;
                    flag = col;
                }else if(board[row][col] == -1){//empty
                    //std::cout << "case 4" << std::endl;
                    sequenceLength = 0;
                    flag = -1;
                }else{
                    //std::cout << "case 5" << std::endl;
                    sequenceLength++;
                }
            }
        }
    } 
    return false;   
}
