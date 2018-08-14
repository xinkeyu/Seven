#include <iostream>
#include <cstdlib>
#include <queue>

#include "game.h"

Game::Game(){
    for(int i = 0; i < 8; i++){
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
            else if(board[row][col] == -2)
                std::cout << "!";
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
    bool toContinue = false;
    for(int num = 1; num <= 7; num++){
        if(evaluateRow(num)){
            toContinue = true;
        }
    }
    if(evaluateCol()){
        toContinue = true;
    }
    if(toContinue){
        displayBoard();
        multiplier *= 2;
        evaluateBoard();
    }
}

bool Game::evaluateRow(int currentChecking){
    int flag = -1 ;
    std::queue<int> numbers;
    
    int sequenceLength = 0;
    //std::cout << "Checking " << currentChecking << "..." << std::endl;
    for (int row = 0; row < 7; row++){
        flag = -1;
        sequenceLength = 0;
        for (int col = 0; col < 7; col++){
            if( board[row][col] == currentChecking && sequenceLength == (currentChecking - 1) && col == 6 ){
                //std::cout << "case 1" << std::endl;
                numbers.push(col);

                popRow(numbers, row);
                return true;
            }else if(col == 6 && sequenceLength == (currentChecking-1) && flag != -1 && board[row][col] != -1){
                
                popRow(numbers, row);

                return true;
            }else if(flag != -1 && sequenceLength == currentChecking && board[row][col] == -1){
                //already found a sequence of desired length w/ token inside 
                //std::cout << "case 2" << std::endl;

                popRow(numbers, row);

                return true;
            }else{//cannot eliminate, keep checking
                if(board[row][col] == currentChecking){
                    //std::cout << "case 3" << std::endl;
                    sequenceLength++;
                    flag = col;
                    numbers.push(col);
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

bool Game::evaluateCol(){
    std::queue<int> numbers;
    bool isChanged = false;
    for(int col = 0; col < 7; col++){
        for(int row = 0; row < positions[col]; row++){
            if(board[row][col] == positions[col]){
                numbers.push(row);
                std::cout << "YESSSS " << col << std::endl;
            }
        }
        if(!numbers.empty()){
            popCol(&numbers, col);
            isChanged = true;
        }
    }
    
    return isChanged;
}

void Game::popRow(std::queue<int> numbers, int row){
    int gain = 0;
    int curNum;
    while(!numbers.empty()){
        curNum = numbers.front();
        gain += 7 * multiplier;
        numbers.pop();
        board[row][curNum] = -1;
        detectUnknown(row, curNum);
        positions[curNum]--;
    }
    
    score += gain;
    std::cout << "BANG!!! +" << gain << std::endl;
}

void Game::popCol(std::queue<int>* numbers, int col){
    int gain = 0;
    int curNum;
    while(!numbers->empty()){
        curNum = numbers->front();
        gain += 7 * multiplier;
        numbers->pop();
        board[curNum][col] = -1;
        detectUnknown(curNum, col);
        positions[col]--;
    }
    moveOver(col);
    score += gain;
    std::cout << "BANG FROM COL!!! +" << gain << std::endl;
}

void Game::moveOver(int col){
    for(int row = 0; row < 7; row++){
        if(board[row][col] == -1){
            for(int cur = row; cur < 7; cur++){
                board[row][col] = board[row][col+1];
            }
        }
    }
}

void Game::detectUnknown(int row, int col){
    if(row == 0 && col == 0){
        revealUnknown(row,col+1);
        revealUnknown(row+1,col);
    }else if(row == 0 && col== 6){
        revealUnknown(row,col-1);
        revealUnknown(row+1,col);
    }else if(col == 0){//col == 0; row != 0
        revealUnknown(row-1,col);
        revealUnknown(row+1,col);
        revealUnknown(row, col+1);
    }else if(col == 6){//col == 6; row != 0
        revealUnknown(row-1,col);
        revealUnknown(row+1,col);
        revealUnknown(row, col-1);
    }else{
        revealUnknown(row-1,col);
        revealUnknown(row+1,col);
        revealUnknown(row,col-1);
        revealUnknown(row,col+1);
    }
}

void Game::revealUnknown(int row, int col){
    if(board[row][col] == -3){
        board[row][col] = -2;
    }else if(board[row][col] == -2){
        board[row][col] = nextNum();
    }
}

bool Game::gameOver(){
    
    for(int col = 0; col < 7; col++){
        if(positions[col] == 7){
            return true;
        }else if(positions[col] < 6){
            return false;
        }
    }
    
    return true;//all full 
}
