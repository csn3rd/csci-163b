#ifndef EULERIAN_H
#define EULERIAN_H

#include <vector>

template <class Vertex> class graph;

template <class Vertex>
class Eulerian {
public:
	// A graph is Eulerian if no vertices have odd degree
	// A graph is Semi-Eulerian if only two vertices have odd degree
	Eulerian(graph<Vertex> G) {
		_hasCycle = _hasPath = false;
		Vertex start;

		if (G.n() == 0) {
		  return;
		}

		std::size_t oddCount(0);
		for (auto &v : G.V()) {
			if (G.deg(v) % 2 == 1) {
				oddCount++;
				start = v;
			}
		}

		switch(oddCount) {
			case 0:			// Eulerian
				_hasCycle = _hasPath = true;
				start = *(G.V().begin());
				break;
			case 2:			// Semi-Eulerian
				_hasPath = true;
				break;
			default:		// Neither
				return;
		}

		_path = findPath(G, start);

	}

	// true if Eulerian
	bool hasCycle() const {
		return _hasCycle;
	}

	// true if Semi-Eulerian
	bool hasPath() const {
		return _hasPath;
	}

	// vector of vertices visited on Eulerian trail	
	std::vector<Vertex> path() const {
		return _path;
	}

private:
	bool _hasCycle, _hasPath;
	std::vector<Vertex> _path;

	// start from vertex v and follow any edges
	std::vector<Vertex> findPath(graph<Vertex> &G, const Vertex &v) {
		std::vector<Vertex> ans;
		while (G.deg(v) > 0) {
			Vertex w = *(G.Adj(v).begin());
			G.removeEdge(v, w);
			std::vector<Vertex> p = findPath(G, w);
			ans.insert(ans.end(), p.begin(), p.end());
		}
		ans.push_back(v);
		return ans;
	}

};

#endif // EULERIAN_H
