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

// TODO dodatkowa klasa abstrakcji w wizytorze tokenów

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

class TokenTester : public TokenVisitor
{
public:

	virtual int visit(Token* host);
	virtual int visit(IdentifierToken* host);
	virtual int visit(KeywordToken* host);
	virtual int visit(OperatorToken* host);
	virtual int visit(IntToken* host);
	virtual int visit(FloatToken* host);
	virtual int visit(EOTToken* host);
	virtual int visit(InvalidToken* host);
	virtual int visit(UnsafeToken* host);

	static const int IDENTIFIER = 0;
	static const int KEYWORD = 1;
	static const int OPERATOR = 2;
	static const int SEPARATOR = 3;
	static const int INT = 4;
	static const int FLOAT = 5;
	static const int EOT = 6;
	static const int INVALID = 7;
	static const int UNSAFE = 8;
	static const int UNKNOWN = 9;
};