#include "StringSource.h"
#include <fstream>

StringSource::StringSource(std::string newString)
{
	myString = newString;
	stringLength = newString.length();
	nextCharPositionInString = 1;
	endOfInput = false;

	if (stringLength < 1)
	{
		endOfInput = true;
		currentChar = CHAR_AFTER_INPUT_END;
		nextChar = CHAR_AFTER_INPUT_END;
	}
	else if (stringLength == 1)
	{
		currentChar = myString[0];
		nextChar = CHAR_AFTER_INPUT_END;
	}
	else
	{
		currentChar = myString[0];
		nextChar = myString[1];
	}
}

void StringSource::processOneChar()
{
	currentChar = nextChar;
	if (nextCharPositionInString < stringLength - 1 && nextCharPositionInString >= 0)
	{
		nextChar = myString[nextCharPositionInString + 1];
		++nextCharPositionInString;
	}
	else if (nextCharPositionInString == stringLength - 1)
	{
		nextChar = CHAR_AFTER_INPUT_END;
		++nextCharPositionInString;
	}
	else if (nextCharPositionInString > stringLength - 1)
	{
		nextChar = CHAR_AFTER_INPUT_END;
		endOfInput = true;
	}
}

std::string StringSource::getMyString()
{
	return myString;
}
