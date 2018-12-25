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
#include <algorithm>
#include <map>

using namespace::std;

struct Node
{
	Node(const string & name, unsigned int size, unsigned int depth)
	{
		this->name = name;
		this->size = size;
		this->depth = depth;
	}

	string name;
	unsigned int size;
	unsigned int depth;
	Node * parent = nullptr;

	vector<Node *> children;
};

vector<string> SeparationLines(const string & str, char separator = ' ')
{
	vector<string> tokens;
	stringstream stream(str);
	string tmp;
	while (getline(stream, tmp, separator)) tokens.push_back(tmp);
	return tokens;
}

void Do(vector<Node *> && nodes) {
	sort(nodes.begin(), nodes.end(), [](const auto & lhs, const auto & rhs) {
		return lhs->depth > rhs->depth;
	});
	for (const auto & node : nodes) if (node->parent) node->parent->size += node->size;
	for (const auto & node : nodes) cout << node->name + ": " + to_string(node->size) << endl;
}

vector<Node *> ReadTree() {
	stack<Node *> box;
	Node * root = nullptr;
	vector<Node *> nodes;
	string str;
	Node * parent = nullptr;

	while (getline(cin, str))
	{
		const auto tokens = SeparationLines(str);
		const auto depth = tokens[0].size();

		Node * node = new Node(tokens[1], (tokens.size() == 3) ? stoul(tokens[2]) : 0, depth);
		nodes.push_back(node);

		if (nodes.size() == 1) { root = node; box.emplace(node); continue; }

		const auto top = box.top();
		if (depth > top->depth)
		{
			parent = top;
			parent->children.push_back(node);
		}
		else if (depth == top->depth) parent->children.push_back(node);
		else
		{
			while (1 + box.top()->depth != depth) box.pop();
			parent = box.top();
			parent->children.push_back(node);
		}
		node->parent = parent;
		box.emplace(node);
	}
	return nodes;
}

int main() {
	Do(ReadTree());
}
