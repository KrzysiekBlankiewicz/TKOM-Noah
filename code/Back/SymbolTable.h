#pragma once
#include <set>
#include <map>
#include <string>
#include <functional>
#include "Token.h"

class Token;

enum class KeywordType
{
	IF_KEYWORD, FOR_KEYWORD, RETURN_KEYWORD
};
enum class OperatorType
{
	EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, ASSIGN, NOT
};

class SymbolTable
{
public:

private:
	std::map<std::string, KeywordType> keywords_map;
	std::map<char, std::function<std::unique_ptr<Token>(char)>> operators_map;

	std::set<std::string> keywords;
	std::set<std::string> operators;

	std::set<char> firstCharsOfOperators;
	std::set<char> secondCharsOfOperators;

public:
	SymbolTable();
	void loadStandardKeywords();
	void loadStandardOperators();
	bool isOperator(std::string stringToCheck);
	bool isKeyword(std::string stringToCheck);
	bool isBeginningOfOperator(char charToCheck);
	bool isEndOfOperator(char charToCheck);

	void addKeyword(std::string newKeyword);
	void addOperator(std::string newOpertaor);
};
