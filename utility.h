#ifndef UTILITY_H
#define UTILITY_H
#include "globals.h"
#include <string>
class Ship
{
public:
	Ship(int l, int i, char s, std::string n);
	int length() const;
	int id() const;
	char symbol() const;
	std::string name() const;
private:
	int sLength;
	int sId;
	char sSymbol;
	std::string sName;
};
#endif

