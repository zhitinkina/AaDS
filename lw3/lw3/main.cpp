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

vector<string> Tokenize(const string & str, char separator = ' ')
{
	vector<string> tokens;
	stringstream stream(str);
	string tmp;
	while (getline(stream, tmp, separator)) tokens.push_back(tmp);
	return tokens;
}

struct Node
{
	Node(const string & name, size_t size, size_t depth)
	{
		this->name = name;
		this->size = size;
		this->depth = depth;
	}

	string name;
	size_t size;
	size_t depth;
	shared_ptr<Node> parent = nullptr;

	vector<shared_ptr<Node>> childs;
};

void Работать(vector<shared_ptr<Node>> && nodes) {
	sort(nodes.begin(), nodes.end(), [](const auto & lhs, const auto & rhs) {
		return lhs->depth > rhs->depth;
	});
	for (const auto & node : nodes) if (node->parent) node->parent->size += node->size;
	for (const auto & node : nodes) cout << node->name + " [" + to_string(node->size) + "]" << endl;
}

vector<shared_ptr<Node>> ReadTree() {
	vector<string> input;
	stack<shared_ptr<Node>> s;
	shared_ptr<Node> root = nullptr;
	vector<shared_ptr<Node>> nodes;
	string str;
	shared_ptr<Node> parent = nullptr;
	while (getline(cin, str))
	{
		input.push_back(str);
		const auto tokens = Tokenize(input.back());
		const auto depth = tokens[0].size();

		shared_ptr<Node> node = make_shared<Node>(tokens[1], (tokens.size() == 3) ? stoul(tokens[2]) : 0, depth);
		nodes.push_back(node);

		if (nodes.size() == 1) { root = node; s.emplace(node); continue; }

		const auto top = s.top();
		if (depth > top->depth)
		{
			parent = top;
			parent->childs.push_back(node);
		}
		else if (depth == top->depth) parent->childs.push_back(node);
		else
		{
			while (1 + s.top()->depth != depth) s.pop();
			parent = s.top();
			parent->childs.push_back(node);
		}
		node->parent = parent;
		s.emplace(node);
	}
	return nodes;
}

int main() {
	Работать(ReadTree());
}
