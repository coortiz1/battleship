#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include "utility.h"
#include <stack>

using namespace std;


//========================================================================
// Timer t;                 // create a timer and start it
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
            std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

Timer timer;
bool testTimer = false;

void fattack(bool& startWrappingItUp)
{
    if (!testTimer)
        return;
    if (!startWrappingItUp)
    {
        for (int k = 0; k < 1000000; k++)
        {
            if (timer.elapsed() > 3900)  // have 3.9 seconds elapsed?
            {
                startWrappingItUp = true;
                return;
            }
            // ... do some computation here ...
        }
    }
}
void fplace(bool& startWrappingItUp)
{
    if (!testTimer)
        return;
    if (!startWrappingItUp)
    {
        for (int k = 0; k < 1000000; k++)
        {
            if (timer.elapsed() > 900)  // have .9 seconds elapsed?
            {
                startWrappingItUp = true;
                return;
            }
            // ... do some computation here ...
        }
    }
}

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

//constructor for awful player, derived from player
AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

//places ship for awful player
bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

//recommends attack for awful player
Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

//does nothing
void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

//does nothing
void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);    
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    void oneStepPlace(Board& b, int shipID);
};

//constructor for human player derived from player
HumanPlayer::HumanPlayer(string nm, const Game& g): Player(nm,g)
{}

//always a human, so always true
bool HumanPlayer::isHuman() const { return true; }

//receives input from human to place ships
bool HumanPlayer::placeShips(Board& b)
{
    string d;
    cout << name() << " must place " << game().nShips() << " ships.\n";
    for (int i = 0; i < game().nShips(); i++)
    {
        oneStepPlace(b, i);
    }
    return true;
}

//receives input from human to attack
Point HumanPlayer::recommendAttack()
{
    int r;
    int c;
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    while (!getLineWithTwoIntegers(r, c))
    {
        cout << "You must enter two integers." << endl;
        cout << "Enter the row and column to attack (e.g., 3 5): ";
    }
    return Point(r, c);
}

//does nothing
void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
    bool /* shotHit */, bool /* shipDestroyed */,
    int /* shipId */)
{}

//does nothing
void HumanPlayer::recordAttackByOpponent(Point /* p */)
{}

//helper function for placeShips (does all the printing and 
//asking for input for one ship)
void HumanPlayer::oneStepPlace(Board& b, int shipID)
{
    string d;
    b.display(false);
    bool notHV = true;
    Direction dir;
    //continually asks until receives valid direction
    while (notHV)
    {
        cout << "Enter h or v for direction of "
            << game().shipName(shipID) << " (length " << game().shipLength(shipID) << "): ";
        getline(cin, d);
        if (d[0] == 'h' || d[0] == 'v')
            notHV = false;
        else
            cout << "Direction must be h or v.\n";
    }
    if (d[0] == 'h')
        dir = HORIZONTAL;
    else if (d[0] == 'v')
        dir = VERTICAL;
    int r;
    int c;
    //continues to ask until it receives a point
    cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
    while (!getLineWithTwoIntegers(r, c))
    {
        cout << "You must enter two integers." << endl;
        cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
    }

    //tells you the result of the point and depending on result will prompt you
    //again for the point
    while (!game().isValid(Point(r, c)) || !b.placeShip(Point(r, c), shipID, dir))
    {
        cout << "The ship can not be placed there." << endl;
        cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
        while (!getLineWithTwoIntegers(r, c))
        {
            cout << "You must enter two integers." << endl;
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
        }
    }
    b.placeShip(Point(r, c), shipID, dir);
    return;
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    bool stateOne = true;
    char status[MAXROWS][MAXCOLS] = {};
    Point middleCross;

    bool helper(Board& b, int shipIndex);
};

//constructor for mediocre, initializes status board to be all dots
MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g)
{
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
            status[r][c] = '.';
    }
}

