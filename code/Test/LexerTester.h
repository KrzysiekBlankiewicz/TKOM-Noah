#pragma once
#include "../Back/Lexer.h"

class LexerTester :
    public Lexer
{
public:
	LexerTester() = delete;
	LexerTester(Source* newSource);

	std::optional<std::unique_ptr<Token>> test_produceIdentifier();
	/*std::optional<std::unique_ptr<Token>> test_produceNumber();
	std::optional<std::unique_ptr<Token>> test_produceOperator();
	std::unique_ptr<Token> test_produceSpecial();
	*/
	// TODO odkomentowywaæ sukcesuwnie pisz¹c testy

	void test_skipWhiteSpacesAndComments();
	bool test_isOperator(char charToCheck);
	int test_parseSingleNumberFromSource();
};

