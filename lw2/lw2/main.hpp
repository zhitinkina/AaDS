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

string DoCalculation(string op1, string oper, string op2)
{
	return (!oper.empty())
		? to_string(DoOperation(oper, op1.empty() ? 0 : stof(op1), op2.empty() ? 0 : stof(op2)))
		: to_string(op2.empty() ? 0 : stof(op2));
}

vector<string> Tokenize(const string & str, char separator = ' ')
{
	stringstream stream(str);

	vector<string> result;
	string tmp;
	while (getline(stream, tmp, separator))
	{
		result.push_back(tmp);
	}
	return result;
}

float CalcRPN(const vector<string> & tokens)
{
	stack<string> stack;
	for (const auto & token : tokens)
	{
		if ((token == "+") || (token == "-") || (token == "*") || (token == "/"))
		{
			if (stack.size() < 2) throw runtime_error("Invalid stack size: cannot get operands");

			auto op1 = stack.top();
			stack.pop();
			auto op2 = stack.top();
			stack.pop();
			stack.emplace(DoCalculation(op2, token, op1));
		}
		else
		{
			if (!std::all_of(token.begin(), token.end(), ::isdigit)) throw exception(string(token + " is not a number").c_str());
			stack.emplace(DoCalculation("", "", token));
		}
	}
	if (stack.size() != 1)
	{
		throw runtime_error("Invalid stack state: should contains only one element");
	}
	return stof(stack.top());
}
