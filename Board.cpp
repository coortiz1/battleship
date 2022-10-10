#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>
#include "utility.h"

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
    vector<int> addedShipID;
    vector<Point> addedShipTopLeft;
    vector<Direction> addedShipDir;
    char gameBoard[MAXROWS][MAXCOLS] = {};
    char blockedBoard[MAXROWS][MAXCOLS] = {};

    int symbolMatch(Point p);
    bool correctPosition(Point topOrLeft, int shipId, Direction dir);
    void writeSymbol(int r, int c, int length, char symbol, Direction dir);
    bool shipGone(int r, int c, int length, Direction dir);
};

//given a point p, looks at the symbol there and returns the ship id it is correlated to
int BoardImpl::symbolMatch(Point p)
{
    char symbol = gameBoard[p.r][p.c];
    for (int i = 0; i < addedShipID.size(); i++)
    {
        if (m_game.shipSymbol(addedShipID.at(i)) == symbol)
            return addedShipID.at(i);

    }
    return -1;
}

//checks whether the ship placement is out of bounds or not
bool BoardImpl::correctPosition(Point topOrLeft, int shipId, Direction dir)
{
    int r = topOrLeft.r;
    int c = topOrLeft.c;
    //top or left already out of bounds
    if (r < 0 || r >= m_game.rows() || c < 0 || c >= m_game.cols())
        return false;
    int sLength = m_game.shipLength(shipId);
    //ship partially or completely outside board
    //vertical
    if (dir == VERTICAL && r + sLength - 1 >= m_game.rows())
        return false;
    //horizontal
    if (dir == HORIZONTAL && c + sLength - 1 >= m_game.cols())
        return false;
    return true;
}

//writes the passed in symbol in the passed in point for A length in direction dir
void BoardImpl::writeSymbol(int r, int c, int length, char symbol, Direction dir)
{
    for (int i = 0; i < length; i++)
    {
        if (dir == VERTICAL)
            gameBoard[r + i][c] = symbol;
        if (dir == HORIZONTAL)
            gameBoard[r][c + i] = symbol;
    }
}

//given the top left point, length, and direction, tells you if the ship there is destroyed
bool BoardImpl::shipGone(int r, int c, int length, Direction dir)
{
    for (int i = 0; i < length; i++)
    {
        if (dir == VERTICAL)
        {
            if (gameBoard[r+i][c] != 'X')
                return false;
        }
        if (dir == HORIZONTAL)
        {
            if (gameBoard[r][c + i] != 'X')
                return false;
        }
    }
    return true;
}

//BoardImpl constructor, set all points to be dots
BoardImpl::BoardImpl(const Game& g) : m_game(g)
{
    clear();
}

//sets all points in the board to be dots
void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            gameBoard[r][c] = '.';
            blockedBoard[r][c] = '.';
        }
    }
}

//blocks half of the cells
void BoardImpl::block()
{
    int totalCells = (m_game.rows()) * (m_game.cols());
    int blockedCount = 0;
    while (blockedCount != totalCells/2)
    {
        int r = randInt(m_game.rows());
        int c = randInt(m_game.cols());
        if (blockedBoard[r][c] != 'B')
        {
            blockedBoard[r][c] = 'B';
            blockedCount++;
        }
    }
}

//unblocks any cells that were blocked
void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
            blockedBoard[r][c] = '.';
    }
}

//returns a bool telling you whether a ship can be placed at the values given
//if a ship can be placed there, places it there
bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    int r = topOrLeft.r;
    int c = topOrLeft.c;
    //ID invalid
    if (shipId < 0 || shipId >= m_game.nShips())
        return false;
    int sLength = m_game.shipLength(shipId);
    //entire ship at correct position
    if (!correctPosition(topOrLeft, shipId, dir))
        return false;
    // matching ids to already added ships
    for (int i = 0; i < addedShipID.size(); i++)
    {
        if (shipId == addedShipID.at(i))
            return false;
    }
    //no overlap or blocked positions
    for (int i = 0; i < sLength; i++)
    {
        if (dir == HORIZONTAL)
        {
            if (gameBoard[r][c + i] != '.' || blockedBoard[r][c + i] == 'B')
                return false;
        }
        if (dir == VERTICAL)
        {
            if (gameBoard[r + i][c] != '.' || blockedBoard[r + i][c] == 'B')
                return false;
        }
    }
    //adds ship to board
    addedShipID.push_back(shipId);
    addedShipTopLeft.push_back(topOrLeft);
    addedShipDir.push_back(dir);
    //writes the symbol on the board
    writeSymbol(r, c, sLength, m_game.shipSymbol(shipId), dir);
    return true;
}

