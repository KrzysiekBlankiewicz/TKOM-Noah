#pragma once
#include <string>

class Source
{
protected :
	char currentChar;
	char nextChar;
	bool endOfInput;	
	int currentLine;
	int currentPosition;
public:

	char getCurrentChar();
	char getNextChar();
	bool isEndOfInput();
	int getCurrentLine();
	int getCurrentPosition();

	virtual void processOneChar() = 0;

	static const char CHAR_AFTER_INPUT_END = '@';	// dowolny char spoza jêzyka
};

