#include "TokenTester.h"

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