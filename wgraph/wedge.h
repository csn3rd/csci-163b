#ifndef WEDGE_H
#define WEDGE_H

template <class Vertex>
struct Edge {
    Vertex v, w;

    Edge(const Vertex &v, const Vertex &w) : v(v), w(w) { }

    // returns the reverse of the edge
    Edge<Vertex> reverse() const {
        return Edge<Vertex>(w, v);
    }
};

// compare edges
template <class Vertex>
bool operator < (const Edge<Vertex> &e1, const Edge<Vertex> &e2) {
    return ((e1.v < e2.v) || (e1.v == e2.v && e1.w < e2.w));
}

// output edge 
template <class Vertex>
std::ostream& operator << (std::ostream &os, const Edge<Vertex> &e) {
    os << "{" << e.v << ", " << e.w << "}";
    return os;
}

template <class Vertex>
struct WEdge : public Edge<Vertex> {
    double c;

    WEdge(const Vertex &v, const Vertex &w, double c) : Edge<Vertex>(v, w), c(c) { }
};

// compare wedges
template <class Vertex>
bool operator < (const WEdge<Vertex> &e1, const WEdge<Vertex> &e2) {
    return ((e1.c < e2.c) || (e1.c == e2.c && Edge<Vertex>(e1) < Edge<Vertex>(e2)));
}

// output wedge
template <class Vertex>
std::ostream& operator << (std::ostream &os, const WEdge<Vertex> &e) {
    os << "{" << e.v << ", " << e.w << ", " << e.c << "}";
    return os;
}

#endif // WEDGE_H
