#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include <algorithm>

#include "../Back/Lexer.h"
#include "LexerTester.h"
#include "TokenTester.h"
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
	BOOST_AUTO_TEST_CASE(checkIfDefaultKeywordtypeIsNOT_A_KEYWORD)
	{
		KeywordType k = KeywordType();
		BOOST_CHECK(k == KeywordType::NOT_A_KEYWORD);
	}
	BOOST_AUTO_TEST_CASE(randomStringIsNotAKeywordInEmptyTable)
	{
		SymbolTable table;
		BOOST_CHECK(! table.getKeywordType("abcd").has_value());
	}
	BOOST_AUTO_TEST_CASE(standardKeywordIsNotAKeywordInEmptyTable)
	{
		SymbolTable table;
		BOOST_CHECK(! table.getKeywordType("if").has_value());
	}
	BOOST_AUTO_TEST_CASE(randomStringIsNotAKeywordInFilledTable)
	{
		SymbolTable table;
		table.loadStandardKeywords();
		BOOST_CHECK(! table.getKeywordType("abcd").has_value());
	}
	BOOST_AUTO_TEST_CASE(standardKeywordIsAKeywordInFilledTable)
	{
		SymbolTable table;
		table.loadStandardKeywords();
		BOOST_CHECK(table.getKeywordType("if").value() == KeywordType::IF_KEYWORD);
	}
	BOOST_AUTO_TEST_CASE(checkIfDefaultOperatortypeIsNOT_A_OPERATOR)
	{
		OperatorType o = OperatorType();
		BOOST_CHECK(o == OperatorType::NOT_A_OPERATOR);
	}
	BOOST_AUTO_TEST_CASE(randomCharIsNotAnOperatorInEmptyTable)
	{
		SymbolTable table;
		BOOST_CHECK(! table.getOperatorToken('a', 'b').has_value());
	}
	BOOST_AUTO_TEST_CASE(standardOnesignOperatorIsNotAnOperatorInEmptyTable)
	{
		SymbolTable table;
		BOOST_CHECK(! table.getOperatorToken('=', 'a').has_value());
	}
	BOOST_AUTO_TEST_CASE(standardDoubleSignOperatorIsNotAnOperatorInEmptyTable)
	{
		SymbolTable table;
		BOOST_CHECK(! table.getOperatorToken('=', '=').has_value());
	}
	BOOST_AUTO_TEST_CASE(standardOnesignOperatorIsAnOperatorInFilledTable)
	{
		SymbolTable table;
		TokenTester tester;
		table.loadStandardOperators();

		std::optional<std::unique_ptr<Token>> t = table.getOperatorToken('=', 'a');
		BOOST_CHECK( t.has_value());
		BOOST_CHECK( t.value()->accept(&tester) == TokenTester::OPERATOR);
		BOOST_CHECK( t.value()->getContent() == "=");
	}
	BOOST_AUTO_TEST_CASE(standardDoublesignOperatorIsAnOperatorInFilledTable)
	{
		SymbolTable table;
		TokenTester tester;
		table.loadStandardOperators();

		std::optional<std::unique_ptr<Token>> t = table.getOperatorToken('=', '=');
		BOOST_CHECK( t.has_value());
		BOOST_CHECK( t.value()->accept(&tester) == TokenTester::OPERATOR);
		BOOST_CHECK( t.value()->getContent() == "==");
	}
	BOOST_AUTO_TEST_CASE(randomCharIsNotAnOperatorInFilledTable)
	{
		SymbolTable table;
		TokenTester tester;
		table.loadStandardOperators();

		std::optional<std::unique_ptr<Token>> t = table.getOperatorToken('a', 'b');
		BOOST_CHECK(! t.has_value());
	}
	/*BOOST_AUTO_TEST_CASE(staticCastingKeywordTypeToDouble)
	{
		KeywordType k = KeywordType::FOR_KEYWORD;

		BOOST_CHECK_EQUAL(static_cast<double>(k), 2.0);
		k = KeywordType::NOT_A_KEYWORD;
		BOOST_CHECK_EQUAL(static_cast<double>(k), 0.0);
	}*/
