#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <sstream>

using namespace::std;

map<string, function<float(float, float)>> OPERATOR = {
	{"+", [](float lhs, float rhs) { return lhs + rhs; }},
	{"-", [](float lhs, float rhs) { return lhs - rhs; }},
	{"*", [](float lhs, float rhs) { return lhs * rhs; }},
	{"/", [](float lhs, float rhs) { return lhs / rhs; }},
};

float DoOperation(string oper, float lhs, float rhs)
{
	if ((oper == "+") || (oper == "-") || (oper == "*") || (oper == "/"))
	{
		return OPERATOR[oper](lhs, rhs);
	}
	throw exception(string("Unknown operation '" + oper + "'").c_str());
}

vector<string> CreateTokens(const string & str, char separator = ' ')
{
	vector<string> result;
	stringstream stream(str);
	string tmp;
	while (getline(stream, tmp, separator))
	{
		result.push_back(tmp);
	}
	return result;
}

float ReturnToExpression(const vector<string> & tokens) 
{
	function<string(string, string, string)> readOpers = [](string oper1, string oper, string oper2) {
		return oper.empty()
			? (to_string(oper2.empty() ? 0 : stof(oper2)))
			: (to_string(DoOperation(oper, oper1.empty() ? 0 : stof(oper1), oper2.empty() ? 0 : stof(oper2))));
	};

	stack<string> stack;
	for (const auto & token : tokens)
	{
		if ((token == "+") || (token == "-") || (token == "*") || (token == "/"))
		{
			if (stack.size() < 2) throw runtime_error("Cannot get operands: enter the correct expression");

			auto oper1 = stack.top();
			stack.pop();
			auto oper2 = stack.top();
			stack.pop();
			stack.emplace(readOpers(oper2, token, oper1));
		}
		else
		{
			if (!std::all_of(token.begin(), token.end(), ::isdigit)) throw exception(string(token + " is not a number").c_str());
			stack.emplace(readOpers("", "", token));
		}
	}
	if (stack.size() != 1)
	{
		throw runtime_error("Invalid stack state: the required statement was not found");
	}
	return stof(stack.top());
}
