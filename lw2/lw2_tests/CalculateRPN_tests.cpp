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
		BOOST_CHECK(TokenizeString("a b + c *") == expectedResult);
	}
}

BOOST_AUTO_TEST_CASE(IsFLoat_fn_)
{
	{
		BOOST_CHECK(IsFloat("42"));
		BOOST_CHECK(IsFloat("-42"));
		BOOST_CHECK(IsFloat("3.141592653589793238"));
		BOOST_CHECK(IsFloat("-3.141592653589793238"));
		BOOST_CHECK(!IsFloat("3,141592653589793238"));
	}
}

BOOST_AUTO_TEST_CASE(CalculateRPN_fn)
{
	{
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("2 3 +")), 5.f, eps);
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("1 2 + 3 *")), 9.f, eps);
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("2 3 + 4 * 5 6 * -")), -10.f, eps);
	}
}

BOOST_AUTO_TEST_SUITE_END()
