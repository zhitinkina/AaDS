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
#include <map>
#include <set>

using namespace std;

vector<string> SeparationLines(const string & str, char separator = ' ')
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
	set<string> tmp;
	cout << "1. The initial list of edges: " << endl;
	while (getline(strm, str))
	{
		const auto tokens = SeparationLines(str);
		for (size_t i = 0; i < tokens.size(); i += 3)
		{
			const auto start = tokens[i];
			const auto end = tokens[i + 1];
			const auto weight = stoul(tokens[i + 2]);
			edges.emplace_back(make_pair(start, end), weight);
			tmp.emplace(start);
			tmp.emplace(end);
			cout << start << "-" << end << " (" << weight << ")" << endl;
		}
	}

	sort(edges.begin(), edges.end(), [](const auto & lhs, const auto & rhs) {
		return lhs.second < rhs.second;
	});
	cout << "2. Graphs sorted in ascending order: " << endl;
	for (const auto & edge : edges)
	{
		cout << edge.first.first << "-" << edge.first.second << " (" << edge.second << ")" << endl;
	}

	vector<string> tmp2(tmp.begin(), tmp.end());

	vector<size_t> comp;
	size_t totalWeight = 0;
	for (size_t i = 0; i < tmp2.size(); ++i) comp.push_back(i);
	cout << "3. Minimum spanning tree: " << endl;
	for (const auto & edge : edges)
	{
		const auto[first, second] = edge.first;
		const auto weight = edge.second;
		const auto start = distance(tmp2.begin(), find(tmp2.begin(), tmp2.end(), first));
		const auto end = distance(tmp2.begin(), find(tmp2.begin(), tmp2.end(), second));
		//cout << comp[start] << " - " << comp[end] << endl;
		if (comp[start] != comp[end])
		{
			totalWeight += weight;
			const auto a = tmp2[start];
			const auto b = tmp2[end];
			for (size_t i = 0; i < tmp2.size(); ++i)
				if (comp[i] == end) comp[i] = start;
			cout << a << "[" << start << "] - " << b << "[" << end << "]" << tmp2.size() << endl;
		}
	}
	cout << "Total min weight: " << totalWeight << endl;
}