BOOST_AUTO_TEST_SUITE_END()


// LEXER TESTS:

BOOST_AUTO_TEST_SUITE(produceIdentifierTests)
	BOOST_AUTO_TEST_CASE(onSourceLengthEqualToZero)
	{
		StringSource source("");		
		LexerTester l(&source);
		BOOST_CHECK( ! l.test_produceIdentifier().has_value() );
		BOOST_CHECK(source.isEndOfInput());
}
	BOOST_AUTO_TEST_CASE(onSourceStartingWithNoalpha)
	{
		StringSource source("4>5_bh7");
		LexerTester l(&source);
		BOOST_CHECK( ! l.test_produceIdentifier().has_value() );
		BOOST_CHECK_EQUAL(source.getCurrentChar(), '4');
	}
	BOOST_AUTO_TEST_CASE(onAlphaOnlyIdentifier)
	{
		std::string s = "alphaOnlyIdentifier";
		StringSource source(s);
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceIdentifier();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value().get()->accept(&t), TokenTester::IDENTIFIER );
		BOOST_CHECK_EQUAL( result.value().get()->getContent(), s);
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(onAlphaAndNumIdentifier)
	{
		std::string s = "alpha1212Ident32i6fier";
		StringSource source(s);
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceIdentifier();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::IDENTIFIER);
		BOOST_CHECK_EQUAL(result.value().get()->getContent(), s);
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(onAlphaNumAndUnderscoreIdentifier)
	{
		std::string s = "alpha_12_2Ident32_fier";
		StringSource source(s);
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceIdentifier();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::IDENTIFIER);
		BOOST_CHECK_EQUAL(result.value().get()->getContent(), s);
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(onKeywordWithTableFilledWithKeywords)
	{
		std::string s = "return";
		StringSource source(s);
		LexerTester l(&source);
		TokenTester t;
		l.loadStuffIntoSymbolTable();

		std::optional<std::unique_ptr<Token>> result = l.test_produceIdentifier();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::KEYWORD);
		BOOST_CHECK_EQUAL(result.value().get()->getContent(), s);
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(onKeywordWithEmptyTable)
	{
		std::string s = "return";
		StringSource source(s);
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceIdentifier();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::IDENTIFIER);
		BOOST_CHECK_EQUAL(result.value().get()->getContent(), s);
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(onIdentifierWithTableFilledWithKeywords)
	{
		std::string s = "someRandomString";
		StringSource source(s);
		LexerTester l(&source);
		TokenTester t;
		l.loadStuffIntoSymbolTable();

		std::optional<std::unique_ptr<Token>> result = l.test_produceIdentifier();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::IDENTIFIER);
		BOOST_CHECK_EQUAL(result.value().get()->getContent(), s);
		BOOST_CHECK(source.isEndOfInput());
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(parseIntegerPartTests)
	BOOST_AUTO_TEST_CASE(sourceStartingWithNodigit)	// to nie ma prawa siê wydarzyæ, ale...
	{
		StringSource s("as67f_3d87n");
		LexerTester l(&s);
		
		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK(! result.has_value() );
	}
	BOOST_AUTO_TEST_CASE(onSourceLengthEqualToZero)
	{
		StringSource s("");
		LexerTester l(&s);
		
		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK(! result.has_value() );
	}
	BOOST_AUTO_TEST_CASE(onStandardNumber)
	{
		StringSource s("123");
		LexerTester l(&s);

		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value(), 123);
	}
	BOOST_AUTO_TEST_CASE(onStandardNumberVol2)
	{
		StringSource s("12300300");
		LexerTester l(&s);

		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value(), 12300300);
	}
	BOOST_AUTO_TEST_CASE(onStandardNumberFollowedByDot)
	{
		StringSource s("123.456");
		LexerTester l(&s);
		
		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value(), 123);
	}
	BOOST_AUTO_TEST_CASE(onStandardNumberFollowedByNondigit)
	{
		StringSource s("123a456");
		LexerTester l(&s);
		
		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value(), 123);
	}
	BOOST_AUTO_TEST_CASE(onNumberBiggerThanMAX_INT)
	{
		std::string maxString = std::to_string(INT_MAX);
		StringSource s(maxString + "12");
		LexerTester l(&s);

		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK(! result.has_value() );
	}
	BOOST_AUTO_TEST_CASE(onLeadingZero)
	{
		StringSource s("0123");
		LexerTester l(&s);

		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK(! result.has_value() );
	}
	BOOST_AUTO_TEST_CASE(onSoloZero)
	{
		StringSource s("0");
		LexerTester l(&s);

		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value(), 0 );
	}
	BOOST_AUTO_TEST_CASE(onZeroFollowedByNondigit)
	{
		StringSource s("0abc");
		LexerTester l(&s);

		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value(), 0 );
	}
	BOOST_AUTO_TEST_CASE(onZeroFollowedByDot)
	{
		StringSource s("0.123");
		LexerTester l(&s);

		std::optional<int> result = l.test_parseIntegerPartFromSource();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value(), 0 );
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(parseFractionalPartTests)
	BOOST_AUTO_TEST_CASE(sourceStartingWithNodigit)	// to nie ma prawa siê wydarzyæ, ale...
	{
		StringSource s("as67f_3d87n");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(!result.has_value());
	}
	BOOST_AUTO_TEST_CASE(onSourceLengthEqualToZero)
	{
		StringSource s("");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(!result.has_value());
	}
	BOOST_AUTO_TEST_CASE(onStandardNumber)
	{
		StringSource s("123");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0.123);
	}
	BOOST_AUTO_TEST_CASE(onStandardNumberVol2)
	{
		StringSource s("12300300");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0.12300300);
	}
	BOOST_AUTO_TEST_CASE(onStandardNumberFollowedByDot)
	{
		StringSource s("123.456");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0.123);
	}
	BOOST_AUTO_TEST_CASE(onStandardNumberFollowedByNondigit)
	{
		StringSource s("123a456");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0.123);
	}
	BOOST_AUTO_TEST_CASE(onNumberBiggerThanMAX_INT)
	{
		std::string maxString = std::to_string(INT_MAX);
		StringSource s(maxString + "12");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(!result.has_value());
	}
	BOOST_AUTO_TEST_CASE(onLeadingZero)
	{
		StringSource s("0123");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0.0123);
	}
	BOOST_AUTO_TEST_CASE(onMultipleLeadingZeros)
	{
		StringSource s("000123");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0.000123);
	}
	BOOST_AUTO_TEST_CASE(onSoloZero)
	{
		StringSource s("0");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0);
	}
	BOOST_AUTO_TEST_CASE(onSoloMultipleZeros)
	{
		StringSource s("0000");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0);
	}
	BOOST_AUTO_TEST_CASE(onZeroFollowedByNondigit)
	{
		StringSource s("0abc");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0);
	}
	BOOST_AUTO_TEST_CASE(onZeroFollowedByDot)
	{
		StringSource s("0.123");
		LexerTester l(&s);

		std::optional<double> result = l.test_parseFractionalPartFromSource();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value(), 0);
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(skipWhiteSpacesAndCommentsTests)
	BOOST_AUTO_TEST_CASE(onSourceLengthEqualZero)
	{
		StringSource source("");
		LexerTester l(&source);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(onSourceBeginnigWithAlpha)
	{
		StringSource s("abc	123	");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onComment)
	{
		StringSource s("#cwhvda#a");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onMultiplewhiteSpaces)
	{
		StringSource s("		 a");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onMultipleComments)
	{
		StringSource s("#cwhvda##76f4#a");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onWhitespace_Comment)
	{
		StringSource s("	#cwhvda#a");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onComment_WhiteSpace)
	{
		StringSource s("#cwhvda# a");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onComment_WhiteSpace_Comment)
	{
		StringSource s("#cwhvda#	#26d4g#a");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onWhitespace_Comment_Whitespace)
	{
		StringSource s(" #cwhvda#	a");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onTwolineComment)
	{
		StringSource s(" #cwhv \n c#	a");
		LexerTester l(&s);

		l.test_skipWhiteSpacesAndComments();
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(produceNumberTests)
	BOOST_AUTO_TEST_CASE(onSourceLengthEqualZero)
	{
		StringSource source("");
		LexerTester l(&source);

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
	}
	BOOST_AUTO_TEST_CASE(onSourceBeginningWithAlpha)
	{
		StringSource source("abc4367");
		LexerTester l(&source);

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
		BOOST_CHECK_EQUAL(source.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onSourceBeginningWithRandomSign)
	{
		StringSource source("<23kjvh6");
		LexerTester l(&source);

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
		BOOST_CHECK_EQUAL(source.getCurrentChar(), '<');
	}
	BOOST_AUTO_TEST_CASE(onStandardInt)
	{
		StringSource source("12345");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( result.has_value());
		BOOST_CHECK_EQUAL( result.value().get()->accept(&t), TokenTester::INT);
		//BOOST_CHECK_EQUAL( result.value().get()->getContent(), "12345");
		BOOST_CHECK( result.value().get()->getValue().has_value() );
		BOOST_CHECK_EQUAL( result.value().get()->getValue().value(), 12345 );
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(onStandardIntFollowedBySth)
	{
		StringSource source("12345abc");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::INT);
		//BOOST_CHECK_EQUAL(result.value().get()->getContent(), "12345");
		BOOST_CHECK(result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getValue().value(), 12345);
		BOOST_CHECK_EQUAL(source.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onStandardFloat)
	{
		StringSource source("123.456");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value().get()->accept(&t), TokenTester::FLOAT );
		//BOOST_CHECK_EQUAL( result.value().get()->getContent(), "123.456" );
		BOOST_CHECK( result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL( result.value().get()->getValue().value(), 123.456);
	}
	BOOST_AUTO_TEST_CASE(onStandardFloatFollowedBySth)
	{
		StringSource source("123.456abc");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::FLOAT);
		//BOOST_CHECK_EQUAL(result.value().get()->getContent(), "123.456");
		BOOST_CHECK(result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getValue().value(), 123.456);
		BOOST_CHECK_EQUAL(source.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onZero)
	{
		StringSource source("0");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::INT);
		//BOOST_CHECK_EQUAL(result.value().get()->getContent(), "0");
		BOOST_CHECK(result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getValue().value(), 0);
	}
	BOOST_AUTO_TEST_CASE(onZeroIntegerPart)
	{
		StringSource source("0.123");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::FLOAT);
		//BOOST_CHECK_EQUAL(result.value().get()->getContent(), "0.123");
		BOOST_CHECK(result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getValue().value(), 0.123);
	}
	BOOST_AUTO_TEST_CASE(onZeroFractionalPart)
	{
		StringSource source("123.0");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::FLOAT);
		//BOOST_CHECK_EQUAL(result.value().get()->getContent(), "123.0");
		BOOST_CHECK(result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getValue().value(), 123.0);
	}
	BOOST_AUTO_TEST_CASE(onZeroBothParts)
	{
		StringSource source("0.0");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::FLOAT);
		//BOOST_CHECK_EQUAL(result.value().get()->getContent(), "0.0");
		BOOST_CHECK(result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getValue().value(), 0.0);
	}
	BOOST_AUTO_TEST_CASE(onFractionalPartWithLeadingZeros)
	{
		StringSource source("123.001");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::FLOAT);
		//BOOST_CHECK_EQUAL(result.value().get()->getContent(), "123.001");
		BOOST_CHECK(result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getValue().value(), 123.001);
		BOOST_CHECK(source.isEndOfInput());
	}
	BOOST_AUTO_TEST_CASE(onIntegerPartWithLeadingZeros)
	{
		StringSource source("001.123");
		LexerTester l(&source);

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
	}
	BOOST_AUTO_TEST_CASE(onZeroIntegerPartWithLeadingZeros)
	{
		StringSource source("000.123");
		LexerTester l(&source);

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
	}
	BOOST_AUTO_TEST_CASE(onDotWithoutIntegralPart)
	{
		StringSource source(".123");
		LexerTester l(&source);

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
	}
	BOOST_AUTO_TEST_CASE(onDotWithoutFractionalPart)
	{
		StringSource source("123.");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::INT);
		//BOOST_CHECK_EQUAL(result.value().get()->getContent(), "123");
		BOOST_CHECK(result.value().get()->getValue().has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getValue().value(), 123);
		BOOST_CHECK_EQUAL(source.getCurrentChar(), '.');
	}
	BOOST_AUTO_TEST_CASE(doesItLeavelastCharOfTooBigNumberAsCurrent) // jest to potrzebne ¿eby produceOperator po produceNumber nie zwróci³o tokenu
	{
		StringSource source("12345678901");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
		BOOST_CHECK_EQUAL(source.getCurrentChar(), '1');
	}
	BOOST_AUTO_TEST_CASE(doesItConsumeOnlyFirstNMinusOneZerosFromLeadingZerosToInvalidToken)	// jest to potrzebne ¿eby produceOperator po produceNumber nie zwróci³o tokenu
	{
		StringSource source("00.6123");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
		BOOST_CHECK_EQUAL(source.getCurrentChar(), '0');
	}
	BOOST_AUTO_TEST_CASE(doesItLeavePartOfTooBigFractionalPartAsCurrent)
	{
		StringSource source("2.12345678901");
		LexerTester l(&source);
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceNumber();
		BOOST_CHECK( ! result.has_value());
		BOOST_CHECK_EQUAL(source.getCurrentChar(), '1');
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(produceOperatorTests)
	BOOST_AUTO_TEST_CASE(onSourceLengthEqualZero)
	{
		StringSource s("");
		LexerTester l(&s);

		std::optional<std::unique_ptr<Token>> result = l.test_produceOperator();
		BOOST_CHECK( ! result.has_value() );
		BOOST_CHECK(s.isEndOfInput() );
	}
	BOOST_AUTO_TEST_CASE(onSourceBeginningWithAlpha)
	{
		StringSource s("abc<6");
		LexerTester l(&s);

		std::optional<std::unique_ptr<Token>> result = l.test_produceOperator();
		BOOST_CHECK( ! result.has_value());
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onSourceBeginningWithRandomSign)
	{
		StringSource s("$=67dt");
		LexerTester l(&s);

		std::optional<std::unique_ptr<Token>> result = l.test_produceOperator();
		BOOST_CHECK( ! result.has_value());
		BOOST_CHECK_EQUAL(s.getCurrentChar(), '$');
	}
	BOOST_AUTO_TEST_CASE(onStandardOperatorWithEmptySymbolTable)
	{
		StringSource s(">=");
		LexerTester l(&s);

		std::optional<std::unique_ptr<Token>> result = l.test_produceOperator();
		BOOST_CHECK( ! result.has_value() );
		BOOST_CHECK_EQUAL(s.getCurrentChar(), '>');
	}
	BOOST_AUTO_TEST_CASE(onOnesighOperatorWithFilledSymbolTable)
	{
		StringSource s("=abc");
		LexerTester l(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceOperator();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL( result.value().get()->getContent(), "=");
		BOOST_CHECK_EQUAL( result.value().get()->accept(&t), TokenTester::OPERATOR);
		BOOST_CHECK_EQUAL( s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onDoublesignOperatorWithFilledSymbolTable)
	{
		StringSource s("!=abc");
		LexerTester l(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceOperator();
		BOOST_CHECK(result.has_value());
		BOOST_CHECK_EQUAL(result.value().get()->getContent(), "!=");
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::OPERATOR);
		BOOST_CHECK_EQUAL(s.getCurrentChar(), 'a');
	}
	BOOST_AUTO_TEST_CASE(onRandomSignWithFilledSymbolTable)
	{
		StringSource s("23abc");
		LexerTester l(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceOperator();
		BOOST_CHECK( ! result.has_value());
		BOOST_CHECK_EQUAL(s.getCurrentChar(), '2');
	}
	BOOST_AUTO_TEST_CASE(onTwoOnesignOperatorsFollowing)
	{
		StringSource s("!<");
		LexerTester l(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::optional<std::unique_ptr<Token>> result = l.test_produceOperator();
		BOOST_CHECK( result.has_value() );
		BOOST_CHECK_EQUAL(result.value().get()->getContent(), "!");
		BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::OPERATOR);
		BOOST_CHECK_EQUAL(s.getCurrentChar(), '<');
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(produceSpecialTests)
BOOST_AUTO_TEST_CASE(onSourceLengthEqualZero)
{
	StringSource s("");
	LexerTester l(&s);
	TokenTester t;

	std::optional<std::unique_ptr<Token>> result = l.test_produceSpecial();
	BOOST_CHECK(result.has_value());
	BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::EOT);
	BOOST_CHECK(s.isEndOfInput());
}
BOOST_AUTO_TEST_CASE(onBeingAtTheEndOfSource)
{
	StringSource s("a");
	LexerTester l(&s);
	TokenTester t;
	s.processOneChar();

	std::optional<std::unique_ptr<Token>> result = l.test_produceSpecial();
	BOOST_CHECK(result.has_value());
	BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::EOT);
	BOOST_CHECK(s.isEndOfInput());
}
BOOST_AUTO_TEST_CASE(onSourceBeginningWithAlpha)
{
	StringSource s("abc");
	LexerTester l(&s);
	TokenTester t;

	std::optional<std::unique_ptr<Token>> result = l.test_produceSpecial();
	BOOST_CHECK(result.has_value());
	BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::INVALID);
	BOOST_CHECK_EQUAL(s.getCurrentChar(), 'b');
}
BOOST_AUTO_TEST_CASE(onSourceBeginningWithNum)
{
	StringSource s("123abc");
	LexerTester l(&s);
	TokenTester t;

	std::optional<std::unique_ptr<Token>> result = l.test_produceSpecial();
	BOOST_CHECK(result.has_value());
	BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::INVALID);
	BOOST_CHECK_EQUAL(s.getCurrentChar(), '2');
}
BOOST_AUTO_TEST_CASE(onSourceBeginningWithSth)
{
	StringSource s("*");
	LexerTester l(&s);
	TokenTester t;

	std::optional<std::unique_ptr<Token>> result = l.test_produceSpecial();
	BOOST_CHECK(result.has_value());
	BOOST_CHECK_EQUAL(result.value().get()->accept(&t), TokenTester::INVALID);
	BOOST_CHECK(s.isEndOfInput());
}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(nextTokenTests)
	BOOST_AUTO_TEST_CASE(identifierTokenTest)
	{
		StringSource s("abc_34a");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "abc_34a");
	}
	BOOST_AUTO_TEST_CASE(keywordTokenTest)
	{
		StringSource s("return");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.KEYWORD);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL( token->getValue().value(), static_cast<double>(KeywordType::RETURN_KEYWORD) );
		BOOST_CHECK_EQUAL(token->getContent(), "return");
	}
	BOOST_AUTO_TEST_CASE(intTokenTest)
	{
		StringSource s("342516");
		Lexer l;
		l.setSource(&s);
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INT);
		BOOST_CHECK_EQUAL(token->getContent(), "342516");
		BOOST_CHECK( token->getValue().has_value() );
		BOOST_CHECK_EQUAL(token->getValue().value(), 342516.0);
	}
	BOOST_AUTO_TEST_CASE(floatTokenTest)
	{
		StringSource s("123.456");
		Lexer l;
		l.setSource(&s);
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.FLOAT);
		//BOOST_CHECK_EQUAL(token->getContent(), "123.456");
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 123.456);
	}
	BOOST_AUTO_TEST_CASE(operatorTokenTest)
	{
		StringSource s(">=");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.OPERATOR);
		BOOST_CHECK_EQUAL(token->getContent(), ">=");
	}
	BOOST_AUTO_TEST_CASE(unknownTokenTest)
	{
		StringSource s("%");
		Lexer l;
		l.setSource(&s);
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INVALID);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tokenPositionTests)
	BOOST_AUTO_TEST_CASE(atTheBeginningCase)
	{
		StringSource s("a");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 1);
	}
	BOOST_AUTO_TEST_CASE(randomPositionsInOneLineCase)
	{
		StringSource s("abd 75.8>= % 4 #8273#");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 1);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 5);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 9);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 12);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 14);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 22);
	}
	BOOST_AUTO_TEST_CASE(multipleLinesCase)
	{
		StringSource s(" abd \n75.8>=\n % 4 #82\n73#");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();

		//abd:
		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 2);
		//float
		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 2);
		BOOST_CHECK_EQUAL(token->getPosition(), 1);
		//>=
		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 2);
		BOOST_CHECK_EQUAL(token->getPosition(), 5);
		//%
		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 3);
		BOOST_CHECK_EQUAL(token->getPosition(), 2);
		//4
		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 3);
		BOOST_CHECK_EQUAL(token->getPosition(), 4);
		//EOT
		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->getLine(), 4);
		BOOST_CHECK_EQUAL(token->getPosition(), 4);
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(aLaIntegrationTests)
	BOOST_AUTO_TEST_CASE(multipleTokenTest)
	{
		StringSource s(" abc 123=34.5 >= #ab%r# % if");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "abc");
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 2);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INT);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 123);
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 6);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.OPERATOR);
		BOOST_CHECK_EQUAL(token->getContent(), "=");
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 9);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.FLOAT);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 34.5);
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 10);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.OPERATOR);
		BOOST_CHECK_EQUAL(token->getContent(), ">=");
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 15);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INVALID);
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 25);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.KEYWORD);
		BOOST_CHECK_EQUAL(token->getContent(), "if");
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), static_cast<double>(KeywordType::IF_KEYWORD));
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 27);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.EOT);
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 29);

		//na wszelki wypadek jeszcze raz:
		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.EOT);
		BOOST_CHECK_EQUAL(token->getLine(), 1);
		BOOST_CHECK_EQUAL(token->getPosition(), 29);
	}
	BOOST_AUTO_TEST_CASE(leadingZeroInNumberCase)
	{
		StringSource s(" abc 0123=34.05");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "abc");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INVALID);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INT);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 123);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.OPERATOR);
		BOOST_CHECK_EQUAL(token->getContent(), "=");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.FLOAT);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 34.05);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.EOT);
	}
	BOOST_AUTO_TEST_CASE(leadingZerosInNumberCase)
	{
		StringSource s(" abc 00123=34.05");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "abc");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INVALID);
		
		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INVALID);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INT);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 123);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.OPERATOR);
		BOOST_CHECK_EQUAL(token->getContent(), "=");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.FLOAT);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 34.05);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.EOT);
	}
	BOOST_AUTO_TEST_CASE(operatorAfterInvalidNumberCase)
	{
		StringSource s(" a 00= a");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "a");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INVALID);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INT);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 0);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.OPERATOR);
		BOOST_CHECK_EQUAL(token->getContent(), "=");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "a");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.EOT);
	}
	BOOST_AUTO_TEST_CASE(operatorAfterTooLongNumberCase)	// Je¿eli liczba jest za du¿a, konsumujemy j¹ do cyfry, która spowodowa³a przepe³nienie w³¹cznie
	{
		StringSource s(" a 12345678901= a");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "a");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INVALID);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.OPERATOR);
		BOOST_CHECK_EQUAL(token->getContent(), "=");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "a");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.EOT);
	}
	BOOST_AUTO_TEST_CASE(restOfTooBigNumberAsAnotherNumberCase)
	{
		StringSource s(" a 1234567890127= a");
		Lexer l;
		l.setSource(&s);
		l.loadStuffIntoSymbolTable();
		TokenTester t;

		std::unique_ptr<Token> token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "a");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INVALID);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.INT);
		BOOST_CHECK(token->getValue().has_value());
		BOOST_CHECK_EQUAL(token->getValue().value(), 27);

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.OPERATOR);
		BOOST_CHECK_EQUAL(token->getContent(), "=");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.IDENTIFIER);
		BOOST_CHECK_EQUAL(token->getContent(), "a");

		token = l.nextToken();
		BOOST_CHECK_EQUAL(token->accept(&t), t.EOT);
	}
BOOST_AUTO_TEST_SUITE_END()
