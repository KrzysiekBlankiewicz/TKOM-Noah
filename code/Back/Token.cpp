#include "Token.h"

Token::Token()
{
	lineInSource = 0;
	positionInLine = 0;
}

Token::Token(std::string newContent)
{
	content = newContent;
}

Token::Token(int line, int position)
{
	lineInSource = line;
	positionInLine = position;
}

Token::Token(std::string newContent, int line, int position)
{
	content = newContent;
	lineInSource = line;
	positionInLine = position;
}

std::string Token::getContent()
{
	return content;
}

void Token::setPosition(int newLine, int newPosition)
{
	lineInSource = newLine;
	positionInLine = newPosition;
}

int Token::getLine()
{
	return lineInSource;
}

int Token::getPosition()
{
	return positionInLine;
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

IdentifierToken::IdentifierToken(std::string v, int line, int position) : Token(v, line, position)
{
}

int IdentifierToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

KeywordToken::KeywordToken()
{
	myType = KeywordType::NOT_A_KEYWORD;
}

KeywordToken::KeywordToken(std::string newContent, KeywordType newType, int line, int position): Token(newContent, line, position)
{
	myType = newType;
}

std::optional<double> KeywordToken::getValue()
{
	return static_cast<double>(myType);
}

int KeywordToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

OperatorToken::OperatorToken()
{
	myType = OperatorType::NOT_A_OPERATOR;
}

OperatorToken::OperatorToken(std::string newContent, OperatorType newType): Token(newContent)
{
	myType = newType;
}

OperatorToken::OperatorToken(std::string newContent, OperatorType newType, int line, int position): Token(newContent, line, position)
{
	myType = newType;
}

int OperatorToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

IntToken::IntToken()
{
	value = 0;
}

IntToken::IntToken(std::string newContent, int newValue, int line, int position): Token(newContent, line, position)
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
	value = 0.0;
}

FloatToken::FloatToken(std::string newContent, double newValue, int line, int position): Token(newContent, line, position)
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

EOTToken::EOTToken(int line, int position): Token(line, position)
{
}

int EOTToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

InvalidToken::InvalidToken()
{
}

InvalidToken::InvalidToken(int line, int position): Token(line, position)
{
}

int InvalidToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}

UnsafeToken::UnsafeToken()
{
}

UnsafeToken::UnsafeToken(int line, int position): Token(line, position)
{
}

int UnsafeToken::accept(TokenVisitor* visitor)
{
	return visitor->visit(this);
}








