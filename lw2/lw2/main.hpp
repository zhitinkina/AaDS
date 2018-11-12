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

class Expression
{
public:
	string oper1;
	string operation;
	string oper2;
	string m_result;

	Expression(string op1, string oper, string op2)
	{
		float o1 = !op1.empty() ? stof(op1) : 0;
		float o2 = !op2.empty() ? stof(op2) : 0;

		m_result = (!oper.empty())
			? to_string(DoOperation(oper, o1, o2))
			: to_string(o2);
		oper1 = "";
		operation = "";
		oper2 = m_result;
	}

	string ToString() const
	{
		return oper1 + operation + oper2;
	}
};

vector<string> Tokenize(const string & str, char delimeter = ' ')
{
	vector<string> tokens;

	stringstream stream(str);
	string tmp;

	while (getline(stream, tmp, delimeter))
	{
		tokens.push_back(tmp);
	}
	return tokens;
}

float CalcRPN(const vector<string> & tokens)
{
	stack<Expression> stack;
	for (const auto & token : tokens)
	{
		if ((token == "+") || (token == "-") || (token == "*") || (token == "/"))
		{
			if (stack.size() < 2) throw runtime_error("Invalid stack size: cannot get operands");

			auto op1 = stack.top();
			stack.pop();
			auto op2 = stack.top();
			stack.pop();
			stack.emplace(
				op2.oper1 + op2.operation + op2.oper2,
				token,
				op1.oper1 + op1.operation + op1.oper2
			);
		}
		else
		{
			if (!std::all_of(token.begin(), token.end(), ::isdigit)) throw exception(string(token + " is not a number").c_str());
			stack.emplace("", "", token);
		}
	}
	if (stack.size() != 1)
	{
		throw runtime_error("Invalid stack state: should contains only one element");
	}
	return stof(stack.top().ToString());
}
