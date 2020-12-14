#include "Source.h"


char Source::getCurrentChar()
{
	return currentChar;
}

char Source::getNextChar()
{
	return nextChar;
}

bool Source::isEndOfInput()
{
	return endOfInput;
}
