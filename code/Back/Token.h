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
	std::string content;
public:
	Token();
	Token(std::string newContent);
	std::string getContent();
	virtual int accept(TokenVisitor* visitor);
	virtual std::optional<double> getValue();
};

class IdentifierToken : public Token
{
	//int symbolTableId;
public:
	IdentifierToken();
	IdentifierToken(std::string v/*, int id*/);
	virtual int accept(TokenVisitor* visitor);

};

class KeywordToken : public Token
{
	KeywordType myType;
public:
	KeywordToken();
	KeywordToken(std::string v);
	KeywordToken(KeywordType newType);
	KeywordToken(std::string newContent, KeywordType newType);
	virtual int accept(TokenVisitor* visitor);
};

class OperatorToken : public Token
{
	OperatorType myType;
public:
	OperatorToken();
	OperatorToken(std::string newContent);
	OperatorToken(OperatorType newType);
	OperatorToken(std::string newContent, OperatorType newType);
	virtual int accept(TokenVisitor* visitor);
};

class IntToken : public Token
{
	int value;
public:
	IntToken();
	IntToken(int newValue);
	IntToken(std::string newContent);
	IntToken(std::string newContent, int newValue);
	virtual std::optional<double> getValue();
	virtual int accept(TokenVisitor* visitor);
};

class FloatToken : public Token
{
	double value;
public:
	FloatToken();
	FloatToken(double newValue);	// TODO nie ustawia content
	FloatToken(std::string newContent);		// TODO nie ustawia value
	FloatToken(std::string newContent, double newValue);
	virtual std::optional<double> getValue();
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

