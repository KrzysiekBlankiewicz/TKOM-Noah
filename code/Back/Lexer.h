#pragma once
#include "SymbolTable.h"
#include "Token.h"
#include "Source.h"
#include <vector>
#include <set>
#include <optional>

class Lexer
{
	std::unique_ptr<Token> currentToken;
	int currentTokenLineInSource, currentTokenPositionInLine;
	Source* mySource;	// TODO to móg³by byæ unique_ptr, tylko nie mia³bym do niego dostêpu po setSource - do przemyœlenia - ew. shared_ptr

	SymbolTable predefinedSymbols;

protected:	// chcê ¿eby klasa testuj¹ca mia³a dostêp do metod

	//metoda z "g³ównym switchem":
	void produceToken();
	
	//metody tworz¹ce odpowiedni token:
	std::optional<std::unique_ptr<Token>> produceIdentifier();
	std::optional<std::unique_ptr<Token>> produceNumber();
	std::optional<std::unique_ptr<Token>> produceOperator();
	std::unique_ptr<Token> produceSpecial();

	//pomocnicze:
	void skipWhiteSpacesAndComments();
	bool isOperator(char charToCheck);
	std::optional<int> parseIntegerPartFromSource();
	std::optional<double> parseFractionalPartFromSource();

	const int MAX_IDENTIFIER_LENGTH = 50;	// TODO docelowo z pliku konfiguracyjnego

public:
	Lexer();
	Lexer(Source* newSourcel);
	void setSource(Source* newSource);

	// metoda wo³ana przez parser:
	std::unique_ptr<Token> nextToken();

	// tablica symboli:
	SymbolTable* getSymbolTable();	// TODO nie wiem czy to potrzebne
	void loadStuffIntoSymbolTable();

};
