#pragma once
#include "../Back/Lexer.h"

class LexerTester :
    public Lexer
{
public:
	LexerTester() = delete;		// wo�am metody protected, kt�re wymagaj� �eby source by�o ustawione
	LexerTester(Source* newSource);

	std::optional<std::unique_ptr<Token>> test_produceIdentifier();
	std::optional<std::unique_ptr<Token>> test_produceNumber();
	std::optional<std::unique_ptr<Token>> test_produceOperator();
	std::unique_ptr<Token> test_produceSpecial();

	void test_skipWhiteSpacesAndComments();
	bool test_isOperator(char charToCheck);
	std::optional<int> test_parseIntegerPartFromSource();
	std::optional<double> test_parseFractionalPartFromSource();
};

