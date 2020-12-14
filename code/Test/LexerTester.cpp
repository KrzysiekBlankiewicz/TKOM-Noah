#include "LexerTester.h"

LexerTester::LexerTester(Source* newSource) : Lexer(newSource)
{
}

std::optional<std::unique_ptr<Token>> LexerTester::test_produceIdentifier()
{
    return this->produceIdentifier();
}

/*
std::optional<std::unique_ptr<Token>> LexerTester::test_produceNumber()
{
    return this->produceNumber();
}

std::optional<std::unique_ptr<Token>> LexerTester::test_produceOperator()
{
    return this->produceOperator();
}

std::unique_ptr<Token> LexerTester::test_produceSpecial()
{
    return this->produceSpecial();
}
*/
// TODO odkomentowywaæ sukcesuwnie pisz¹c testy

void LexerTester::test_skipWhiteSpacesAndComments()
{
}

bool LexerTester::test_isOperator(char charToCheck)
{
    return false;
}

int LexerTester::test_parseSingleNumberFromSource()
{
    return 0;
}
