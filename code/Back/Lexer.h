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
	Source* mySource;

	SymbolTable predefinedSymbols;

protected:	// chcê ¿eby klasa testuj¹ca mia³a dostêp do metod

	//metoda z "g³ównym switchem":
	void produceToken();
	
	//metody tworz¹ce odpowiedni token:
	std::optional<std::unique_ptr<Token>> produceIdentifier();
	/*
	std::optional<std::unique_ptr<Token>> produceNumber();
	std::optional<std::unique_ptr<Token>> produceOperator();
	std::unique_ptr<Token> produceSpecial();
	*/
	// TODO odkomentowywaæ sukcesuwnie pisz¹c testy

	//pomocnicze:
	void skipWhiteSpacesAndComments();
	bool isOperator(char charToCheck);
	//int parseSingleNumberFromSource();
	// TODO odkomentowywaæ sukcesuwnie pisz¹c testy

	const int MAX_IDENTIFIER_LENGTH = 50;	// TODO docelowo z pliku konfiguracyjnego

public:
	Lexer();
	Lexer(Source* newSourcel);
	void loadStandardOperators();
	void setSource(Source* newSource);

	// metoda wo³ana przez parser:
	std::unique_ptr<Token> nextToken();

	// tablica symboli:
	SymbolTable* getSymbolTable();

};
