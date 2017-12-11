#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <map>
#include <limits.h>
#include <time.h>

using namespace std;

class TreeBuilder
{
	const int INF = INT_MAX;

	map<int, string> *headersMap;
	vector <vector<int> > *graph;

	vector<pair<int, int> > *tree;
public:
	TreeBuilder()
	{
		headersMap = new map<int, string>();
		graph = new vector <vector<int> >();
		
		tree = new vector<pair<int, int> >;
	}

	~TreeBuilder()
	{
		delete headersMap;
		delete graph;

		delete tree;
	}

	void BuildTree()
	{
		prepareGraphToTreeBuild();
		int n = graph->size();

		vector<bool> used(n);
		vector<int> min_e(n, INF), sel_e(n, -1);
		min_e[0] = 0;
		for (int i = 0; i < n; ++i) {
			int v = -1;
			for (int j = 0; j < n; ++j)
				if (!used[j] && (v == -1 || min_e[j] < min_e[v]))
					v = j;
			if (min_e[v] == INF) {
				cout << "No MST!";
				return;
			}

			used[v] = true;
			if (sel_e[v] != -1)
				tree->push_back(make_pair(v, sel_e[v]));

			for (int to = 0; to < n; ++to)
				if ((*graph)[v][to] < min_e[to]) {
					min_e[to] = (*graph)[v][to];
					sel_e[to] = v;
				}
		}
	}

	void InitDataFromFile(string filename)
	{
		ifstream file;
		file.open(filename);
		if (!file)
		{
			cout << "Can't read from file " << filename << endl;
			return;
		}

		string buf;
		getline(file, buf);
		auto headers = parseCsvLine(buf);
		for (int i = 0; i < headers.size(); i++)
		{
			headersMap->insert(make_pair(i, headers[i]));
		}
		graph->resize(headersMap->size());
		
		for (int i = 0; i < graph->size(); i++)
		{
			getline(file, buf);
			auto values = parseCsvLine(buf);
			for (int j = 0; j < graph->size(); j++)
			{
				if (isspace(values[j][0]))
					(*graph)[i].push_back(0);
				else
					(*graph)[i].push_back(stoi(values[j]));
			}
		}
	}

	void WriteTree()
	{
		if (tree->empty())
		{
			cout << "Tree is empty!" << endl;
			return;
		}
	int sum_weight = 0;
		for (int i = 0; i < tree->size(); i++)
		{
			auto edge = (*tree)[i];
			sum_weight += (*graph)[edge.first][edge.second];
			cout << (*headersMap)[edge.first] << " " << (*headersMap)[edge.second] << " "
				<< (*graph)[edge.first][edge.second] << endl;
		}
		cout << "Tree weight " << sum_weight << endl;
	}

private:
	vector<string> parseCsvLine(string line)
	{
		size_t pos = 0;
		string delimiter = ";";
		string token;
		vector<string> result;
		while ((pos = line.find(delimiter)) != string::npos) {
			token = line.substr(0, pos);
			result.push_back(token);
			line.erase(0, pos + delimiter.length());
		}

		return result;
	}

	void prepareGraphToTreeBuild()
	{
		for (int i = 0; i < graph->size(); i++)
		{
			for (int j = 0; j < graph->size(); j++)
			{
				if ((*graph)[i][j] == 0)
					(*graph)[i][j] = INF;
			}
		}
	}
};

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "Input file is required" << endl;
		cout << "Using: SpanningTreeBuild.exe Table.russia_ansi.txt" << endl;
		return -1;
	}

	setlocale(LC_CTYPE, "rus");

	TreeBuilder builder;
	builder.InitDataFromFile(argv[1]);
	auto start_time = clock();
	builder.BuildTree();
	builder.WriteTree();
	auto finish_time = clock();
	cout << "Elapsed time " << finish_time - start_time << "ms"<< endl;
}
