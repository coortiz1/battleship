#include "Game.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <cassert>
#include "Board.h"


using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

int main()
{
    //Game g1(5, 6);
    ////Game g2(11, 6);
    ////Game g3(5, 11);
    ////Game g4(0, 6);
    ////Game g5(6, 0);
    //assert(g1.rows() == 5);
    //assert(g1.cols() == 6);
    //assert(!g1.isValid(Point(-1, -1)));
    //assert(!g1.isValid(Point(6, 1)));
    //assert(!g1.isValid(Point(5, 0)));
    //assert(!g1.isValid(Point(4, 6)));
    //assert(g1.isValid(Point(0,0)));
    //assert(g1.isValid(Point(0,5)));
    //assert(g1.isValid(Point(4,0)));
    //assert(g1.isValid(Point(4,5)));
    //assert(!g1.addShip(-1, 'b', "taco"));
    //assert(!g1.addShip(7, 'A', "aircraft carrier"));
    //assert(!g1.addShip(6, 'X', "taco"));
    //assert(!g1.addShip(6, 'o', "taco"));
    //assert(!g1.addShip(6, '.', "taco"));
    //assert(g1.addShip(3, 'S', "submarine"));
    //assert(g1.addShip(4, 'G', "gunner"));
    ////g1.shipPrint();
    //assert(g1.nShips() == 2);
    ////g1.shipLength(2);
    ////g1.shipLength(-1);
    //assert(g1.shipLength(0) == 3);
    ////g1.shipSymbol(2);
    ////g1.shipSymbol(-1);
    //assert(g1.shipSymbol(1) == 'G');
    ////g1.shipName(2);
    ////g1.shipName(-1);
    //assert(g1.shipName(0) == "submarine");

    //Board b1(g1);
    //b1.block();
    //b1.unblock();
    //assert(!b1.placeShip(Point(0, 0), -1, VERTICAL));
    //assert(!b1.placeShip(Point(0, 0), 2, VERTICAL));
    //assert(!b1.placeShip(Point(2, 0), 1, VERTICAL));
    //assert(!b1.placeShip(Point(0, 4), 1, HORIZONTAL));
    //assert(!b1.placeShip(Point(5, 0), 1, HORIZONTAL));
    //assert(b1.placeShip(Point(1, 1), 1, VERTICAL));
    //assert(!b1.placeShip(Point(1, 0), 0, HORIZONTAL));
    ////b1.block();
    //assert(!b1.placeShip(Point(1, 2), 1, VERTICAL));
    //assert(!b1.unplaceShip(Point(1, 1), 0, VERTICAL));
    //assert(!b1.unplaceShip(Point(1, 1), -1, VERTICAL));
    //assert(!b1.unplaceShip(Point(1, 1), 2, VERTICAL));
    //assert(!b1.unplaceShip(Point(0, 1), 1, VERTICAL));
    //assert(b1.unplaceShip(Point(1, 1), 1, VERTICAL));
    //assert(b1.placeShip(Point(1, 1), 1, VERTICAL));
    //bool shothit;
    //bool shipdestroyed;
    //int shipid;
    //b1.attack(Point(1, 1), shothit, shipdestroyed, shipid);
    //b1.attack(Point(2, 2), shothit, shipdestroyed, shipid);
    //
    //assert(!b1.attack(Point(-1, -1), shothit, shipdestroyed, shipid));
    //assert(!b1.attack(Point(6, 0), shothit, shipdestroyed, shipid));
    //assert(!b1.attack(Point(5, 0), shothit, shipdestroyed, shipid));
    //assert(!b1.attack(Point(4, 6), shothit, shipdestroyed, shipid));
    //assert(!b1.attack(Point(1, 1), shothit, shipdestroyed, shipid));
    //assert(!b1.attack(Point(2, 2), shothit, shipdestroyed, shipid));
    //shipid = 123;
    //shothit = false;
    //shipdestroyed = true;
    //assert(b1.attack(Point(2, 1), shothit, shipdestroyed, shipid));
    //assert(shothit);
    //assert(!shipdestroyed);
    //assert(shipid == 123);
    //shothit = true;
    //assert(b1.attack(Point(0, 5), shothit, shipdestroyed, shipid));
    //assert(!shothit);
    //b1.attack(Point(3, 1), shothit, shipdestroyed, shipid);
    //shipdestroyed = false;
    //shipid = 123;
    //assert(!b1.allShipsDestroyed());
    //b1.attack(Point(4, 1), shothit, shipdestroyed, shipid);
    //assert(shipdestroyed);
    //assert(shipid == 1);
    ////assert(b1.allShipsDestroyed());
    //assert(b1.placeShip(Point(0, 3), 0, VERTICAL));
    //shipdestroyed = false;
    //shipid = 123;
    //b1.attack(Point(0, 3), shothit, shipdestroyed, shipid);
    //b1.attack(Point(1, 3), shothit, shipdestroyed, shipid);
    //b1.attack(Point(2, 3), shothit, shipdestroyed, shipid);
    //assert(shipdestroyed);
    //assert(shipid == 0);
    ////b1.display(false);
    //assert(b1.allShipsDestroyed());
    ////assert(!b1.placeShip(Point(0, 2), 0, VERTICAL));
    //
    //Player* h1 = createPlayer("human", "Bob", g1);
   
    //Game g2(5, 6);
    //Board b2(g2);
    //assert(g2.addShip(3, 'S', "submarine"));
    //assert(g2.addShip(2, 'T', "tank"));
    ////h1->placeShips(b2);
    //delete h1;
    //
    //Game realGame1(6, 6);
    //realGame1.addShip(2, 'G', "gunner");
    //realGame1.addShip(3, 'S', "sub");
    //Player* p1 = createPlayer("human", "Bob", realGame1);
    //Player* p2 = createPlayer("awful", "Chris", realGame1);
    ////realGame1.play(p1, p2);
    //delete p1;
    //delete p2;


    //Game testGame1(10, 10);
    //Board b5(testGame1);
    //addStandardShips(testGame1);
    //Player* p6 = createPlayer("mediocre", "Bob", testGame1);
    //assert(p6->placeShips(b5));
    //delete p6;
    ////b5.display(false);

    //Game uu1(5, 5);
    //Board uu(uu1);
    //uu1.addShip(3, 'K', "killer");
    //uu.placeShip(Point(0, 0), 0, HORIZONTAL);
    ////uu.display(false);
    //bool a;
    //bool b;
    //int c;
    //uu.attack(Point(0, 0), a,b,c);
    //uu.attack(Point(0, 1), a, b, c);
    //uu.attack(Point(0, 2), a, b, c);
    ////uu.display(false);
    //assert(uu.allShipsDestroyed());

    ////Game ppp(10, 10);
    ////addStandardShips(ppp);
    ////Board bbb(ppp);
    ////Player* pp1 = createPlayer("good", "godly", ppp);
    ////pp1->placeShips(bbb);
    //////bbb.display(false);
    ////delete pp1;

    //int counter = 0;
    //for (int i = 0; i < 20; i++)
    //{
    //    Game rr(10, 10);
    //    addStandardShips(rr);
    //    Player* p11 = createPlayer("mediocre", "pro", rr);
    //    Player* p22 = createPlayer("good", "god", rr);
    //    
    //    if (rr.play(p11, p22, false) == p22)
    //    {
    //        counter++;
    //        delete p11;
    //        delete p22;
    //    }
    //}
    //cout << counter << endl;
    //cout << counter / 20.0;


    //Game rr1(10, 10);
    //addStandardShips(rr1);
    //Player* p111 = createPlayer("mediocre", "pro", rr1);
    //Player* p222 = createPlayer("good", "god", rr1);
    //
    //if (rr1.play(p111, p222, false) == p222)
    //{
    //    cout << "win";
    //}
    
    //cout << "Done" << endl;
    
    Game testing1(10, 10);
    addStandardShips(testing1);
    Player* kyle = createPlayer("human", "Kyle the noob", testing1);
    Player* curtis = createPlayer("good", "Curtis the god", testing1);
    testing1.play(kyle, curtis);
    cout << "Done" << endl;

}

