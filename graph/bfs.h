#ifndef BFS_H
#define BFS_H

#include <unordered_map>
#include <queue>

template <class Vertex> class graph;

template <class Vertex>
class BFS {
public:
	BFS(const graph<Vertex> &G, const Vertex &start = Vertex()) {
		_ncc = 0;

		if (G.isVertex(start)) {
			bfs_one(G, start);
			_ncc++;
		}

		for (auto &v : G.V()) {
			if (_D.count(v) == 0) {
				bfs_one(G, v);
				_ncc++;
			}
		}
	}

	void bfs_one(const graph<Vertex> &G, const Vertex &v) {
		std::queue<Vertex> Q;
		Q.push(v);
		_D[v] = 0;
		_C[v] = _ncc;

		while (!Q.empty()) {
			Vertex f = Q.front();
			Q.pop();

			for (auto &w : G.Adj(f)) {
				if (_D.count(w) == 0) {
					Q.push(w);
					_D[w] = 1 + _D[f];
					_P[w] = f;
					_C[w] = _ncc;
				}
			}
		}
	}

	// distances of each vertex to root of component
	std::unordered_map<Vertex, std::size_t> distances() const {
		return _D;
	}

	// parents of each vertex in Breadth First Tree
	std::unordered_map<Vertex, Vertex> bfTree() const {
		return _P;
	}

	// component ids for each vertex
	std::unordered_map<Vertex, std::size_t> components() const {
		return _C;
	}

	// number of connected components
	std::size_t ncc() const {
		return _ncc;
	}

private:
	std::size_t _ncc;
	std::unordered_map<Vertex, std::size_t> _D, _C;
	std::unordered_map<Vertex, Vertex> _P;

};

#endif // BFS_H
