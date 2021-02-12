#include <iostream>
#include <fstream>
#include <cassert>

#include "digraph.h"
#include "dfs.h"
#include "tscc.h"

using namespace std;

int main() {
	digraph<string> D;
	cin >> D;
	cout << D;
}