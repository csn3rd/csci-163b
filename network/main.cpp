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
}
