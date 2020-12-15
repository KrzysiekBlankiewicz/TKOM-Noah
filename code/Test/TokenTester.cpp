#include "TokenTester.h"

std::map<int, std::string> TokenTester::namesOfTokenTypes = {
	{TokenTester::IDENTIFIER, "IDENTIFIER"},
	{TokenTester::KEYWORD, "KEYWORD" },
	{TokenTester::OPERATOR, "OPERATOR"},
	{TokenTester::INT, "INT"},
	{TokenTester::FLOAT, "FLOAT"},
	{TokenTester::INVALID, "INVALID"},
	{TokenTester::EOT,	"EOT"},
	{TokenTester::UNSAFE, "UNSAFE"},
	{TokenTester::UNKNOWN, "UNKNOWN"},
};

int TokenTester::visit(Token* host)
{
	return UNKNOWN;
}

int TokenTester::visit(IdentifierToken* host)
{
	return IDENTIFIER;
}

int TokenTester::visit(KeywordToken* host)
{
	return KEYWORD;
}

int TokenTester::visit(OperatorToken* host)
{
	return OPERATOR;
}


int TokenTester::visit(IntToken* host)
{
	return INT;
}

int TokenTester::visit(FloatToken* host)
{
	return FLOAT;
}

int TokenTester::visit(EOTToken* host)
{
	return EOT;
}

int TokenTester::visit(InvalidToken* host)
{
	return INVALID;
}

int TokenTester::visit(UnsafeToken* host)
{
	return UNSAFE;
}