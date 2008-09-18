#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

const int TYPE_MINE = 9; //is of type mine
const int TYPE_HIDDEN = -1;
const int TYPE_FLAG = -2;

//Gameboard class
class Gameboard {
    public:
        Gameboard(int bWidth, int bHeight, int nMines, int seed=-1);
        ~Gameboard();
        void generate();
        int getCoordinate(int i, int n, char player='p');
        void print(char player);
        void pickSquare(int i, int n);
        float percentComplete();
        int boardWidth;
        int boardHeight;
        int boardNumberOfMines;
        int boardSquaresCleared;
    private:
        int gameSeed;
        bool setMine();
        void clear();
        void placeAttached(int &i, int &n, char player);
        void setCoordinate(int i, int n, int value, char player);
        int *** board;
        int *** boardPlayer;
};
#endif /*Gameboard_H_*/
