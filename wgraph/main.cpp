#include <iostream>
#include <fstream>
#include <cassert>

#include "wedge.h"
#include "wgraph.h"
#include "ds.h"

using namespace std;

int main() {
	wgraph<string> W;
	cin >> W;
	cout << W << endl;

	wgraph<string> m = W.Kruskal_MST();
	wgraph<string> n = W.Boruvka_MST();
	cout << m << endl;
	cout << n << endl;
}
