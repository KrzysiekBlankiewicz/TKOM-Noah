#include "StringSource.h"
#include <fstream>

StringSource::StringSource(std::string newString)
{
	myString = newString;
	stringLength = newString.length();
	nextCharPositionInString = 1;
	endOfInput = false;
	currentLine = currentPosition = 1;

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
	bool areWeMovingToNextLine = (currentChar == '\n');

	currentChar = nextChar;
	
	if (nextCharPositionInString < stringLength - 1 && nextCharPositionInString >= 0)
	{
		nextChar = myString[nextCharPositionInString + 1];
		++nextCharPositionInString;
		++currentPosition;
		if (areWeMovingToNextLine)
		{
			++currentLine;
			currentPosition = 1;
		}
	}
	else if (nextCharPositionInString == stringLength - 1)
	{
		nextChar = CHAR_AFTER_INPUT_END;
		++nextCharPositionInString;
		++currentPosition;
	}
	else if (nextCharPositionInString == stringLength)
	{
		nextChar = CHAR_AFTER_INPUT_END;
		++currentPosition;
		endOfInput = true;
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
