/* 21. Реализовать  алгоритм  Краскала  нахождения   остовного
дерева и проиллюстрировать по шагам этапы его выполнения (10).

Житинкиной Анны, ПС-21
*/

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <stack>

using namespace std;

vector<string> Tokenize(const string & str, char separator = ' ')
{
	vector<string> edges;
	stringstream stream(str);
	string tmp;
	while (getline(stream, tmp, separator))
	{
		edges.push_back(tmp);
	}
	return edges;
}

int main(int argv, char * argc[]) {
	if (argv != 2) {
		cerr << "No open file!" << endl;
		return 1;
	}

	const string fileName = argc[1];
	ifstream strm(fileName);
	if (!strm.is_open()) {
		cerr << "File is not open!" << endl;
		return 1;
	}

	string str;
	vector<pair<pair<string, string>, unsigned>> edges;

	while (getline(strm, str))
	{
		const auto tokens = Tokenize(str);
		for (size_t i = 0; i < tokens.size(); i += 3)
		{
			edges.emplace_back(make_pair(tokens[i], tokens[i + 1]), stoul(tokens[i + 2]));
		}
	}
	sort(edges.begin(), edges.end(), [](const auto & lhs, const auto & rhs) {
		return lhs.second < rhs.second;
	});

	for (const auto & edge : edges)
	{
		cout << edge.first.first << " " << edge.first.second << " " << edge.second << endl;
	}

	const auto tokens = Tokenize(str);
	const auto depth = tokens[0].size();

	int n = edges.size();
	vector <size_t> comp;
	for (size_t i = 0; i < n; ++i) comp.push_back(i);

	int ans = 0;
	for (auto & edge : edges)
	{
		const auto weight = edge.second;
		const auto start = edge.first.first;
		const auto end = edge.first.second;
		if (comp[start] != comp[end])
		{
			ans += weight;
			int a = comp[start];
			int b = comp[end];
			for (int i = 0; i < n; ++i)
				if (comp[i] == b)
					comp[i] = a;
		}
	}
}