#include <iostream>
#include <fstream>
#include <cassert>

#include "network.h"
#include "flownetwork.h"

using namespace std;

int main() {
	// network<string> N;
	// cin >> N;
	// cout << N << endl;

	flownetwork<string> F("s","t");
	cin >> F;

	flow<string> mf = F.max_flow();

}
