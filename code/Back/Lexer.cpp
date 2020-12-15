
#include <algorithm>
#include <iostream>
#include <math.h>
#include <optional>
#include "Lexer.h"

Lexer::Lexer()
{
    mySource = nullptr;
    currentToken = nullptr;
    currentTokenLineInSource = 0;
    currentTokenPositionInLine = 0;
}

Lexer::Lexer(Source* newSource)
{
    mySource = newSource;
    currentToken = nullptr;
}

void Lexer::setSource(Source* newSource)
{
    mySource = newSource;
}



void Lexer::produceToken()
{
    std::optional<std::unique_ptr<Token>> returnedToken = std::nullopt;

    skipWhiteSpacesAndComments();

    currentTokenLineInSource = mySource->getCurrentLine();
    currentTokenPositionInLine = mySource->getCurrentPosition();
    
    if ((returnedToken = produceIdentifier()) != std::nullopt)
        currentToken = std::move(*returnedToken);

    else if ((returnedToken = produceNumber()) != std::nullopt)
        currentToken = std::move(*returnedToken);

    else if ((returnedToken = produceOperator()) != std::nullopt)
        currentToken = std::move(*returnedToken);

    else 
        currentToken = produceSpecial();
}

void Lexer::skipWhiteSpacesAndComments()
{   
    char currChar = mySource->getCurrentChar();
    while (isspace(currChar) || currChar == '#')    // komentarze zaczynaj¹ siê i koñcz¹ '#'
    {
        if (currChar != '#')
        {
            mySource->processOneChar();
            currChar = mySource->getCurrentChar();
        }
        else
        {
            do {
                mySource->processOneChar();
                currChar = mySource->getCurrentChar();
            } while ( ( mySource->getCurrentChar() != '#') && !( mySource->isEndOfInput() ) );  // na wypadek, jakby ktoœ nie zamnk¹³ komentarza

            // przejad³em ca³y komentarz, teraz muszê przejœæ nad znakiem koñca komentarza:
            mySource->processOneChar();
            currChar = mySource->getCurrentChar();
        }
    }
}



std::optional<std::unique_ptr<Token>> Lexer::produceIdentifier()
{
    if (!isalpha(mySource->getCurrentChar()))
        return std::nullopt;

    std::string result = "";
    char tempChar = mySource->getCurrentChar();

    do {
        result += tempChar;
        mySource->processOneChar();
        tempChar = mySource->getCurrentChar();
        
        if (result.length() > MAX_IDENTIFIER_LENGTH)
        {
            return std::make_unique<UnsafeToken>(currentTokenLineInSource, currentTokenPositionInLine);
        }

    } while ( isalnum(tempChar) || tempChar == '_');

    std::optional<KeywordType> resultType = predefinedSymbols.getKeywordType(result);
    if ( resultType != std::nullopt )
        return std::make_unique<KeywordToken>(result, resultType.value(), currentTokenLineInSource, currentTokenPositionInLine);
    else
        return std::make_unique<IdentifierToken>(result, currentTokenLineInSource, currentTokenPositionInLine);
}

std::optional<std::unique_ptr<Token>> Lexer::produceNumber()
{
    if (!isdigit(mySource->getCurrentChar()))
        return std::nullopt;

    int integralPart;
    double fractionalPart;

    std::optional<int> optionalIntegerPart = parseIntegerPartFromSource();
    if (optionalIntegerPart == std::nullopt)
        return std::nullopt;
    else
        integralPart = *(optionalIntegerPart);

    if (mySource->getCurrentChar() == '.' && isdigit( mySource->getNextChar() ) )
    {
        mySource->processOneChar();
        std::optional<double> optionalFractionalPart = parseFractionalPartFromSource();
        if (optionalFractionalPart == std::nullopt)
            return std::nullopt;
        else
            fractionalPart = *(optionalFractionalPart);

        double result = integralPart + fractionalPart;
        return std::make_unique<FloatToken>(std::to_string(result), result, currentTokenLineInSource, currentTokenPositionInLine);
    }
    else
        return std::make_unique<IntToken>(std::to_string(integralPart), integralPart, currentTokenLineInSource, currentTokenPositionInLine);
}

std::optional<int> Lexer::parseIntegerPartFromSource()
{
    int result = 0, tempResult = 0;
    char tempChar = mySource->getCurrentChar();

    if (!isdigit(tempChar))
        return std::nullopt;

    if ( tempChar == '0' && isdigit(mySource->getNextChar()) )
    {
        // dobrze by by³o wywo³aæ processOneChar() ¿eby lexer nie zapêtli³ siê na tym wiod¹cym zerze, zrobi to produceSpecial()
        return std::nullopt;
    }

    do {
        tempResult = result * 10 + ((int)tempChar - (int)'0');
        if (tempResult < result)
            return std::nullopt;       // to oznacza przekroczenie zakresu; w³aœciwie niekoniecznie, ale to jedyne co mogê sprawdziæ
        result = tempResult;
        mySource->processOneChar();
        tempChar = mySource->getCurrentChar();
    } while (isdigit(tempChar));

    return result;
}

std::optional<double> Lexer::parseFractionalPartFromSource()
{
    char tempChar = mySource->getCurrentChar();

    if (!isdigit(tempChar))
        return std::nullopt;

    int numberOfLeadingZeros = 0;
    while (tempChar == '0')
    {
        ++numberOfLeadingZeros;
        mySource->processOneChar();
        tempChar = mySource->getCurrentChar();
    }

    if (!isdigit(tempChar))
        return 0.0;

    int fractionalPart;
    std::optional<int> optFractionalPart = parseIntegerPartFromSource();
    if (optFractionalPart == std::nullopt)
        return std::nullopt;
    else
        fractionalPart = *optFractionalPart;

    int length = (floor(log10((float)fractionalPart)) + 1) + numberOfLeadingZeros;
    double x = fractionalPart / pow(10, length);

    return x;
}

std::optional<std::unique_ptr<Token>> Lexer::produceOperator()
{
    char firstChar = mySource->getCurrentChar();
    char secondChar = mySource->getNextChar();

    std::optional<std::unique_ptr<Token>> result = predefinedSymbols.getOperatorToken(firstChar, secondChar);
    
    if (!result.has_value())
        return std::nullopt;

    std::string operatorContent = result.value().get()->getContent();
    int operatorContentLength = operatorContent.length();
    if (operatorContentLength == 1)
    {
        mySource->processOneChar();
        result.value().get()->setPosition(currentTokenLineInSource, currentTokenPositionInLine);
        return result;
    }
    else if (operatorContentLength == 2)
    {
        mySource->processOneChar();
        mySource->processOneChar();
        result.value().get()->setPosition(currentTokenLineInSource, currentTokenPositionInLine);
        return result;
    }
    else
    {
        return std::nullopt;
    }
}

std::unique_ptr<Token> Lexer::produceSpecial()
{
    if (mySource->isEndOfInput())
    {
        return std::make_unique<EOTToken>(currentTokenLineInSource, currentTokenPositionInLine);
    }
    else {
        mySource->processOneChar();
        return std::make_unique<InvalidToken>(currentTokenLineInSource, currentTokenPositionInLine);
    }
    return nullptr;
}



std::unique_ptr<Token> Lexer::nextToken()
{
    if (mySource != nullptr)
        produceToken();
    else
        currentToken = std::make_unique<UnsafeToken>(0,0);

    return std::move(currentToken);
}

void Lexer::loadStuffIntoSymbolTable()
{
    predefinedSymbols.loadStandardKeywords();
    predefinedSymbols.loadStandardOperators();
}
