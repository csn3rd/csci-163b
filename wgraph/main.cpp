#include <iostream>
#include <fstream>
#include <cassert>

#include "wedge.h"
#include "wgraph.h"
#include "ds.h"
#include "dary_heap.h"

using namespace std;

int main() {
	wgraph<string> W;
	cin >> W;
	cout << W << endl;

	wgraph<string> m = W.Kruskal_MST();
	cout << m << endl;

	wgraph<string> n = W.Boruvka_MST();
	cout << n << endl;
}
