#pragma once
#include "Token.h"

class Token;
class IdentifierToken;
class KeywordToken;
class OperatorToken;
class IntToken;
class FloatToken;
class EOTToken;
class InvalidToken;
class UnsafeToken;

class TokenVisitor
{
public:
	virtual int visit(Token* host) = 0;
	virtual int visit(IdentifierToken* host) = 0;
	virtual int visit(KeywordToken* host) = 0;
	virtual int visit(OperatorToken* host) = 0;
	virtual int visit(IntToken* host) = 0;
	virtual int visit(FloatToken* host) = 0;
	virtual int visit(EOTToken* host) = 0;
	virtual int visit(InvalidToken* host) = 0;
	virtual int visit(UnsafeToken* host) = 0;
};