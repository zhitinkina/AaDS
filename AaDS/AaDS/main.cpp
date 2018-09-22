#include <iostream>
#include <fstream>
#include <exception>
#include <vector>
#include <string>
#include <algorithm>

using namespace::std;

enum class Operator
{
	Plus,
	Minus,
	Div,
	Mul,
};

Operator CharToOperator(char ch)
{
	switch (ch)
	{
	case '+':
		return Operator::Plus;
	case '-':
		return Operator::Minus;
	case '*':
		return Operator::Mul;
	case '/':
		return Operator::Div;
	}
	throw invalid_argument("Operator should be [+-*/]. Was sent '" + ch + '\'');
}

char OperatorToChar(Operator oper)
{
	switch (oper)
	{
	case Operator::Plus:
		return '+';
	case Operator::Minus:
		return '-';
	case Operator::Mul:
		return '*';
	case Operator::Div:
		return '/';
	}
	throw invalid_argument("Unknown operator");
}

struct Expression
{
	int firstOperand = 0;
	int secondOperand = 0;
	Operator oper;

	operator float() const
	{
		switch (oper)
		{
		case Operator::Plus:
			return static_cast<float>(firstOperand + secondOperand);
		case Operator::Minus:
			return static_cast<float>(firstOperand - secondOperand);
		case Operator::Mul:
			return static_cast<float>(firstOperand * secondOperand);
		case Operator::Div:
			if (!secondOperand)
			{
				throw invalid_argument("Divizion by zero: " + to_string(firstOperand) + '/' + to_string(secondOperand));
			}
			return static_cast<float>(firstOperand) / secondOperand;
		}
		throw invalid_argument("Unknown operator");
	}
};

ostream & operator<<(ostream & stream, const Expression & expr)
{
	stream << expr.firstOperand << " " << OperatorToChar(expr.oper) << " " << expr.secondOperand;
	return stream;
}

vector<Expression> ParseString(const string & str)
{
	vector<string> tokens;

	vector<size_t> poses;

	size_t prev = 0, pos;
	while ((pos = str.find_first_of("-+/*", prev)) != string::npos)
	{
		if (pos > prev)
		{
			tokens.push_back(str.substr(prev, pos - prev));
			poses.push_back(pos);
		}
		prev = pos + 1;
	}
	if (prev < str.length())
	{
		tokens.push_back(str.substr(prev, string::npos));
	}

	vector<Expression> bla;
	for (unsigned i = 0; i < tokens.size() - 1; ++i)
	{
		if (isdigit(tokens[i].back()) && isdigit(tokens[i+1].front()))
		{
			auto firstNotNow = tokens[i];
			reverse(firstNotNow.begin(), firstNotNow.end());

			auto firstStr = to_string(stoi(firstNotNow));
			reverse(firstStr.begin(), firstStr.end());
			const auto first = stoi(firstStr);
			const auto second = stoi(tokens[i + 1]);
			bla.push_back({ first, second, CharToOperator(str[poses[i]]) });
		}
	}
	
	return bla;
}

Expression GetMaxValueExpression(const string & str)
{
	auto tokens = ParseString(str);
	return *max_element(tokens.begin(), tokens.end());
}

vector<Expression> GetMaxExpressionsInFile(const string & inputFileName)
{
	ifstream stream(inputFileName);

	if (!stream.is_open())
	{
		throw runtime_error("Cannot open file " + inputFileName);
	}

	vector<Expression> expressions;

	string str;
	while (getline(stream, str))
	{
		if (str.empty()) continue;

		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		expressions.push_back(GetMaxValueExpression(str));
	}

	return expressions;
}

int main(int argv, char * argc[])
{
	try
	{
		if (argv != 2)
		{
			throw exception("lw1.exe <input_file_name>");
		}

		auto expressions = GetMaxExpressionsInFile(argc[1]);
		const auto max = max_element(expressions.begin(), expressions.end());
		cout << distance(expressions.begin(), max)
			<< ": "
			<< *max
			<< endl;
	}
	catch (const exception & ex)
	{
		cerr << ex.what() << endl;
	}
}
