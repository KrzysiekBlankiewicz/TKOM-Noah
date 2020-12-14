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
    operators_map['!'] = [](char x) { if (x == '=') return std::make_unique<OperatorToken>("!=", OperatorType::NOT_EQUAL); else return std::make_unique<OperatorToken>("=", OperatorType::NOT); };
    operators_map['>'] = [](char x) { if (x == '=') return std::make_unique<OperatorToken>(">=", OperatorType::GREATER_EQUAL); else return std::make_unique<OperatorToken>("=", OperatorType::GREATER); };
}

bool SymbolTable::isOperator(std::string stringToCheck)
{
    if ( std::find(operators.begin(), operators.end(), stringToCheck) != operators.end() )
        return true;
    else
        return false;
}

bool SymbolTable::isKeyword(std::string stringToCheck)
{
    if (std::find(keywords.begin(), keywords.end(), stringToCheck) != keywords.end())
        return true;
    else
        return false;
}

bool SymbolTable::isBeginningOfOperator(char charToCheck)
{
    if (std::find(firstCharsOfOperators.begin(), firstCharsOfOperators.end(), charToCheck) != firstCharsOfOperators.end())
        return true;
    else
        return false;
}

bool SymbolTable::isEndOfOperator(char charToCheck)
{
    if (std::find(secondCharsOfOperators.begin(), secondCharsOfOperators.end(), charToCheck) != secondCharsOfOperators.end())
        return true;
    else
        return false;
}

void SymbolTable::addKeyword(std::string newKeyword)
{
    keywords.insert(newKeyword);
}

void SymbolTable::addOperator(std::string newOpertaor)
{
    if (newOpertaor.length() > 0)
    {
        operators.insert(newOpertaor);
        firstCharsOfOperators.insert(newOpertaor[0]);
        
        if (newOpertaor.length() == 2)
            secondCharsOfOperators.insert(newOpertaor[1]);
    }
}
