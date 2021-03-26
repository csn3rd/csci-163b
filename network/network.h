#ifndef NETWORK_H
#define NETWORK_H

#include "../digraph/digraph.h"
#include "../digraph/dfs.h"
#include "../digraph/tscc.h"
#include "../wgraph/wedge.h"

#include <cassert>
#include <map>
#include <unordered_map>
#include <set>
#include <limits>

template <class Vertex>
class network : public digraph<Vertex> {
public:

	network() { }

	// add edge if not already in graph
	void addEdge(const Vertex &v, const Vertex &w, double c) {
		digraph<Vertex>::addEdge(v, w);
		_c[Edge<Vertex>(v, w)] = c;
	}

	// add edge if not already in graph
	void addEdge(const WEdge<Vertex> &e) {
		digraph<Vertex>::addEdge(e.v, e.w);
		_c[Edge<Vertex>(e.v, e.w)] = e.c;
	}

	// return cost of edge
	double cost(const Vertex &v, const Vertex &w) const {
		assert(digraph<Vertex>::isEdge(v, w));
		return _c.at(Edge<Vertex>(v, w));
	}

	// return cost of edge
	double cost(const Edge<Vertex> &e) {
		assert(digraph<Vertex>::isEdge(e.v, e.w));
		return _c.at(e);
	}    

	// update the cost of edge
	void setCost(const Vertex &v, const Vertex &w, double newcost) {
		assert(digraph<Vertex>::isEdge(v, w));
		_c[Edge<Vertex>(v,w)] = newcost;
	}

	// return set of all edges
	std::set< WEdge<Vertex> > E() const {
		std::set< WEdge<Vertex> > ans;
		for (auto &v : digraph<Vertex>::V()) {
			for (auto &w : digraph<Vertex>::Adj(v)) {
				ans.insert(WEdge<Vertex>(v, w, cost(v,w)));
			}
		}
		return ans;
	}

	// return the shortest distances from start vertex using Bellman Ford's Algorithm
	network<Vertex> Bellman_Ford(const Vertex &s) const {
		network<Vertex> ans;
		std::unordered_map<Vertex, double> d, D;    // old and new distances from s
		std::unordered_map<Vertex, Vertex> parent;  // parent
		bool nwc(false);

		for (auto &v : digraph<Vertex>::V()) {
			d[v] = std::numeric_limits<double>::infinity();
		}
		d[s] = 0.0;

		for (std::size_t i = 1; i < digraph<Vertex>::n(); i++) {
			D = d;
			for (auto &e : E()) {
				double temp = d[e.v] + e.c;
				if (temp < D[e.w]) {
					D[e.w].temp;
					parent[e.w] = e.v;
					if (i == digraph<Vertex>::n()) {
						nwc = true;
					}
				}
			}
			d = D;
		}

		for (auto &v : digraph<Vertex>::V()) {
			ans.digraph<Vertex>::addVertex(v);
		}

		for (auto &p : parent) {
			ans.addEdge(p.second, p.first, cost(p.second, p.first));
		}

		return ans;
	}

	// return the shortest distances between all vertices using Floyd Warshall's Algorithm
	network<Vertex> Floyd_Warshall() const {
		network<Vertex> ans;

		for (auto &v : digraph<Vertex>::V()) {
			ans.addVertex(v);
		}

		for (auto &v : ans.V()) {
			for (auto &w : ans.V()) {
				if (v == w) {
					ans.addEdge(v, w, 0.0);
				} else if (digraph<Vertex>::isEdge(v, w)) {
					ans.addEdge(v, w, cost(v, w));
				} else {
					ans.addEdge(v, w, std::numeric_limits<double>::infinity());
				}
			}
		}

		for (auto &m : ans.V()) {
			for (auto &s : ans.V()) {
				for (auto &d : ans.V()) {
					ans.setCost(s, d, std::min(ans.cost(s, d), ans.cost(s, m) + ans.cost(m, d)));
				}
			}
		}

		return ans;
	}

private:
	// cost "red-black tree" representation
	std::map<Edge<Vertex>, double> _c; 
};


// input network
template <class Vertex>
std::istream& operator >> (std::istream &is, network<Vertex> &N) {
	std::size_t n, m;
	Vertex v, w;
	double c;

	is >> n >> m;
	for (std::size_t i = 0; i < n; i++) {
		is >> v;
		N.addVertex(v);
	}

	for (std::size_t i = 0; i < m; i++) {
		is >> v >> w >> c;
		N.addEdge(v, w, c);
	}

	return is;
}

// output network
template <class Vertex>
std::ostream& operator << (std::ostream &os, const network<Vertex> &N) {
	os << "\nNetwork:" << std::endl;
	os << "# Vertices: " << N.n() << "\n# Edges: " << N.m() << std::endl << std::endl;

	os << "Vertices:";
	for (auto &v : N.V()) {
		os << " " << v;
	}
	os << std::endl << std::endl;

	os << "Edges: " << std::endl;
	for (auto &v : N.V()) {
		for (auto &w : N.Adj(v)) {
			os << v << " " << w << " " << N.cost(v, w) << std::endl;
		}
	}
	os << std::endl;

	return os;
}

#endif // NETWORK_H
