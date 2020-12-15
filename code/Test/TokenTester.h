#pragma once
#include "../Back/TokenVisitor.h"

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
	static const int INT = 4;
	static const int FLOAT = 5;
	static const int EOT = 6;
	static const int INVALID = 7;
	static const int UNSAFE = 8;
	static const int UNKNOWN = 9;

	static std::map<int, std::string> namesOfTokenTypes;
};