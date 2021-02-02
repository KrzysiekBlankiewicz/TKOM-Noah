#pragma once
#include <string>
#include "TokenVisitor.h"
#include "SymbolTable.h"

class TokenVisitor;
class SymbolTable;
enum class KeywordType;
enum class OperatorType;

class Token
{
	std::string content;
	int lineInSource;
	int positionInLine;
	// dodatkowo bezwzgl. pozycja
public:
	Token();
	Token(std::string newContent);
	Token(int line, int position);
	Token(std::string newContent, int line, int position);
	std::string getContent();
	void setPosition(int newLine, int newPosition);
	int getLine();
	int getPosition();

	virtual int accept(TokenVisitor* visitor);
	virtual std::optional<double> getValue();
};

class IdentifierToken : public Token
{
	
public:
	IdentifierToken();
	IdentifierToken(std::string v, int line, int position);
	virtual int accept(TokenVisitor* visitor);

};

class KeywordToken : public Token
{
	KeywordType myType;
public:
	KeywordToken();
	KeywordToken(std::string newContent, KeywordType newType, int line, int position);
	virtual std::optional<double> getValue();
	virtual int accept(TokenVisitor* visitor);
};

class OperatorToken : public Token
{
	OperatorType myType;
public:
	OperatorToken();
	OperatorToken(std::string newContent, OperatorType newType);
	OperatorToken(std::string newContent, OperatorType newType, int line, int position);
	virtual int accept(TokenVisitor* visitor);
};

class IntToken : public Token
{
	int value;
public:
	IntToken();
	IntToken(std::string newContent, int newValue, int line, int position);
	virtual std::optional<double> getValue();
	virtual int accept(TokenVisitor* visitor);
};

class FloatToken : public Token
{
	double value;
public:
	FloatToken();
	FloatToken(std::string newContent, double newValue, int line, int position);
	virtual std::optional<double> getValue();
	virtual int accept(TokenVisitor* visitor);
};

class EOTToken : public Token
{
public:
	EOTToken();
	EOTToken(int line, int position);
	virtual int accept(TokenVisitor* visitor);
};

class InvalidToken : public Token
{
public:
	InvalidToken();
	InvalidToken(int line, int position);
	virtual int accept(TokenVisitor* visitor);
};

class UnsafeToken : public Token
{
public:
	UnsafeToken();
	UnsafeToken(int line, int position);
	virtual int accept(TokenVisitor* visitor);
};

