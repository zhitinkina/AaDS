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
	Expression() = delete;
	Expression(string op1, string oper, string op2)
	{
		m_op1 = op1;
		m_op2 = op2;
		m_oper = oper;

		float o1 = 0;
		float o2 = 0;
		if (!oper.empty())
		{
			try
			{
				if (!op1.empty() && !op2.empty())
				{
					o1 = stof(op1);
					o2 = stof(op2);
				}
				else if (!op1.empty())
				{
					o1 = stof(op1);
				}
				else if (!op2.empty())
				{
					o2 = stof(op2);
				}
			}
			catch (...) {}
		}
		else
		{
			try
			{
				o2 = stof(op2);
			}
			catch (...) {}
		}

		m_result = (!oper.empty())
			? to_string(DoOperation(oper, o1, o2))
			: to_string(o2);
		m_op1 = "";
		m_oper = "";
		m_op2 = m_result;
	}

	string op1() const { return m_op1; }
	string oper() const { return m_oper; }
	string op2() const { return m_op2; }

	virtual string ToString() const
	{
		return m_op1 + m_oper + m_op2;
	}

private:
	string m_op1;
	string m_oper;
	string m_op2;

	string m_result = "[NotCalculated]";
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
				op2.op1() + op2.oper() + op2.op2(),
				token,
				op1.op1() + op1.oper() + op1.op2()
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
