/*
   4. В    листьях    дерева,    соответствующего    некоторой
конструкции,  указаны  значения  массы.  Масса  сборного  узла
определяется как сумма масс  составляющих  деталей.  Требуется
определить  значение  массы  для каждого узла конструкции,  не
превосходящего заданного уровня от вершины дерева (9).

Житинкина Анна, ПС-21
*/

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <memory>
#include <algorithm>
#include <map>
#include <fstream>

using namespace::std;

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

struct Node
{

	Node(const string & name, size_t size, size_t depth)
		:name(name)
		,size(size)
		,depth(depth)
	{}

	string name;
	size_t size;
	size_t depth;
	shared_ptr<Node> parent = nullptr;

	vector<shared_ptr<Node>> childs;
};

int main()
{
	vector<string> input;

	stack<shared_ptr<Node>> s;
	size_t rootChecker = 0;
	string str;

	shared_ptr<Node> root = nullptr;
	shared_ptr<Node> parent = nullptr;

	vector<shared_ptr<Node>> nodes;

	while (getline(cin, str))
	{
		input.push_back(str);
		auto tokens = Tokenize(str);
		auto depth = tokens[0].size();
		auto name = tokens[1];
		auto size = (tokens.size() == 3) ? stoul(tokens[2]) : 0;

		shared_ptr<Node> node = make_shared<Node>(name, size, depth);
		nodes.push_back(node);

		if (!rootChecker) { root = node; ++rootChecker; s.emplace(node); continue; }
		++rootChecker;

		auto top = s.top();
		if (depth > top->depth)
		{
			parent = top;
			parent->childs.push_back(node);
		}
		else if (depth == top->depth)
		{
			parent->childs.push_back(node);
		}
		else
		{
			while (s.top()->depth != depth - 1) { s.pop(); }
			parent = s.top();
			parent->childs.push_back(node);
		}
		node->parent = parent;
		s.emplace(node);
	}

	sort(nodes.begin(), nodes.end(), [](const auto & lhs, const auto & rhs) {
		return lhs->depth > rhs->depth;
	});

	for (const auto & node : nodes)
	{
		if (node->parent) node->parent->size += node->size;
	}

	for (const auto & node : nodes)
	{
		cout << node->name + " [" + to_string(node->size) + "]" << endl;
	}
}
