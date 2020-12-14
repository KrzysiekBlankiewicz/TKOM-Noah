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

void Lexer::loadStandardOperators()
{
    predefinedSymbols.addOperator("+");
    predefinedSymbols.addOperator("-");
    predefinedSymbols.addOperator("*");
    predefinedSymbols.addOperator("/");
    predefinedSymbols.addOperator("[");
    predefinedSymbols.addOperator("]");
    predefinedSymbols.addOperator(".");
    predefinedSymbols.addOperator("==");
    predefinedSymbols.addOperator(">");
    predefinedSymbols.addOperator("<");
    predefinedSymbols.addOperator(">=");
    predefinedSymbols.addOperator("<=");
    predefinedSymbols.addOperator("!=");
    predefinedSymbols.addOperator("!");

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
            } while ( ( mySource->getCurrentChar() != '#') && !( mySource->isEndOfInput() ) );  // na wypadek, jakby ktoœ nie zamnk¹³ komentarza
        }
    }
}

std::optional<std::unique_ptr<Token>> Lexer::produceIdentifier() //TODO jako optional. I reszta metod te¿
{
    if (!isalpha(mySource->getCurrentChar()))
        return std::nullopt;

    std::string result = "";
    char tempChar = mySource->getCurrentChar();

    do {
        result += tempChar;
        mySource->processOneChar();
        tempChar = mySource->getCurrentChar();
        
        if (result.length() == MAX_IDENTIFIER_LENGTH)
        {
            return std::make_unique<UnsafeToken>();
        }

    } while ( isalnum(tempChar) || tempChar == '_');

    if (predefinedSymbols.isKeyword(result))
        return std::make_unique<KeywordToken>();
    else
        return std::make_unique<IdentifierToken>();
}

/*
bool Lexer::produceNumber()
{
    if (!isdigit(mySource->getCurrentChar()))
        return false;

    int integerPart = parseSingleNumberFromSource();

    if (mySource->getCurrentChar() == '.' && isdigit( mySource->getNextChar() ) )
    {
        mySource->processOneChar();
        int fractionalPart = parseSingleNumberFromSource();
        int length = floor(log10((float)fractionalPart)) + 1;   // TODO znowy problem z zerami wiod¹cymi
        double x = fractionalPart / pow(10, length);
        currentToken = new FloatToken( (double) integerPart + x);
    }
    else
        currentToken = new IntToken(integerPart);

    return true;
}

int Lexer::parseSingleNumberFromSource()
{
    // TODO co z zerami wiod¹cymi
    int result = 0;
    char tempChar = mySource->getCurrentChar();

    do {
        if (result < INT_MAX)
            result = result * 10 + ((int)tempChar - (int)'0');
        mySource->processOneChar();
        tempChar = mySource->getCurrentChar();
    } while (isdigit(tempChar));

    return result;
}

bool Lexer::produceOperator()
{
    if (!isOperator(mySource->getCurrentChar()))
        return false;

    char currChar = mySource->getCurrentChar();

    std::string operatorValue = "" + currChar;
    mySource->processOneChar();
    
    currChar = mySource->getCurrentChar();
    if (predefinedSymbols.isEndOfOperator(currChar))
    {
        operatorValue += currChar;
        mySource->processOneChar();
    }

    currentToken = new OperatorToken(operatorValue);
    
    return true;
}

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

bool Lexer::isOperator(char charToCheck)
{
    if (predefinedSymbols.isBeginningOfOperator(charToCheck))
        return true;
    else
        return false;
}

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