//places ship for mediocre, uses helper
bool MediocrePlayer::placeShips(Board& b)
{
    Timer();
    bool greaterThanOne = false;
    b.block();
    int counter = 0;
    while (!helper(b, game().nShips() - 1))
    {
        b.unblock();
        b.block();
        counter++;
        //if tried 50 times, return false
        if (counter == 50)
        {
            b.unblock();
            return false;
        }
    }
    b.unblock();
    fplace(greaterThanOne);
    if (greaterThanOne)
        cout << "Fail!";
    return true;
}

//recommend attack for mediocre
Point MediocrePlayer::recommendAttack()
{
    //state 1
    if (stateOne)
    {            
        bool alreadyAttacked = true;
        Point rand;
        while (alreadyAttacked)
        {
            rand = game().randomPoint();
            if (status[rand.r][rand.c] == '.')
                alreadyAttacked = false;
        }
        return rand;
    }
    //state 2
    else
    {
        //check to see if cross has all positions chosen
        bool allPositionsChosen = true;            
        for (int i = 1; i < 5; i++)
        {
            //check up:
            if (game().isValid(Point(middleCross.r + i, middleCross.c)) && status[middleCross.r + i][middleCross.c] == '.')
                allPositionsChosen = false;
            //check down
            if (game().isValid(Point(middleCross.r - i, middleCross.c)) && status[middleCross.r - i][middleCross.c] == '.')
                allPositionsChosen = false;
            //check right
            if (game().isValid(Point(middleCross.r, middleCross.c + i)) && status[middleCross.r][middleCross.c + i] == '.')
                allPositionsChosen = false;
            //check left
            if (game().isValid(Point(middleCross.r, middleCross.c - i)) && status[middleCross.r][middleCross.c - i] == '.')
                allPositionsChosen = false;
        }
        if (allPositionsChosen)
        {
            stateOne = true;
            return recommendAttack();
        }
        Point newAttack;
        bool notValidorAttacked = true;
        while (notValidorAttacked)
        {
            int length = randInt(4) + 1;
            int direction = randInt(4);                                
            //up
            if (direction == 0)
                newAttack = Point(middleCross.r + length, middleCross.c);
            //right
            else if (direction == 1)
                newAttack = Point(middleCross.r, middleCross.c + length);
            //down
            else if (direction == 2)
                newAttack = Point(middleCross.r - length, middleCross.c);
            //right
            else
                newAttack = Point(middleCross.r, middleCross.c - length);
            if (game().isValid(newAttack) && status[newAttack.r][newAttack.c] == '.')
            {
                notValidorAttacked = false;
            }
        }
        return newAttack;
    }
}

//record attack for mediocre
void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId)
{
    //miss
    if (validShot && !shotHit)
        status[p.r][p.c] = 'o';
    //hit
    if (validShot && shotHit)
        status[p.r][p.c] = 'X';
    //damaged state 1
    if (validShot && shotHit && !shipDestroyed && stateOne)
    {
        stateOne = false;
        middleCross = p;
    }
    //destroyed ship state 2 -> opposite of lucky
    if (validShot && shotHit && shipDestroyed && !stateOne)
        stateOne = true;
}

//does nothing
void MediocrePlayer::recordAttackByOpponent(Point /* p */)
{}

//helper for place ship, uses recursion
bool MediocrePlayer::helper(Board& b, int shipIndex)
{
    if (shipIndex == -1)
        return true;
    for (int r = 0; r < game().rows(); r++)
        for (int c = 0; c < game().cols(); c++)
        {
            //can place ship vertically
            if (b.placeShip(Point(r, c), shipIndex, VERTICAL))
            {
                //cannot place rest of ships
                if (!helper(b, shipIndex - 1))
                    b.unplaceShip(Point(r, c), shipIndex, VERTICAL);
                else
                    return true;
            }
            //can place ship horizontally
            if (b.placeShip(Point(r, c), shipIndex, HORIZONTAL))
            {
                //cannot place rest of ships
                if (!helper(b, shipIndex - 1))
                    b.unplaceShip(Point(r, c), shipIndex, HORIZONTAL);
                else
                    return true;
            }
        }
    return false;
}

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public MediocrePlayer
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
private:
    int state = 1;
    Point origin = Point(0, 0);
    Point center = Point(0, 0);
    char status[MAXROWS][MAXCOLS];
    //0 for up,1 for right, 2 for down, 3 for left
    int direction = 0;
    int missCount = 0;
    int loopCounter = 0;
    vector<Point> firstGuess;
    vector<int> shipsLeft;

    bool OneShipPlace(int r, int c, int shipID);
    bool canPlace(int r, int c);
};

