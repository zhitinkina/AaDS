#include "stdafx.h"

#include "../lw2/main.hpp"

namespace
{

static const auto eps = 10e-3;

}

BOOST_AUTO_TEST_SUITE(ConvertRPN_)

BOOST_AUTO_TEST_CASE(TokenizeString_fn_)
{
	{
		const auto expectedResult = vector<string>{ "a", "b", "+", "c", "*" };
		BOOST_CHECK(Tokenize("a b + c *") == expectedResult);
	}
}

BOOST_AUTO_TEST_CASE(CalculateRPN_fn)
{
	{
		BOOST_REQUIRE_CLOSE_FRACTION(CalcRPN(Tokenize("2 3 +")), 5.f, eps);
		BOOST_REQUIRE_CLOSE_FRACTION(CalcRPN(Tokenize("1 2 + 3 *")), 9.f, eps);
		BOOST_REQUIRE_CLOSE_FRACTION(CalcRPN(Tokenize("2 3 + 4 * 5 6 * -")), -10.f, eps);
	}
}

BOOST_AUTO_TEST_SUITE_END()