//int main()
//{
//    const int NTRIALS = 10;
//
//    cout << "Select one of these choices for an example of the game:" << endl;
//    cout << "  1.  A mini-game between two mediocre players" << endl;
//    cout << "  2.  A mediocre player against a human player" << endl;
//    cout << "  3.  A " << NTRIALS
//         << "-game match between a mediocre and an awful player, with no pauses"
//         << endl;
//    cout << "Enter your choice: ";
//    string line;
//    getline(cin,line);
//    if (line.empty())
//    {
//        cout << "You did not enter a choice" << endl;
//    }
//    else if (line[0] == '1')
//    {
//        Game g(2, 3);
//        g.addShip(2, 'R', "rowboat");
//        Player* p1 = createPlayer("mediocre", "Popeye", g);
//        Player* p2 = createPlayer("mediocre", "Bluto", g);
//        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
//        g.play(p1, p2);
//        delete p1;
//        delete p2;
//    }
//    else if (line[0] == '2')
//    {
//        Game g(10, 10);
//        addStandardShips(g);
//        //new change!!
//        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
//        Player* p2 = createPlayer("human", "Shuman the Human", g);
//        g.play(p1, p2);
//        delete p1;
//        delete p2;
//    }
//    else if (line[0] == '3')
//    {
//        int nMediocreWins = 0;
//
//        for (int k = 1; k <= NTRIALS; k++)
//        {
//            cout << "============================= Game " << k
//                 << " =============================" << endl;
//            Game g(10, 10);
//            addStandardShips(g);
//            Player* p1 = createPlayer("awful", "Awful Audrey", g);
//            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
//            Player* winner = (k % 2 == 1 ?
//                                g.play(p1, p2, false) : g.play(p2, p1, false));
//            if (winner == p2)
//                nMediocreWins++;
//            delete p1;
//            delete p2;
//        }
//        cout << "The mediocre player won " << nMediocreWins << " out of "
//             << NTRIALS << " games." << endl;
//          // We'd expect a mediocre player to win most of the games against
//          // an awful player.  Similarly, a good player should outperform
//          // a mediocre player.
//    }
//    else
//    {
//        cout << "That's not one of the choices." << endl;
//    }
//}

