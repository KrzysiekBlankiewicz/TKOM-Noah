#pragma once
#include <string>
#include "TokenTester.h"
#include "SymbolTable.h"

class TokenVisitor;
class SymbolTable;
enum class KeywordType;
enum class OperatorType;

class Token
{
public:
	virtual int accept(TokenVisitor* visitor);
};

class IdentifierToken : public Token
{
	std::string value;
	//int symbolTableId;
public:
	IdentifierToken();
	IdentifierToken(std::string v/*, int id*/);
	virtual int accept(TokenVisitor* visitor);

};

class KeywordToken : public Token
{
	std::string value;
	KeywordType myType;
public:
	KeywordToken();
	KeywordToken(std::string v);
	KeywordToken(KeywordType newType);
	KeywordToken(KeywordType newType, std::string newValue);
	virtual int accept(TokenVisitor* visitor);
};

class OperatorToken : public Token
{
	std::string value;
	OperatorType myType;
public:
	OperatorToken();
	OperatorToken(std::string v);
	OperatorToken(OperatorType newType);
	OperatorToken(std::string newValue, OperatorType newType);
	virtual int accept(TokenVisitor* visitor);
};

class IntToken : public Token
{
	int value;
public:
	IntToken();
	IntToken(int v);
	virtual int accept(TokenVisitor* visitor);
};

class FloatToken : public Token
{
	double value;
public:
	FloatToken();
	FloatToken(double v);
	virtual int accept(TokenVisitor* visitor);
};

class EOTToken : public Token
{
public:
	EOTToken();
	virtual int accept(TokenVisitor* visitor);
};

class InvalidToken : public Token
{
public:
	InvalidToken();
	virtual int accept(TokenVisitor* visitor);
};

class UnsafeToken : public Token
{
public:
	UnsafeToken();
	virtual int accept(TokenVisitor* visitor);
};

