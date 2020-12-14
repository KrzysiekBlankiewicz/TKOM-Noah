#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include <algorithm>

#include "../Back/Lexer.h"
#include "../Back/StringSource.h"


BOOST_AUTO_TEST_SUITE(TokenTsterTests)

	BOOST_AUTO_TEST_CASE(visitMethod)
	{
		std::unique_ptr<TokenTester> tester = std::make_unique<TokenTester>();
	
		std::unique_ptr<Token> token = std::make_unique<Token>();
		int tokenTypeFlag = TokenTester::UNKNOWN;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);

		token = std::make_unique<IdentifierToken>();
		tokenTypeFlag = TokenTester::IDENTIFIER;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);

		token = std::make_unique<KeywordToken>();
		tokenTypeFlag = TokenTester::KEYWORD;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);

		token = std::make_unique<OperatorToken>();
		tokenTypeFlag = TokenTester::OPERATOR;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);

		token = std::make_unique<IntToken>();
		tokenTypeFlag = TokenTester::INT;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);

		token = std::make_unique<FloatToken>();
		tokenTypeFlag = TokenTester::FLOAT;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);

		token = std::make_unique<EOTToken>();
		tokenTypeFlag = TokenTester::EOT;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);

		token = std::make_unique<InvalidToken>();
		tokenTypeFlag = TokenTester::INVALID;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);

		token = std::make_unique<UnsafeToken>();
		tokenTypeFlag = TokenTester::UNSAFE;
		BOOST_CHECK_EQUAL(token->accept(tester.get()), tokenTypeFlag);
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(stringSourceTests)

	std::string testString = "abcd";

	BOOST_AUTO_TEST_CASE(isStringSet)
	{
		std::string s = testString;
		StringSource source(s);
		BOOST_CHECK(source.getMyString() == s);
	}
	BOOST_AUTO_TEST_CASE(initialCurrentCharInStandardString)
	{
		std::string s = testString;
		StringSource source(s);
		BOOST_CHECK(source.getCurrentChar() == s[0]);
		BOOST_CHECK(source.getNextChar() == s[1]);
	}
	BOOST_AUTO_TEST_CASE(initialCurrentCharInEmptyString)
	{
		std::string s = "";
		StringSource source(s);
		BOOST_CHECK(source.getCurrentChar() == StringSource::CHAR_AFTER_INPUT_END);
		BOOST_CHECK(source.getNextChar() == StringSource::CHAR_AFTER_INPUT_END);
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(processingAllCharsInSource)
	{
		std::string s = testString;
		StringSource source(s);
		for (int i = 0; i < s.length() - 1; ++i)
			source.processOneChar();

		BOOST_CHECK(source.getCurrentChar() == s[s.length() - 1] );
		BOOST_CHECK(source.getNextChar() == StringSource::CHAR_AFTER_INPUT_END);
		BOOST_CHECK( ! source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(processingMoreCharsThanInSource)
	{
		std::string s = testString;
		StringSource source(s);
		for(int i = 0; i < s.length() + 1; ++i)
			source.processOneChar();

		BOOST_CHECK(source.getCurrentChar() == StringSource::CHAR_AFTER_INPUT_END);
		BOOST_CHECK(source.getNextChar() == StringSource::CHAR_AFTER_INPUT_END);
		BOOST_CHECK(source.isEndOfInput());
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(symbolTableTests)
	BOOST_AUTO_TEST_CASE(searchingInEmptytable)
	{
		SymbolTable table;
		std::string s = "abcd";
		BOOST_CHECK( ! table.isKeyword(s));
		BOOST_CHECK( ! table.isOperator(s));
	}
	BOOST_AUTO_TEST_CASE(addedSinglesignOperatorIsInTable)
	{
		SymbolTable table;
		std::string s = "a";
		table.addOperator(s);
		BOOST_CHECK(table.isOperator(s));
		BOOST_CHECK(table.isBeginningOfOperator(s[0]));
	}
	BOOST_AUTO_TEST_CASE(addedDoublesignOperatorIsInTable)
	{
		SymbolTable table;
		std::string s = "ab", s1 = "ba";
		table.addOperator(s);
		BOOST_CHECK(table.isOperator(s));
		BOOST_CHECK( ! table.isOperator(s1));
		BOOST_CHECK(table.isBeginningOfOperator(s[0]));
		BOOST_CHECK(table.isEndOfOperator(s[1]));
	}
	BOOST_AUTO_TEST_CASE(addedKeywordIsInTable)
	{
		SymbolTable table;
		std::string s = "ab", s1  = "ba";
		table.addKeyword(s);
		BOOST_CHECK(table.isKeyword(s));
		BOOST_CHECK( ! table.isKeyword(""));
		BOOST_CHECK( ! table.isKeyword(s1));
	}
BOOST_AUTO_TEST_SUITE_END()


std::string indentifierString = "scdsd";
std::string intString = "245";
std::string floatString = "854.05";
std::string operatorString = ">=";
std::string unknownString = "%";
std::vector < std::pair<std::string, int>> stringsVector = { {indentifierString, TokenTester::IDENTIFIER},
															{intString, TokenTester::INT},
															{floatString, TokenTester::FLOAT},
															{operatorString, TokenTester::OPERATOR},
															{unknownString, TokenTester::INVALID} };

std::string sumStringVector(std::vector<std::pair<std::string, int>>* v)
{
	std::string result = "";
	for (auto a : *v)
	{
		result += a.first;
		result += " ";
	}
	return result;
}


BOOST_AUTO_TEST_SUITE(lexerTests)
	
	BOOST_AUTO_TEST_SUITE(particulateMathod)
		BOOST_AUTO_TEST_CASE(identifierTokenTest)
		{
			BOOST_CHECK(1 == 1);
		}


	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_CASE(identifierTokenTest)
	{
		StringSource s(indentifierString);
		Lexer l;
		l.setSource(&s);
		TokenTester t;

		BOOST_CHECK_EQUAL((l.nextToken())->accept(&t), t.IDENTIFIER);
	}
	BOOST_AUTO_TEST_CASE(keywordTokenTest)
	{
		StringSource s(indentifierString);
		Lexer l;
		l.getSymbolTable()->addKeyword(indentifierString);
		l.setSource(&s);
		TokenTester t;

		BOOST_CHECK_EQUAL((l.nextToken())->accept(&t), t.KEYWORD);
	}
	BOOST_AUTO_TEST_CASE(intTokenTest)
	{
		StringSource s(intString);
		Lexer l;
		l.setSource(&s);
		TokenTester t;

		BOOST_CHECK_EQUAL((l.nextToken())->accept(&t), t.INT);
	}
	BOOST_AUTO_TEST_CASE(floatTokenTest)
	{
		StringSource s(floatString);
		Lexer l;
		l.setSource(&s);
		TokenTester t;

		BOOST_CHECK_EQUAL((l.nextToken())->accept(&t), t.FLOAT);
	}
	BOOST_AUTO_TEST_CASE(operatorTokenTest)
	{
		StringSource s(operatorString);
		Lexer l;
		l.setSource(&s);
		l.getSymbolTable()->addOperator(operatorString);
		TokenTester t;

		BOOST_CHECK_EQUAL((l.nextToken())->accept(&t), t.OPERATOR);
	}

	BOOST_AUTO_TEST_CASE(unknownTokenTest)
	{
		StringSource s(unknownString);
		Lexer l;
		l.setSource(&s);
		TokenTester t;

		BOOST_CHECK_EQUAL((l.nextToken())->accept(&t), t.INVALID);
	}
	BOOST_AUTO_TEST_CASE(multipleTokenTest)
	{
		std::string input = sumStringVector(&stringsVector);
		StringSource s(input);
		Lexer l;
		l.setSource(&s);
		l.getSymbolTable()->addOperator(operatorString);
		TokenTester t;

		for (auto a : stringsVector)
		{
			BOOST_CHECK_EQUAL((l.nextToken())->accept(&t), a.second);
		}
	}
	BOOST_AUTO_TEST_CASE(multipleShuffledTokenTest)
	{
		//std::random_shuffle(stringsVector.begin(), stringsVector.end()); // TODO nie wiem dlaczego nie m tego w std...
		std::string input = sumStringVector(&stringsVector);

		StringSource s(input);
		Lexer l;
		l.setSource(&s);
		l.getSymbolTable()->addOperator(operatorString);
		TokenTester t;

		for (auto a : stringsVector)
		{
			BOOST_CHECK_EQUAL((l.nextToken())->accept(&t), a.second);
		}
	}
BOOST_AUTO_TEST_SUITE_END()