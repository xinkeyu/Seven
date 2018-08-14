#include <queue>
#ifndef GAME_H
    #define GAME_H

    class Game{
        //keep track the board state
        int board[8][7];
        //1-7: regular token
        //-1: empty
        //-2: !
        //-3: ?
        int currentToken;
        int positions[7];//row position
        int turn;
        
        //used to calculate score
        int score;
        int multiplier;
        
        
    public:
        Game();
        void displayBoard();
        int nextNum();
        void advanceGame();//generate the next token, possibly add a new line of unknowns
        void placeToken(int col);
        void evaluateBoard();//check if we can make something disappear
        bool evaluateRow(int currentChecking);
        bool evaluateCol();
        void popRow(std::queue<int> number, int row);
        void popCol(std::queue<int>* number, int col);
        void upRow();//add one row unknowns
        void moveOver(int col);
        void detectUnknown(int row, int col);
        void revealUnknown(int row, int col);
        bool gameOver();
    };
#endif
