#include <iostream>
#include <fstream>
#include <cassert>

#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "eulerian.h"

using namespace std;

int main() {
	graph<string> G;
	cin >> G;
	cout << G;
}