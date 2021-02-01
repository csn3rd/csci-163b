#ifndef DFS_H
#define DFS_H

#include <unordered_map>
#include <list>

template <class Vertex> class digraph;

template <class Vertex>
class DFS {
public:
	DFS(const digraph<Vertex> &D, const Vertex &start = Vertex()) {
		_time = 0;

		if (D.isVertex(start)) {
			dfs_one(D, start);
		}

		for (auto &v : D.V()) {
			if (_pre.count(v) == 0) {
				dfs_one(D, v);
			}
		}

		for (auto &v : D.V()) {
			_back.addVertex(v);
			_cross.addVertex(v);
			_forward.addVertex(v);
		}

		for (auto &v : D.V()) {
			for (auto &w : D.Adj(v)) {
				int type = edge_type(v, w);
				if (type == 2) {
					_forward.addEdge(v, w);
				} else if (type == 1) {
					_back.addEdge(v, w);
				} else if (type == 3) {
					_cross.addEdge(v, w);
				}
			}
		}
	}

	void dfs_one(const digraph<Vertex> &D, const Vertex &v) {
		_pre[v] = _time++;
		for (auto &w : D.Adj(v)) {
			if (_pre.count(w) == 0) {
				_P[w] = v;
				dfs_one(D, w);
			}
		}
		_post[v] = _time++;
		_ts.push_front(v);
	}

	// pre times for each vertex
	std::unordered_map<Vertex, std::size_t> pre() const {
		return _pre;
	}

	// post times for each vertex
	std::unordered_map<Vertex, std::size_t> post() const {
		return _post;
	}

	// parents of each vertex in Depth First Tree
	std::unordered_map<Vertex, Vertex> dfTree() const {
		return _P;
	}

	// digraph of back edges
	digraph<Vertex> back() const {
		return _back;
	}

	// digraph of forward edges
	digraph<Vertex> forward() const {
		return _forward;
	}

	// digraph of cross edges
	digraph<Vertex> cross() const {
		return _cross;
	}

	// returns topological order of vertices
	std::list<Vertex> ts() const {
		return _ts;
	}

	// returns type of edge where 0: tree 1: back 2: forward 3: cross
	int edge_type(const Vertex &v, const Vertex &w) const {
		assert(_pre.count(v) && _pre.count(w));
		if (_P.count(w) == 1 && _P.at(w) == v) {
			return 0;
		}
		if (_pre.at(v) < _pre.at(w)) {
			return 2;
		}
		if (_post.at(v) < _post.at(w)) {
			return 1;
		}
		return 3;
	}

private:
	std::unordered_map<Vertex, std::size_t> _pre, _post;
	std::unordered_map<Vertex, Vertex> _P;
	std::size_t _time;
	digraph<Vertex> _back, _cross, _forward;
	std::list<Vertex> _ts;

};

#endif // DFS_H