//constructor for good player
GoodPlayer::GoodPlayer(string nm, const Game& g)
    : MediocrePlayer(nm, g)
{
    //clears board to be dots
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
            status[r][c] = '.';
    }
    vector<int> x;
    vector<int> y;
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if ((r % 2 == 0 && c % 2 == 0) || (r % 2 == 1 && c % 2 == 1))
            {
                x.push_back(r);
                y.push_back(c);
            }

        }
    }
    //inputs all the points that are in a checkerboard pattern
    for (int i = 0; i < x.size(); i++)
        firstGuess.push_back(Point(x[i], y[i]));
    //inputs all of the ship ids
    for (int i = 0; i < game().nShips(); i++)
        shipsLeft.push_back(i);    
}

//recommend attack for good player
Point GoodPlayer::recommendAttack()
{
    bool greaterThanFour = false;
    timer.start();
    //state 1
    if (state == 1)
    {
        //still have first guesses
        if (!firstGuess.empty())
        {
            int randV = randInt(firstGuess.size());
            Point guess = firstGuess.at(randV);
            if (canPlace(guess.r, guess.c))
            {
                firstGuess.erase(firstGuess.begin() + randV);
                return guess;
            }
            else
            {
                firstGuess.erase(firstGuess.begin() + randV);
                fattack(greaterThanFour);
                if (greaterThanFour)
                {
                    cout << "Fail!";
                }
                return recommendAttack();
            }
                
        }
        //choose random point
        bool alreadyAttacked = true;
        Point rand;
        loopCounter = 0;
        while (alreadyAttacked)
        {
            rand = game().randomPoint();
            if (status[rand.r][rand.c] == '.' && canPlace(rand.r,rand.c))
            {
                alreadyAttacked = false;
            }
            else if (status[rand.r][rand.c] == '.')
            {
                loopCounter++;
                if (loopCounter == 100)
                    alreadyAttacked = false;
            }
        }
        loopCounter = 0;
        fattack(greaterThanFour);
        if (greaterThanFour)
        {
            cout << "Fail!";
        }
        return rand;
    }
    //state 2
    else if (state == 2)
    {
        bool alreadyAttacked = true;
        Point crossAttack;
        while (alreadyAttacked)
        {
            //if all points 1 spot outward cannot be used
            if ((!game().isValid(Point(origin.r - 1, origin.c)) || status[origin.r - 1][origin.c] != '.' || !canPlace(origin.r - 1, origin.c)) &&
                (!game().isValid(Point(origin.r, origin.c + 1)) || status[origin.r][origin.c + 1] != '.' || !canPlace(origin.r, origin.c + 1)) &&
                (!game().isValid(Point(origin.r + 1, origin.c)) || status[origin.r + 1][origin.c] != '.' || !canPlace(origin.r + 1, origin.c)) &&
                (!game().isValid(Point(origin.r, origin.c - 1)) || status[origin.r][origin.c - 1] != '.' || !canPlace(origin.r, origin.c - 1)))
            {
                state = 1;
                return recommendAttack();
            }
            //chooses random direction
            direction = randInt(4);
            //up
            if (direction == 0)
                crossAttack = Point(origin.r - 1, origin.c);
            //right
            else if (direction == 1)
                crossAttack = Point(origin.r, origin.c + 1);
            //down
            else if (direction == 2)
                crossAttack = Point(origin.r + 1, origin.c);
            else
                crossAttack = Point(origin.r, origin.c - 1);
            //attack point is valid and empty and can place ship there
            if (game().isValid(crossAttack) && status[crossAttack.r][crossAttack.c] == '.' && canPlace(crossAttack.r,crossAttack.c))
                alreadyAttacked = false;
        }
        fattack(greaterThanFour);
        if (greaterThanFour)
        {
            cout << "Fail!";
        }
        return crossAttack;
    }
    //state 3
    else
    {
        //depending on direction of state 2, picks next point
        Point straight;
        if (direction == 0)
            straight = Point(origin.r - 1, origin.c);
        //right
        else if (direction == 1)
            straight = Point(origin.r, origin.c + 1);
        //down
        else if (direction == 2)
            straight = Point(origin.r + 1, origin.c);
        else
            straight = Point(origin.r, origin.c - 1);
        //if the next point is not empty or you hit the wall
        if (!game().isValid(straight) || status[straight.r][straight.c] != '.')
        {
            if (direction == 0)
                direction = 2;
            else if (direction == 1)
                direction = 3;
            else if (direction == 2)
                direction = 0;
            else if (direction == 3)
                direction = 1;
            origin = center;
            loopCounter++;
            if (loopCounter == 5)
            {
                state = 1;
                loopCounter = 0;
                return recommendAttack();
            }
            return recommendAttack();
        }
        //if point is valid and empty
        if (game().isValid(straight) && status[straight.r][straight.c] == '.')
            origin = straight;
        fattack(greaterThanFour);
        if (greaterThanFour)
        {
            cout << "Fail!";
        }
        return straight;
    }
}

