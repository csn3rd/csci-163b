#ifndef NETWORK_H
#define NETWORK_H

#include "../digraph/digraph.h"
#include "../wgraph/wedge.h"

#include <cassert>
#include <map>
#include <set>

template <class Vertex>
class network : public digraph<Vertex> {
public:

	network() { }

    void addEdge(const Vertex &v, const Vertex &w, double c) {
        digraph<Vertex>::addEdge(v, w);
        _c[Edge<Vertex>(v, w)] = c;
    }

    void addEdge(const WEdge<Vertex> &e) {
        digraph<Vertex>::addEdge(e.v, e.w);
        _c[Edge<Vertex>(e.v, e.w)] = e.c;
    }

    double cost(const Vertex &v, const Vertex &w) const {
        assert(digraph<Vertex>::isEdge(v, w));
        return _c.at(Edge<Vertex>(v, w));
    }

    double cost(const Edge<Vertex> &e) {
        assert(digraph<Vertex>::isEdge(e.v, e.w));
        return _c.at(e);
    }    

    std::set< WEdge<Vertex> > E() const {
        std::set< WEdge<Vertex> > ans;
        for (auto &v : digraph<Vertex>::V()) {
			for (auto &w : digraph<Vertex>::Adj(v)) {
				ans.insert(WEdge<Vertex>(v, w, cost(v,w)));
			}
		}
		return ans;
    }

private:
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

// output wgraph
template <class Vertex>
std::ostream& operator << (std::ostream &os, const network<Vertex> &N) {
	os << "\nNetowrk:" << std::endl;
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
