#include "utility.h"
#include <string>
using namespace std;

Ship::Ship(int l, int i, char s, string n) : sLength(l),sId(i),sSymbol(s),sName(n)
{}

//basic get functions for class ship
int Ship::length() const
{
	return sLength;
}

int Ship::id() const
{
	return sId;
}

char Ship::symbol() const
{
	return sSymbol;
}

string Ship::name() const
{
	return sName;
}
