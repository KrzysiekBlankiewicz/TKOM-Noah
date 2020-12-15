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

	static const char CHAR_AFTER_INPUT_END = '@';	// dowolny char spoza jêzyka
};

