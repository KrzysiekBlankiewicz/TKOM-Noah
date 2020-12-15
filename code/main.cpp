
#include <iostream>
#include <iomanip>

#include"Back/StringSource.h"
#include "Back/Lexer.h"
#include "Test/TokenTester.h"


int main()
{
	std::string s = "abc 3 fr \n 00.05 #y\nbj# a";

	//std::cin >> s;

	StringSource source(s);
	Lexer l;
	TokenTester t;
	l.setSource(&source);
	l.loadStuffIntoSymbolTable();

	std::unique_ptr<Token> token;
	std::string tokenType;

	do
	{
		token = l.nextToken();

		if (token == nullptr)
		{
			std::cout << "error - lexer returned nullptr" << std::endl;
			break;
		}
		
		tokenType = TokenTester::namesOfTokenTypes[token->accept(&t)];
		std::cout << std::setw(15) << std::left << tokenType << 
						std::setw(10) << std::left << token->getContent() << 
						" Line: " << token->getLine() << 
						" Position: " << token->getPosition() << std::endl;

	} while (token->accept(&t) != t.EOT);
	
	return 0;
}