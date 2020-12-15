#include "Token.h"

Token::Token()
{
}

Token::Token(std::string newContent)
{
	content = newContent;
}

std::string Token::getContent()
{
	return content;
}

int Token::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

std::optional<double> Token::getValue()
{
	return std::nullopt;
}


IdentifierToken::IdentifierToken()
{
}

IdentifierToken::IdentifierToken(std::string v/*, int id*/) : Token(v)
{
	//symbolTableId = id;
}

int IdentifierToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

KeywordToken::KeywordToken()
{
}

KeywordToken::KeywordToken(std::string v): Token(v)
{

}

KeywordToken::KeywordToken(KeywordType newType)
{
	myType = newType;
}

KeywordToken::KeywordToken(std::string newContent, KeywordType newType): Token(newContent)
{
	myType = newType;
}

int KeywordToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

OperatorToken::OperatorToken()
{
}

OperatorToken::OperatorToken(std::string newContent): Token(newContent)
{

}

OperatorToken::OperatorToken(OperatorType newType)
{
	myType = newType;
}

OperatorToken::OperatorToken(std::string newContent, OperatorType newType): Token(newContent)
{
	myType = newType;
}

int OperatorToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

IntToken::IntToken()
{
}

IntToken::IntToken(int newValue): Token(std::to_string(newValue))
{
	value = newValue;
}

IntToken::IntToken(std::string newContent): Token(newContent)
{
	value = std::stoi(newContent);
}

IntToken::IntToken(std::string newContent, int newValue): Token(newContent)
{
	value = newValue;
}

std::optional<double> IntToken::getValue()
{
	return (double) value;
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

FloatToken::FloatToken(std::string newContent): Token(newContent)
{
}

FloatToken::FloatToken(std::string newContent, double newValue): Token(newContent)
{
	value = newValue;
}

std::optional<double> FloatToken::getValue()
{
	return value;
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








