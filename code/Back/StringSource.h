#pragma once
#include "Source.h"

class StringSource :
    public Source
{
    std::string myString;
    int nextCharPositionInString;
    int stringLength;

public:
    StringSource(std::string newString);
    StringSource() = delete;
    void processOneChar();
    std::string getMyString();

    static const char CHAR_AFTER_INPUT_END = '@';	// dowolny char spoza jêzyka
};