//returns a bool telling you whether a ship can be unplaced at the values given
//if a ship can be unplaced there, unplaces it
bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId < 0 || shipId >= m_game.nShips())
        return false;
    int r = topOrLeft.r;
    int c = topOrLeft.c;
    int sLength = m_game.shipLength(shipId);
    //shipID invalid
    for (int i = 0; i < addedShipID.size(); i++)
        if (shipId == addedShipID.at(i))
        {
            if (topOrLeft.r == addedShipTopLeft.at(i).r &&
                topOrLeft.c == addedShipTopLeft.at(i).c &&
                dir == addedShipDir.at(i))
            {
                writeSymbol(r, c, sLength, '.', dir);
                addedShipID.erase(addedShipID.begin() + i);
                addedShipTopLeft.erase(addedShipTopLeft.begin() + i);
                addedShipDir.erase(addedShipDir.begin() + i);
                return true;
            }
        }
    return false; 
}

//displays the board
void BoardImpl::display(bool shotsOnly) const
{
    int rows = m_game.rows();
    int cols = m_game.cols();
    //top row
    cout << "  ";
    for (int c = 0; c < cols; c++)
    {
        cout << c;
    }
    cout << endl;
    //rest of the rows
    for (int r = 0; r < rows; r++)
    {
        //row number
        cout << r << " ";
        for (int c = 0; c < cols; c++)
        {
            if (!shotsOnly)
                //prints out entire gameboard with ship positions
                cout << gameBoard[r][c];
            else
            {
                //prints out shots only and rest are dots
                if (gameBoard[r][c] != 'X' && gameBoard[r][c] != 'o')
                    cout << '.';
                else
                    cout << gameBoard[r][c];
            }            
        }
        cout << endl;
    }
}

//attacks a ship at the values passed in
//booleans passed in by value are changed depending on result
bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    int r = p.r;
    int c = p.c;
    //attack is out of bounds or on previously attacked position
    if (r < 0 || r >= m_game.rows() || c < 0 || c >= m_game.cols() || 
        gameBoard[r][c] == 'X' || gameBoard[r][c] == 'o')
    {
        shotHit = false;
        shipDestroyed = false;
        shipId = -1;
        return false;
    }
    //position is a segment of the ship
    if (gameBoard[r][c] != '.')
    {
        shotHit = true;
        
    }
    //position is water
    else
    {
        shotHit = false;
        shipDestroyed = false;
        shipId = -1;
        gameBoard[r][c] = 'o';
    }
    //position is a ship and you match the symbol to the shipid
    if (shotHit && symbolMatch(p) != -1)
    {
        int shipID = symbolMatch(p);
        gameBoard[r][c] = 'X';
        int length = m_game.shipLength(shipID);
        Point p;
        Direction d;
        for (int i = 0; i < addedShipID.size(); i++)
        {
            if (shipID == addedShipID.at(i))
            {
                p = addedShipTopLeft.at(i);
                d = addedShipDir.at(i);
            }
        }
        //if ship is destroyed
        if (shipGone(p.r, p.c, length, d))
        {
            shipDestroyed = true;
            shipId = shipID;
        }
        else
            shipDestroyed = false;
    }
    return true;
}

//true if all ships are destroyed, else false
bool BoardImpl::allShipsDestroyed() const
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (gameBoard[r][c] != 'X' && gameBoard[r][c] != '.' && gameBoard[r][c] != 'o')
                return false;
        }
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.


Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
