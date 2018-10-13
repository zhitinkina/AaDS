/*15. В некоторых строках текстового файла имеются выражения,
состоящие   из   двух   целых   чисел,   разделенных    знаком
арифметической   операции ('+','-','*','/'). Первое  из  чисел
может быть отрицательным. В строке может содержаться несколько
выражений. Перед  выражением  и  после него  могут  находиться
произвольные символы. Требуется  выделить  строку,  в  которой
значение выражения максимально. Вывести найденное максимальное
значение (7).

Житинкина Анна, ПС-21
*/

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

	float ToFloat() const
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

vector<Expression> ParseString(const string & str)
{
	vector<string> tokens;

	vector<size_t> poses;

	size_t prev = 0, pos;
	while ((pos = str.find_first_of("-+/*", prev)) != string::npos)
	{
		if (pos > prev)
		{
			auto bla = str.substr(prev, pos - prev);
			tokens.push_back((prev && str[prev - 1] == '-')
				? "-" + bla
				: bla
			);
			
			poses.push_back(pos);
		}
		prev = pos + 1;
	}
	if (prev < str.length())
	{
		tokens.push_back(str.substr(prev, string::npos));
	}

	vector<Expression> expressions;
	for (size_t i = 0; i < tokens.size() - 1; ++i)
	{
		if (isdigit(tokens[i].back()) && isdigit(tokens[i+1].front()))
		{
			auto firstNotNow = tokens[i];
			reverse(firstNotNow.begin(), firstNotNow.end());

			auto firstStr = to_string(stoi(firstNotNow));
			reverse(firstStr.begin(), firstStr.end());
			firstStr = (firstNotNow.back() == '-') ? '-' + firstStr : firstStr;
			const auto first = stoi(firstStr);
			const auto second = stoi(tokens[i + 1]);
			expressions.push_back({ first, second, CharToOperator(str[poses[i]]) });
		}
	}
	
	return expressions;
}

Expression GetMaxValueExpression(const vector<Expression> & expressions)
{
	return *max_element(expressions.begin(), expressions.end(), [](const auto & lhs, const auto & rhs) {
		return lhs.ToFloat() < rhs.ToFloat();
	});
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
		const auto tokens = ParseString(str);
		if (!tokens.empty())
		{
			expressions.push_back(GetMaxValueExpression(tokens));
		}
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

		const string fileName = argc[1];
		auto expressions = GetMaxExpressionsInFile(fileName);
		if (expressions.empty())
		{
			throw exception(string("No tokens in file " + fileName).c_str());
		}
		const auto max = max_element(expressions.begin(), expressions.end(), [](const auto & lhs, const auto & rhs) {
			return lhs.ToFloat() < rhs.ToFloat();
		});
		cout << distance(expressions.begin(), max)
			<< ": "
			<< max->ToFloat()
			<< endl;
	}
	catch (const exception & ex)
	{
		cerr << ex.what() << endl;
	}
}
