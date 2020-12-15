#pragma once
#include <set>
#include <map>
#include <string>
#include <functional>
#include <optional>
#include "Token.h"

class Token;

enum class KeywordType
{
	NOT_A_KEYWORD = 0, IF_KEYWORD, FOR_KEYWORD, RETURN_KEYWORD
};
enum class OperatorType
{
	NOT_A_OPERATOR = 0, EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, ASSIGN, NOT
};

class SymbolTable
{
public:

private:
	std::map<std::string, KeywordType> keywords_map;
	std::map<char, std::function<std::unique_ptr<Token>(char)>> operators_map;

public:
	SymbolTable();
	void loadStandardKeywords();
	void loadStandardOperators();
	
	std::optional<KeywordType> getKeywordType(std::string keywordInString);
	std::optional<std::unique_ptr<Token>> getOperatorToken(char currentChar, char followingChar);
};
