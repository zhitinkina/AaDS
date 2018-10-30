#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <sstream>
#include <iomanip>

using namespace::std;

enum OperationPriority
{
	DIV = 3,
	MUL = 3,
	MINUS = 2,
	PLUS = 1,
	NONE = 0,
};

bool IsOperator(const string & str)
{
	return (str == "+") || (str == "-")
		|| (str == "*") || (str == "/");
}

bool IsFloat(const string & str)
{
	istringstream stream(str);
	float value;
	stream >> noskipws >> value;
	return stream.eof() && !stream.fail();
}

OperationPriority GetOperationPriority(const string & str)
{
	if (str == "/") return OperationPriority::DIV;
	if (str == "*") return OperationPriority::MUL;
	if (str == "-") return OperationPriority::MINUS;
	if (str == "+") return OperationPriority::PLUS;
	return OperationPriority::NONE;
}

float DoOperation(float op1, float op2, const string & op)
{
	if (op == "+") return op1 + op2;
	if (op == "-") return op1 - op2;
	if (op == "*") return op1 * op2;
	if (op == "/") return op1 / op2;
	throw invalid_argument("Unknown operator '" + op + "'");
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
			? to_string(DoOperation(o1, o2, oper))
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

vector<string> TokenizeString(const string & str, char delimeter = ' ')
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

void ProcessBinaryOperation(stack<shared_ptr<Expression>> & stack, const string & oper)
{
	if (stack.size() < 2)
	{
		throw runtime_error("Invalid stack size: cannot get operands");
	}
	auto op1 = stack.top(); stack.pop();
	auto op2 = stack.top(); stack.pop();

	stack.push(make_shared<Expression>(
		op2->op1() + op2->oper() + op2->op2(),
		oper,
		op1->op1() + op1->oper() + op1->op2()
	));
}

float CalculateRPN(const vector<string> & tokens)
{
	stack<shared_ptr<Expression>> stack;
	for (const auto & token : tokens)
	{
		if (!IsOperator(token))
		{
			stack.push(make_shared<Expression>("", "", token));
		}
		else
		{
			ProcessBinaryOperation(stack, token);
		}
	}
	if (stack.size() != 1)
	{
		throw runtime_error("Invalid stack state: should contains only one element");
	}
	return stof(stack.top()->ToString());
}
