#ifndef WGRAPH_H
#define WGRAPH_H

#include "../graph/graph.h"
#include "wedge.h"

#include <map>


template <class Vertex>
class wgraph : public graph<Vertex> {
public:

    // default constructor
    wgraph() { }

    // return true and add edge if not already in graph
    bool addEdge(const Vertex &v, const Vertex &w, double c) {
        if (!graph<Vertex>::isVertex(v) || !graph<Vertex>::isVertex(w)) { return false; }
        if (graph<Vertex>::isEdge(v, w)) { return false; }

        graph<Vertex>::addEdge(v, w);
        _c[Edge<Vertex>(v, w)] = c;
        _c[Edge<Vertex>(w, v)] = c;
        return true;
    }

    // return true and add edge if not already in graph
    bool addEdge(const Edge<Vertex> &e, double c) {
        return addEdge(e.v, e.w, c);
    }

    // return true and add edge if not already in graph
    bool addEdge(const WEdge<Vertex> &e) {
        return addEdge(e.v, e.w, e.c);
    }

	// removes edge if in graph
    void removeEdge(const Vertex &v, const Vertex &w) {
        graph<Vertex>::removeEdge(v, w);
        _c.erase(Edge<Vertex>(v, w));
        _c.erase(Edge<Vertex>(w, v));
    }

	// removes edge if in graph
    void removeEdge(const Edge<Vertex> &e) {
        graph<Vertex>::removeEdge(e.v, e.w);
        _c.erase(e);
        _c.erase(e.reverse());
    }

    // return cost of edge
    double cost(const Vertex &v, const Vertex &w) const {
        assert(graph<Vertex>::isVertex(v) && graph<Vertex>::isVertex(w));
        return _c.at(Edge<Vertex>(v, w));
    }

private:
    // cost "red-black tree" representation
    std::map<Edge<Vertex>, double> _c;
};

// input wgraph
template <class Vertex>
std::istream& operator >> (std::istream &is, wgraph<Vertex> &W) {
    std::size_t n, m;
    Vertex v, w;
    double c;

    is >> n >> m;
    for (std::size_t i = 0; i < n; i++) {
		is >> v;
		W.addVertex(v);
	}

	for (std::size_t i = 0; i < m; i++) {
		is >> v >> w >> c;
		W.addEdge(v, w, c);
	}

	return is;
}

// output wgraph
template <class Vertex>
std::ostream& operator << (std::ostream &os, const wgraph<Vertex> &W) {
    os << "\nWeighted Graph:" << std::endl;
	os << "# Vertices: " << W.n() << "\n# Edges: " << W.m() << std::endl << std::endl;

    os << "Vertices:";
	for (auto &v : W.V()) {
		os << " " << v;
	}
	os << std::endl << std::endl;

	os << "Edges: " << std::endl;
	for (auto &v : W.V()) {
		for (auto &w : W.Adj(v)) {
            if (v < w) {
    			os << v << " " << w << " " << W.cost(v, w) << std::endl;
            }
		}
	}
	os << std::endl;

    return os;
}

#endif // WGRAPH_H
