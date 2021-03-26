#include <iostream>
#include <fstream>
#include <cassert>

#include "network.h"

using namespace std;

int main() {
	network<string> N;
	cin >> N;
	cout << N << endl;

	network<string> apsp = N.Floyd_Warshall();
	cout << apsp << endl;

	network<string> sssp1 = N.Bellman_Ford("0");
	cout << sssp1 << endl;
	
	network<string> sssp2 = N.Dijkstra("0");
	cout << sssp2 << endl;
}