//record attack results for good player
void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId)
{
    //miss
    if (validShot && !shotHit)
        status[p.r][p.c] = 'o';
    //shot hit
    if (validShot && shotHit)
        status[p.r][p.c] = 'X';
    //ship destroyed
    if (shipDestroyed)
    {
        //removes the id from vector
        for (vector<int>::iterator i = shipsLeft.begin(); i != shipsLeft.end();)
        {
            if (shipId == (*i))
            {
                i = shipsLeft.erase(i);
            }
            else
                i++;
        }
    }
    if (state == 1 && shotHit && !shipDestroyed)
    {
        center = p;
        origin = p;
        state = 2;
    }
    else if (state == 2 && shotHit && shipDestroyed)            
        state = 1;
    else if (state == 2 && shotHit && !shipDestroyed)
    {
        origin = p;
        state = 3;
    }
    else if (state == 3 && !shotHit)
    {
        if (direction == 0)
            direction = 2;
        if (direction == 1)
            direction = 3;
        if (direction == 2)
            direction = 0;
        if (direction == 3)
            direction = 1;
        origin = center;
        missCount++;
        //cannot miss more than twice
        if (missCount == 2)
        {
            state = 1;
            missCount = 0;
        }
    }
    else if (state == 3 && shipDestroyed)
    {
        missCount = 0;
        state = 1;
    }
           
}

//helper function checks if ship of given ship id can be placed at given point
bool GoodPlayer::OneShipPlace(int r, int c, int shipID)
{
    int length = game().shipLength(shipID);
    bool oneRight = false;
    for (int i = -length + 1; i <= 0; i++)
    {
        bool cH = true;
        bool cV = true;
        //all the possible top left points
        Point currentH = Point(r, c + i);
        Point currentV = Point(r + i, c);
        for (int a = 0; a < length; a++)
        {
            //horizontallly
            if (!game().isValid(Point(currentH.r, currentH.c + a)) || status[currentH.r][currentH.c + a] == 'o') 
                cH = false;
            //vertically
            if (!game().isValid(Point(currentV.r + a, currentV.c)) || status[currentV.r + a][currentV.c] == 'o') 
                cV = false;
        }
        //if you can place either horizontally or vertically
        if (cH || cV)
            oneRight = true;
    }
    return oneRight;
}

//checks if you can place at least one unsunken ship at the point given
bool GoodPlayer::canPlace(int r, int c)
{
    for (int i = 0; i < shipsLeft.size(); i++)
    {
        if (OneShipPlace(r, c, shipsLeft.at(i)))
            return true;
    }
    return false;
}


//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
