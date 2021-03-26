#include <iostream>
#include <fstream>
#include <cassert>

#include "flownetwork.h"

using namespace std;

int main() {
	flownetwork<string> F("s","t");
	cin >> F;

	flow<string> mf = F.max_flow();
}
