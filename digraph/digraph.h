#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <iostream>
#include <fstream>
#include <cassert>

template <class Vertex> class DFS;
template <class Vertex> class TSCC;

template <class Vertex>
class digraph {
public:
	typedef std::unordered_set<Vertex> VertexSet;

	// default constructor
	digraph() { }

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
		return ans;
	}

	// return set of all vertices
	VertexSet V() const {
		VertexSet ans;
		for (auto &p : _t) {
			ans.insert(p.first);
		}
		return ans;
	}

	// check if vertex is in graph
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

	// returns out degree / number of adjacent vertices from v
	std::size_t outdeg(const Vertex &v) const {
		assert(isVertex(v));
		return _t.at(v).size();
	}

	// return in degree / number of adjacent vertices to v
	std::size_t indeg(const Vertex &v) const {
		assert(isVertex(v));

		std::size_t ans(0);
		for (auto &p : _t) {
			ans += p.second.count(v);
		}
		return ans;
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
		_t[v].remove(w);
	}

	// return true if no cycles
	bool isDAG() const {
		DFS<Vertex> d(*this);
		digraph<Vertex> back = d.back();
		return (back.m() == 0);
	}

	// return topological order of vertices if is DAG
	std::list<Vertex> ts() const {
		assert(isDAG());
		DFS<Vertex> d(*this);
		return d.ts();
	}

	// return the reverse digraph
	digraph<Vertex> reverse() const {
		digraph<Vertex> rev;
		for (auto &v : V()) {
			rev.addVertex(v);
		}
		for (auto &v : V()) {
			for (auto &w : Adj(v)) {
				rev.addEdge(w, v);
			}
		}
		return rev;
	}

	// return the strongly connected components using Kosaraju's algorithm
	std::unordered_map<Vertex, std::size_t> Kscc() const {
		DFS<Vertex> D1(reverse());
		DFS<Vertex> D2(*this, D1.ts());
		return D2.components();
	}

private:
	// adjacency "hashmap" representation
	std::unordered_map<Vertex, VertexSet> _t;

};

// input digraph
template <class Vertex>
std::istream& operator >> (std::istream &is, digraph<Vertex> &D) {
	std::size_t n, m;
	Vertex v, w;

	D = digraph<Vertex>();

	is >> n >> m;
	for (std::size_t i = 0; i < n; i++) {
		is >> v;
		D.addVertex(v);
	}

	for (std::size_t i = 0; i < m; i++) {
		is >> v >> w;
		D.addEdge(v, w);
	}

	return is;
}


// output digraph
template <class Vertex>
std::ostream& operator << (std::ostream &os, digraph<Vertex> &D) {
	os << "\nDigraph:" << std::endl;
	os << "# Vertices: " << D.n() << "\n# Edges: " << D.m() << std::endl << std::endl;

	os << "Vertices:";
	for (auto &v : D.V()) {
		os << " " << v;
	}
	os << std::endl << std::endl;

	os << "Edges: " << std::endl;
	for (auto &v : D.V()) {
		for (auto &w : D.Adj(v)) {
			os << v << " " << w << std::endl;
		}
	}
	os << std::endl;

	os << "DFS:" << std::endl;

	DFS<Vertex> d(D);
	std::unordered_map<Vertex, std::size_t> _pre = d.pre();
	std::unordered_map<Vertex, std::size_t> _post = d.post();
	std::unordered_map<Vertex, Vertex> _P = d.dfTree();
	std::size_t nback = d.back().m();
	std::size_t nforward = d.forward().m();
	std::size_t ncross = d.cross().m();

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

	os << "Parents:" << std::endl;
	for (auto &p : _P) {
		os << p.first << ": " << p.second << std::endl;
	}
	os << std::endl;

	os << "Back Edges: " << nback << std::endl;
	for (auto &v : d.back().V()) {
		for (auto &w : d.back().Adj(v)) {
			os << v << " " << w << std::endl;
		}
	}
	os << std::endl;

	os << "Forward Edges: " << nforward << std::endl;
	for (auto &v : d.forward().V()) {
		for (auto &w : d.forward().Adj(v)) {
			os << v << " " << w << std::endl;
		}
	}
	os << std::endl;

	os << "Cross Edges: " << ncross << std::endl;
	for (auto &v : d.cross().V()) {
		for (auto &w : d.cross().Adj(v)) {
			os << v << " " << w << std::endl;
		}
	}
	os << std::endl;

	os << std::endl;

	if (D.isDAG()) {
		os << "Graph is acyclic, a \"DAG\"." << std::endl;
		os << "Topological Order:";
		std::list<Vertex> ts = D.ts();
		for (auto &v : ts) {
			os << " " << v;
		}
		os << std::endl;
	} else {
		os << "Graph is not acyclic." << std::endl;
	}
	os << std::endl;

	os << "Kosaraju Strongly Connected Components:" << std::endl;
	std::unordered_map<Vertex, std::size_t> _C = D.Kscc();
	for (auto &c : _C) {
		os << c.first << ": " << c.second << std::endl;
	}
	os << std::endl;

	os << "Tarjan Strongly Connected Components:" << std::endl;
	TSCC<Vertex> t(D);
	_C = t.components();
	for (auto &c : _C) {
		os << c.first << ": " << c.second << std::endl;
	}
	os << std::endl;

	return os;
}

#endif // DIGRAPH_H
