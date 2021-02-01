#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <fstream>
#include <cassert>

template <class Vertex> class BFS;
template <class Vertex> class DFS;
template <class Vertex> class Eulerian;

template <class Vertex>
class graph {
public:
	typedef std::unordered_set<Vertex> VertexSet;

	// default constructor
	graph() { }

	// number of vertices
	std::size_t n() const {
		return _t.size();
	}

	// number of edges
	std::size_t m() const {
		std::size_t ans(0);
		for (auto &p : _t) {
			ans += p.second.size();
		}
		return ans/2;
	}

	// return set of all vertices
	VertexSet V() const {
		VertexSet ans;
		for (auto &p : _t) {
			ans.insert(p.first);
		}
		return ans;
	}

	// return true if vertex is in graph
	bool isVertex(const Vertex &v) const {
		return (_t.count(v) != 0);
	}

	// return true if edge is in graph
	bool isEdge(const Vertex &v, const Vertex &w) const {
		return isVertex(v) && isVertex(w) && (_t.at(v).count(w) != 0);
	}

	// return set of adjacent vertices
	VertexSet Adj(const Vertex &v) const {
		assert(isVertex(v));
		return _t.at(v);
	}

	// return degree / number of adjacent vertices
	std::size_t deg(const Vertex &v) const {
		assert(isVertex(v));
		return _t.at(v).size();
	}

	// return true and add vertex if not already in graph
	bool addVertex(const Vertex &v) {
		if (isVertex(v)) { return false; }

		_t[v] = VertexSet();
		return true;
	}

	// return true and add edge if not already in graph
	bool addEdge(const Vertex &v, const Vertex &w) {
		if (!isVertex(v) || !isVertex(w)) { return false; }
		if (isEdge(v, w)) { return false; }

		_t[v].insert(w);
		_t[w].insert(v);
		return true;
	}

	// removes vertex if in graph
	void removeVertex(const Vertex &v) {
		assert(isVertex(v));

		for (auto &p : _t) {
			p.second.remove(v);
		}

		_t.remove(v);
	}

	// removes edge if in graph
	void removeEdge(const Vertex &v, const Vertex &w) {
		assert(isVertex(v) && isVertex(w) && isEdge(v, w));
		_t[v].erase(w);
		_t[w].erase(v);
	}

	// number of connected components
	std::size_t ncc() const {
		BFS<Vertex> b(*this);
		return b.ncc();

		// DFS<Vertex> d(*this);
		// return d.ncc();
	}

	// return true if connected
	bool isConnected() const {
		return (ncc() == 1);
	}

	// return true if no cycles
	bool isAcyclic() const {
		return (n() == m() + ncc());
	}

	// return true if tree
	bool isTree() const {
		return isConnected() && isAcyclic();
	}

private:
	// adjacency "hashmap" representation
	std::unordered_map<Vertex, VertexSet> _t;

};

// input graph
template <class Vertex>
std::istream& operator >> (std::istream &is, graph<Vertex> &G) {
	std::size_t n, m;
	Vertex v, w;

	G = graph<Vertex>();

	is >> n >> m;
	for (std::size_t i = 0; i < n; i++) {
		is >> v;
		G.addVertex(v);
	}

	for (std::size_t i = 0; i < m; i++) {
		is >> v >> w;
		G.addEdge(v, w);
	}

	return is;
}

// output graph
template <class Vertex>
std::ostream& operator << (std::ostream &os, graph<Vertex> &G) {
	os << "\nGraph:" << std::endl;
	os << "# Vertices: " << G.n() << "\n# Edges: " << G.m() << std::endl << std::endl;

	os << "Vertices:";
	for (auto &v : G.V()) {
		os << " " << v;
	}
	os << std::endl << std::endl;

	os << "Edges: " << std::endl;
	for (auto &v : G.V()) {
		for (auto &w : G.Adj(v)) {
			if (v < w) {
				os << v << " " << w << std::endl;
			}
		}
	}
	os << std::endl;

	os << "BFS:" << std::endl;

	BFS<Vertex> b(G);
	std::unordered_map<Vertex, std::size_t> _D = b.distances();
	std::unordered_map<Vertex, Vertex> _P = b.bfTree();
	std::unordered_map<Vertex, std::size_t> _C = b.components();

	os << "# Connected Components: " << b.ncc() << std::endl;
	os << std::endl;

	os << "Distances:" << std::endl;
	for (auto &d : _D) {
		os << d.first << ": " << d.second << std::endl;
	}
	os << std::endl;

	os << "Parents:" << std::endl;
	for (auto &p : _P) {
		os << p.first << ": " << p.second << std::endl;
	}
	os << std::endl;

	os << "Components:" << std::endl;
	for (auto &c : _C) {
		os << c.first << ": " << c.second << std::endl;
	}
	os << std::endl;

	os << "DFS:" << std::endl;

	DFS<Vertex> d(G);
	std::unordered_map<Vertex, std::size_t> _pre = d.pre();
	std::unordered_map<Vertex, std::size_t> _post = d.post();
	std::unordered_map<Vertex, std::size_t> _low = d.low();
	_P = d.dfTree();
	_C = d.components();

	os << "# Connected Components: " << d.ncc() << std::endl;
	os << std::endl;

	os << "Pre:" << std::endl;
	for (auto &p : _pre) {
		os << p.first << ": " << p.second << std::endl;
	}
	os << std::endl;

	os << "Post:" << std::endl;
	for (auto &p : _post) {
		os << p.first << ": " << p.second << std::endl;
	}
	os << std::endl;

	os << "Low:" << std::endl;
	for (auto &l : _low) {
		os << l.first << ": " << l.second << std::endl;
	}
	os << std::endl;

	os << "Parents:" << std::endl;
	for (auto &p : _P) {
		os << p.first << ": " << p.second << std::endl;
	}
	os << std::endl;

	os << "Components:" << std::endl;
	for (auto &c : _C) {
		os << c.first << ": " << c.second << std::endl;
	}
	os << std::endl;

	if (G.isTree()) {
		os << "Graph is connected and acyclic." << std::endl;
	} else if (G.isConnected()) {
		os << "Graph is connected but not acyclic." << std::endl;
	} else if (G.isAcyclic()) {
		os << "Graph is acyclic but not connected." << std::endl;
	} else {
		os << "Graph is not connected or acyclic." << std::endl;
	}
	os << std::endl;

	Eulerian<Vertex> e(G);
	if (e.hasCycle()) {
		os << "Graph is Eulerian." << std::endl;

		os << "Eulerian Cycle:";
		std::vector<Vertex> path = e.path();
		for (auto &v : path) {
			os << " " << v;
		}
		os << std::endl << std::endl;
	} else if (e.hasPath()) {
		os << "Graph is Semi-Eulerian." << std::endl;

		os << "Eulerian Path:";
		std::vector<Vertex> path = e.path();
		for (auto &v : path) {
			os << " " << v;
		}
		os << std::endl << std::endl;
	}
	
	return os;
}

#endif // GRAPH_H
