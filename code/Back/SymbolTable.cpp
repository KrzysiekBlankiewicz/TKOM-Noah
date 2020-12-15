#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
}

void SymbolTable::loadStandardKeywords()
{
    keywords_map["if"] = KeywordType::IF_KEYWORD;
    keywords_map["for"] = KeywordType::FOR_KEYWORD;
    keywords_map["return"] = KeywordType::RETURN_KEYWORD;
}

void SymbolTable::loadStandardOperators()
{
    operators_map['='] = [](char x) { if (x == '=') return std::make_unique<OperatorToken>("==", OperatorType::EQUAL); else return std::make_unique<OperatorToken>("=", OperatorType::ASSIGN); };
    operators_map['!'] = [](char x) { if (x == '=') return std::make_unique<OperatorToken>("!=", OperatorType::NOT_EQUAL); else return std::make_unique<OperatorToken>("!", OperatorType::NOT); };
    operators_map['>'] = [](char x) { if (x == '=') return std::make_unique<OperatorToken>(">=", OperatorType::GREATER_EQUAL); else return std::make_unique<OperatorToken>(">", OperatorType::GREATER); };
}


std::optional<KeywordType> SymbolTable::getKeywordType(std::string keywordInString)
{
    if (keywords_map.count(keywordInString) > 0)
        return keywords_map[keywordInString];
    else
        return std::nullopt;
}

std::optional<std::unique_ptr<Token>> SymbolTable::getOperatorToken(char currentChar, char followingChar)
{
    if (operators_map.count(currentChar) > 0)
        return operators_map[currentChar](followingChar);
    else
        return std::nullopt;
}