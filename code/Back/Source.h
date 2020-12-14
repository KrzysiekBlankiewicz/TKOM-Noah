#pragma once
#include <string>

class Source
{
protected :
	char currentChar;
	char nextChar;
	bool endOfInput;	

public:

	char getCurrentChar();
	char getNextChar();
	bool isEndOfInput();
	virtual void processOneChar() = 0;
};

