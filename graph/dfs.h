#ifndef DFS_H
#define DFS_H

#include <unordered_map>

template <class Vertex> class graph;

template <class Vertex>
class DFS {
public:
	DFS(const graph<Vertex> &G, const Vertex &start = Vertex()) {
		_ncc = _time = 0;

		if (G.isVertex(start)) {
			dfs_one(G, start);
			_ncc++;
		}

		for (auto &v : G.V()) {
			if (_pre.count(v) == 0) {
				dfs_one(G, v);
				_ncc++;
			}
		}
	}

	void dfs_one(const graph<Vertex> &G, const Vertex &v) {
		_pre[v] = _low[v] = _time++;
		_C[v] = _ncc;
		for (auto &w : G.Adj(v)) {
			if (_pre.count(w) == 0) {
				_P[w] = v;
				dfs_one(G, w);
				_low[v] = std::min(_low[v], _low[w]);
			} else if (_P.count(v) == 0 || _P[v] != w) {
				_low[v] = std::min(_low[v], _pre[w]);
			}
		}
		_post[v] = _time++;
	}

	// pre times for each vertex
	std::unordered_map<Vertex, std::size_t> pre() const {
		return _pre;
	}

	// post times for each vertex
	std::unordered_map<Vertex, std::size_t> post() const {
		return _post;
	}

	// low times for each vertex
	std::unordered_map<Vertex, std::size_t> low() const {
		return _low;
	}

	// parents of each vertex in Depth First Tree
	std::unordered_map<Vertex, Vertex> dfTree() const {
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

	// return true if edge between v and w is bridge
	bool isBridge(const Vertex &v, const Vertex &w) const {
		return ((_P.count(v) == 1 && _P[v] == w && _low[v] >= _pre[v])
			|| (_P.count(w) == 1 && _P[w] == v && _low[w] >= _pre[w]));
	}

	// return true if vertex v is an articulation point
	bool isArticulation(const Vertex &v) const {
		if (_P.count(v) == 0) {				// root has more than one child
			int nchildren(0);
			for (auto &p : _P) {
				if (p.second == v) {
					nchildren++;
				}
			}
			return (nchildren >= 2);
		} else {							// pre time smaller than low of children 
			for (auto &p : _P) {
				if (p.second == v && _low[p.first] >= _pre[v]) {
					return true;
				}
			}
			return false;
		}
	}

private:
	std::size_t _ncc, _time;
	std::unordered_map<Vertex, std::size_t> _pre, _post, _low, _C;
	std::unordered_map<Vertex, Vertex> _P;

};

#endif // DFS_H
