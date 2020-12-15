#include "Lexer.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <optional>

Lexer::Lexer()
{
    mySource = nullptr;
    currentToken = nullptr;
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

    //TODO zatrzasn¹æ pos tokenu 
    
    if ((returnedToken = produceIdentifier()) != std::nullopt)
        currentToken = std::move(*returnedToken);

    /*
    else if ((returnedToken = produceNumber()) != std::nullopt)
        currentToken = std::move(*returnedToken);

    else if ((returnedToken = produceOperator()) != std::nullopt)
        currentToken = std::move(*returnedToken);

    else 
        currentToken = produceSpecial();
    */

    // TODO czasem pierwsze produce zwróc¹ nullopt, chocia¿ na wierzchu jest sensowny char - mimo to trzeba zrobiæ jednego invalidTokena (np. wiod¹ce zera)
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
            return std::make_unique<UnsafeToken>();
        }

    } while ( isalnum(tempChar) || tempChar == '_');

    std::optional<KeywordType> resultType;
    if ( (resultType = predefinedSymbols.getKeywordType(result)) != std::nullopt )
        return std::make_unique<KeywordToken>(result, resultType.value());
    else
        return std::make_unique<IdentifierToken>(result);
}


std::optional<std::unique_ptr<Token>> Lexer::produceNumber()
{
    if (!isdigit(mySource->getCurrentChar()))
        return std::nullopt;

    std::optional<int> optionalIntegerPart;
    int integralPart;
    std::optional<double> optionalFractionalPart;
    double fractionalPart;

    if ((optionalIntegerPart = parseIntegerPartFromSource()) == std::nullopt)
        return std::nullopt;
    else
        integralPart = *(optionalIntegerPart);

    if (mySource->getCurrentChar() == '.' && isdigit( mySource->getNextChar() ) )
    {
        mySource->processOneChar();
        if ((optionalFractionalPart = parseFractionalPartFromSource()) == std::nullopt)
            return std::nullopt;
        else
            fractionalPart = *(optionalFractionalPart);

        double result = integralPart + fractionalPart;
        return std::make_unique<FloatToken>(std::to_string(result), result);
    }
    else
        return std::make_unique<IntToken>(std::to_string(integralPart), integralPart);
}


std::optional<int> Lexer::parseIntegerPartFromSource()
{
    int result = 0, tempResult = 0;
    char tempChar = mySource->getCurrentChar();

    if (!isdigit(tempChar))
        return std::nullopt;

    if ( tempChar == '0' && isdigit(mySource->getNextChar()) )
    {
        mySource->processOneChar();     // Wywo³ujê processOneChar() ¿eby lexer nie zapêtli³ siê na tym zerze. Potraktuje je jako invalid i bêdzie analizowa³ dalej
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

    std::optional<int> optFractionalPart;
    int fractionalPart;
    if ((optFractionalPart = parseIntegerPartFromSource()) == std::nullopt)
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

    std::optional<std::unique_ptr<Token>> result = predefinedSymbols.getOperatorToken(firstChar, secondChar);   // TODO tak mo¿na zrobiæ w wielu miejscach - czêsto wrzuca³em to bez sensu do ifa
    
    if (!result.has_value())
        return std::nullopt;

    std::string operatorContent = result.value().get()->getContent();
    int operatorContentLength = operatorContent.length();
    if (operatorContentLength == 1)
    {
        mySource->processOneChar();
        return result;
    }
    else if (operatorContentLength == 2)
    {
        mySource->processOneChar();
        mySource->processOneChar();
        return result;
    }
    else
    {
        return std::nullopt;        // wydaje siê, ¿e to nie ma prawa wyst¹piæ
        mySource->processOneChar(); // ¿eby nie zapêtliæ analizy leksykalnej w podejrzanym miejscu
    }
}

/*
bool Lexer::produceSpecial()
{
    if (mySource->isEndOfInput())
    {
        currentToken = new EOTToken();
        return true;
    }
    else {
        mySource->processOneChar();
        currentToken = new InvalidToken();
        return true;
    }
}

*/
// TODO odkomentowywaæ sukcesuwnie pisz¹c testy


std::unique_ptr<Token> Lexer::nextToken()
{
    if (mySource != nullptr)
        produceToken();
    else
        currentToken = std::make_unique<InvalidToken>();

    return std::move(currentToken); // TODO nie mam ju¿ potem dostêpu do currentToken... sprwadziæ, czy to nie szkodzi
}

SymbolTable* Lexer::getSymbolTable()
{
    return &predefinedSymbols;
}

void Lexer::loadStuffIntoSymbolTable()
{
    predefinedSymbols.loadStandardKeywords();
    predefinedSymbols.loadStandardOperators();
}
