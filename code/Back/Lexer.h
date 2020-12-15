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
	Source* mySource;	// TODO to m�g�by by� unique_ptr, tylko nie mia�bym do niego dost�pu po setSource - do przemy�lenia - ew. shared_ptr

	SymbolTable predefinedSymbols;

protected:	// chc� �eby klasa testuj�ca mia�a dost�p do metod

	//metoda z "g��wnym switchem":
	void produceToken();
	
	//metody tworz�ce odpowiedni token:
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

	// metoda wo�ana przez parser:
	std::unique_ptr<Token> nextToken();

	// tablica symboli:
	SymbolTable* getSymbolTable();	// TODO nie wiem czy to potrzebne
	void loadStuffIntoSymbolTable();

};
