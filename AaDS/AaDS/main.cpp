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

struct Expression
{
	int firstOperand = 0;
	int secondOperand = 0;
	char oper;

	float ToFloat() const
	{
		switch (oper)
		{
		case '+':
			return static_cast<float>(firstOperand + secondOperand);
		case '-':
			return static_cast<float>(firstOperand - secondOperand);
		case '*':
			return static_cast<float>(firstOperand * secondOperand);
		case '/':
			if (!secondOperand)
			{
				throw invalid_argument("Divizion by zero: " + to_string(firstOperand) + '/' + to_string(secondOperand));
			}
			return static_cast<float>(firstOperand) / secondOperand;
		}
		throw invalid_argument("Unknown operator");
	}
};

int GetIntFromBack(const string & str)
{
	string tmp;
	for (size_t i = str.size(); i != 0; --i)
	{
		const auto ch = str[i - 1];
		if (!isdigit(ch)) break;
		tmp.insert(tmp.begin(), ch);
	}
	tmp = (str.front() == '-') ? '-' + tmp : tmp;
	return stoi(tmp);
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
			auto token = str.substr(prev, pos - prev);
			tokens.push_back((prev && str[prev - 1] == '-')
				? "-" + token
				: token
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
			const auto first = GetIntFromBack(tokens[i]);
			const auto second = stoi(tokens[i + 1]);
			expressions.push_back({ first, second, str[poses[i]] });
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
	const Expression minExpr = { numeric_limits<int>::lowest(), 1, '*' };
	string str;
	while (getline(stream, str))
	{
		if (str.empty())
		{
			expressions.push_back(minExpr);
			continue;
		}

		const auto tokens = ParseString(str);
		expressions.push_back(tokens.empty()
			? minExpr
			: GetMaxValueExpression(tokens)
		);
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
		cout << 1 + distance(expressions.begin(), max)
			<< ": "
			<< max->ToFloat()
			<< endl;
	}
	catch (const exception & ex)
	{
		cerr << ex.what() << endl;
	}
}
