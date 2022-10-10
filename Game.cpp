#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include "utility.h"
#include <vector>
using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int gRow;
    int gCol;
    vector<Ship> shipTypes;
    char p1Attacks[MAXROWS][MAXCOLS] = {};
    char p2Attacks[MAXROWS][MAXCOLS] = {};
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols) : gRow(nRows), gCol(nCols)
{
    for (int r = 0; r < gRow; r++)
    {
        for (int c = 0; c < gCol; c++)
        {
            p1Attacks[r][c] = '.';
            p2Attacks[r][c] = '.';
        }
    }
}

//returns row of game
int GameImpl::rows() const
{
    return gRow; 
}

//returns columns of game
int GameImpl::cols() const
{
    return gCol; 
}

//tells you if a passed in point is out of bounds or not
bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

//returns a random point
Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

//adds a ship type of the values given
bool GameImpl::addShip(int length, char symbol, string name)
{
    shipTypes.push_back(Ship(length, nShips(), symbol, name));
    return true;
}

//returns how many ship types there are
int GameImpl::nShips() const
{
    return shipTypes.size();  
}

//returns the length of the ship given its id
int GameImpl::shipLength(int shipId) const
{
    return shipTypes.at(shipId).length(); 
}

//returns symbol of ship given its id
char GameImpl::shipSymbol(int shipId) const
{
    return shipTypes.at(shipId).symbol(); 
}

//returns name of ship given its id
string GameImpl::shipName(int shipId) const
{
    return shipTypes.at(shipId).name();
}

//plays through one game of battleship using the values passed in
Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause = true)
{
    //can't place ships
    if (!p1->placeShips(b1))
        return nullptr;
    if (!p2->placeShips(b2))
        return nullptr;
    bool shothit;
    bool shipdestroyed;
    int shipid;
    bool shothit2;
    bool shipdestroyed2;
    int shipid2;
    //player 1 attacks board 2
    //player 2 attacks board 1
    while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed())
    {
        cout << p1->name() << "'s turn.  Board for " << p2->name() << ":\n";
        b2.display(p1->isHuman());
        Point p1Attack = p1->recommendAttack();
        //ship attack out of bounds or on already attacked area
        if (!isValid(p1Attack) || p1Attacks[p1Attack.r][p1Attack.c] == 'A')
        {
            p1->recordAttackResult(p1Attack, false, false, false, -1);
            cout << p1->name() << " wasted a shot at ("
                << p1Attack.r << "," << p1Attack.c << ").\n";
        }
        //ship attack valid and on never attacked area
        if (isValid(p1Attack) && p1Attacks[p1Attack.r][p1Attack.c] == '.')
        {
            b2.attack(p1Attack, shothit, shipdestroyed, shipid);
            string result;
            if (!shothit)
                result = "missed";
            else if (shothit && !shipdestroyed)
                result = "hit something";
            else if (shothit && shipdestroyed)
                result = "destroyed the " + shipName(shipid);
            cout << p1->name() << " attacked ("
                << p1Attack.r << "," << p1Attack.c << ") and " << result
                << ", resulting in:\n";
            p1Attacks[p1Attack.r][p1Attack.c] = 'A';
            p1->recordAttackResult(p1Attack, true, shothit, shipdestroyed, shipid);
            b2.display(p1->isHuman());
        }
        if (shouldPause && !b2.allShipsDestroyed())
            waitForEnter();
        if (!b2.allShipsDestroyed())
        {
            cout << p2->name() << "'s turn.  Board for " << p1->name() << ":\n";
            b1.display(p2->isHuman());
            Point p2Attack = p2->recommendAttack();
            //if attack is not valid or on previously attacked area
            if (!isValid(p2Attack) || p2Attacks[p2Attack.r][p2Attack.c] == 'A')
            {
                p2->recordAttackResult(p2Attack, false, false, false, -1);
                cout << p2->name() << " wasted a shot at ("
                    << p2Attack.r << "," << p2Attack.c << ").\n";
            }
            //if attack is valid and not on previously attacked area
            if (isValid(p2Attack) && p2Attacks[p2Attack.r][p2Attack.c] == '.')
            {
                b1.attack(p2Attack, shothit2, shipdestroyed2, shipid2);
                string result;
                if (!shothit2)
                    result = "missed";
                else if (shothit2 && !shipdestroyed2)
                    result = "hit something";
                else if (shothit2 && shipdestroyed2)
                    result = "destroyed the " + shipName(shipid2);
                cout << p2->name() << " attacked ("
                    << p2Attack.r << "," << p2Attack.c << ") and " << result
                    << ", resulting in:\n";
                p2Attacks[p2Attack.r][p2Attack.c] = 'A';
                p2->recordAttackResult(p2Attack, true, shothit2, shipdestroyed2, shipid2);
                b1.display(p2->isHuman());
            }
            if (shouldPause && !b1.allShipsDestroyed())
                waitForEnter();
        }
    }
    //board 1 is not completely destroyed, means player 2 lost (winner player 1)
    if (b2.allShipsDestroyed())
    {
        cout << p1->name() << " wins!\n";
        if (p2->isHuman())
        {
            cout << "Here is where " << p1->name() << "'s ships were:\n";
            b1.display(false);
        }
        return p1;
    }
    //winner player 2
    if (b1.allShipsDestroyed())
    {
        cout << p2->name() << " wins!\n";
        if (p1->isHuman())
        {
            cout << "Here is where " << p2->name() << "'s ships were:\n";
            b2.display(false);
        }
        return p2;
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.


Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    
    
    
    
    (shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

