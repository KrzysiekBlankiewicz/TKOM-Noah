#include "Token.h"

int Token::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}


IdentifierToken::IdentifierToken()
{
}

IdentifierToken::IdentifierToken(std::string v/*, int id*/)
{
	value = v;
	//symbolTableId = id;
}

int IdentifierToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

KeywordToken::KeywordToken()
{
}

KeywordToken::KeywordToken(std::string v)
{
	value = v;
	// TODO emun z mapy
}

KeywordToken::KeywordToken(KeywordType newType)
{
	myType = newType;
}

KeywordToken::KeywordToken(KeywordType newType, std::string newValue)
{
	myType = newType;
	value = newValue;
}

int KeywordToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

OperatorToken::OperatorToken()
{
}

OperatorToken::OperatorToken(std::string v)
{
	value = v;
}

OperatorToken::OperatorToken(OperatorType newType)
{
	myType = newType;
}

OperatorToken::OperatorToken(std::string newValue, OperatorType newType)
{
	myType = newType;
	value = newValue;
}

int OperatorToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

IntToken::IntToken()
{
}

IntToken::IntToken(int v)
{
	value = v;
}

int IntToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

FloatToken::FloatToken()
{
}

FloatToken::FloatToken(double v)
{
	value = v;
}

int FloatToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

EOTToken::EOTToken()
{

}

int EOTToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

InvalidToken::InvalidToken()
{

}

int InvalidToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

UnsafeToken::UnsafeToken()
{
}

int UnsafeToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}








