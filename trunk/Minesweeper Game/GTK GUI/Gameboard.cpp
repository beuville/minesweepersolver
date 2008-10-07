#include <iostream>
#include <ctime>
#include "Gameboard.h"
using namespace std;
/*
* constructor
*/
Gameboard::Gameboard(int bWidth, int bHeight, int nMines, int seed) {
    newBoard = true;
    
    //set the seed
    gameSeed = seed;
    if(gameSeed==-1)
        srand(time(0));
    else
        srand(gameSeed);
    
    boardWidth = bWidth;
    boardHeight = bHeight;
    
    //don't allow every square to be a mine
    if(nMines >= bWidth*bHeight)
        boardNumberOfMines = bWidth*bHeight - 1;
    else
        boardNumberOfMines = nMines;
    
    //create a multidimensional array for holding the gameboard
    board = new int ** [boardHeight];
    for (int i = 0; i <= boardHeight; i++)
        board[i] = new int* [boardWidth];
    
    //create a multidimensional array for holding the player's board
    boardPlayer = new int ** [boardHeight];
    for (int i = 0; i <= boardHeight; i++)
        boardPlayer[i] = new int* [boardWidth];
    
    for (int i = 0; i < boardHeight; i++)
        for (int j = 0; j < boardWidth; j++)
        {
            board[i][j] = new int(0);
        }
    
    //make sure the boards are clear
    clear();
}
/*
* destructor
*/
Gameboard::~Gameboard() {
    for (int i = 0; i <= boardWidth-1; i++)
        for (int n = 0; n <= boardHeight-1; n++) {
            delete board[i][n];
            delete boardPlayer[i][n];
        }
}
/*
* print out the board
*/
void Gameboard::print(char player) {
    cout << endl;
    for (int j = 1; j <= (boardHeight*4)+1; j++)
        cout << '-';
    cout << endl;
    for (int i = 0; i <= boardWidth-1; i++) {
        for (int n = 0; n <= boardHeight-1; n++) {
            cout << "| ";
            if (getCoordinate(i, n, player) == TYPE_MINE) cout << 'M';
            else if (getCoordinate(i, n, player) == TYPE_HIDDEN) cout << '#';
            else if (getCoordinate(i, n, player) == TYPE_FLAG) cout << '!';
            else if (getCoordinate(i, n, player) == 0) cout << ' ';
            else cout << getCoordinate(i, n, player);
            cout << ' ';
        }
        cout << '|';
        cout << endl;
        for (int j = 1; j <= (boardHeight*4)+1; j++)
        cout << '-';
        cout << endl;
    }
}
/*
* determines if a mine should be set a square
*/
bool Gameboard::setMine() {
    return ((rand()%(boardWidth*boardHeight)) == 0);
}
/*
* report percentage of board cleared
*/
float Gameboard::percentComplete() {
    float percent = (1.0*boardSquaresCleared)/((boardWidth*boardHeight)-boardNumberOfMines);
    return (percent*100);
}
/*
* choose a square as a player
*/
void Gameboard::pickSquare(int i, int n) {
    if(i<boardHeight && n<boardWidth && i>-1 && n>-1)
    {
        if(newBoard){
            startSquarei = i;
            startSquaren = n;
            generate();
            newBoard = false;
            pickSquare(i, n);
        }
        //find value of picked square
        int contents = getCoordinate(i, n, 'm');
        
        //check to see if it's a mine
        /*if(contents == TYPE_MINE){
            //if a mine display percent complete
        //        cout << percentComplete() << endl;
        //}
        //if it's already been picked, do nothing
        else*/ if (contents == getCoordinate(i, n))
            ;
        else
        {
            setCoordinate(i, n, contents, 'p');
            
            if(contents != TYPE_MINE)
                boardSquaresCleared++;
            
            //if the square is empty, clear out neighboring trivial squares
            if (contents == 0)
            {
                for(int j=-1; j<2; j++)
                    for(int k=-1; k<2; k++)
                        if(k!=0 || j!= 0)
                            pickSquare(i+j, n+k);
            }
        }
    }
    else
    {
        ;
    }
}
/*
* clear the board of values
*/
void Gameboard::clear() {
    for (int i = 0; i < boardHeight; i++)
        for (int n = 0; n < boardWidth; n++)
        {
            setCoordinate(i, n, 0, 'p');
            setCoordinate(i, n, 0, 'm');
        }
    boardSquaresCleared = 0;
}
/*
* sets the value of square
*/
void Gameboard::setCoordinate(int i, int n, int value, char player) {
    if(player == 'm')
    {
        delete board[i][n];
        board[i][n] = new int(value);
    }
    else if(player == 'p')
    {
        delete boardPlayer[i][n];
        boardPlayer[i][n] = new int(value);
    }
}
/*
* gets the value of a square
*/
int Gameboard::getCoordinate(int i, int n, char player) {
    if(player == 'm')
    {
        return *board[i][n];
    }
    else if(player == 'p')
    {
        return *boardPlayer[i][n];
    }
    else
        return 0;
}
/*
* Generate the board layout
*/
void Gameboard::generate() {
    clear();
    int minesToBePlaced = boardNumberOfMines;
    //random code to place the mines
    do {
        for (int i = 0; i < boardHeight; i++)
            for (int n = 0; n < boardWidth; n++) {
                //check to see if a mine can/should be placed
                if (minesToBePlaced != 0 && (getCoordinate(i, n, 'm') != TYPE_MINE)
                                         && !(i == startSquarei && n == startSquaren)
                                         && (setMine())) {
                    setCoordinate(i, n, TYPE_MINE, 'm');
                    placeAttached(i, n, 'm');
                    minesToBePlaced--;
                }
                //mask the player board
                setCoordinate(i, n, TYPE_HIDDEN, 'p');
            }
    } while (minesToBePlaced > 0);
}
/*
* increment each square around a mine by 1
*/
void Gameboard::placeAttached(int &m_i, int &m_n, char player) {
    for (int i = m_i-1; i <= m_i+1; i++)
        for (int n = m_n-1; n <= m_n+1; n++)
            if ((n > -1) & (i > -1) & (i < boardHeight) & (n < boardWidth))
                if (getCoordinate(i, n, player) != TYPE_MINE)
                    setCoordinate(i, n, getCoordinate(i, n, player)+1, player);
}
