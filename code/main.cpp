#include"Back/StringSource.h"
#include "Back/Lexer.h"

#include<iostream>

int main()
{
	StringSource s("to jest testowy string");

	Lexer l;
	l.setSource(&s);
	l.loadStandardOperators();

	Token* t = nullptr;
	t = l.nextToken();

	return 0;
}