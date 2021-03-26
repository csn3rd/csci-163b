#ifndef FLOWNETWORK_H
#define FLOWNETWORK_H

#include <cassert>
#include <queue>
#include <unordered_map>
#include <limits>

#include "../network/network.h"

template <class T>
class flow : public network<T> {
public:

    // constructor
	flow(const T &source, const T &sink) {
		assert(source != sink);
		_source = source;
		_sink = sink;
		network<T>::addVertex(_source);
		network<T>::addVertex(_sink);
	}

    // add vertex if not already in graph
	void addVertex(const T &v) {
		if (v == _source || v == _sink) return;
		network<T>::addVertex(v);
	}

    // return whether the flow is empty
	bool empty() const {
		return (network<T>::m() == 0);
	}

    // return the total flow produced from source
	double value() const {
		double ans(0.0);
		for (auto &v : network<T>::Adj(_source)) {
			ans += network<T>::cost(_source, v);
		}
		return ans;
	}

    // combine this flow with another flow
	void operator +=(const flow &f) {
		for (auto &e : f.E()) {
			if (network<T>::isEdge(e.v, e.w)) {
				network<T>::setCost(e.v, e.w, network<T>::cost(e) + network<T>(f).cost(e));
			} else {
				network<T>::addEdge(e);
			}
		}
	}

private:
	T _source, _sink;
};

template <class T>
class flownetwork : public network<T> {
public:

    // constructor
	flownetwork(const T &source, const T &sink) {
		assert(source != sink);
		_source = source;
		_sink = sink;
		network<T>::addVertex(_source);
		network<T>::addVertex(_sink);
	}

    // add vertex if not in graph
	void addVertex(const T &v) {
		assert(v != _source && v != _sink);
		network<T>::addVertex(v);
	}

    // return source vertex
	T source() const {
		return _source;
	}

    // return sink vertex
	T sink() const {
		return _sink;
	}

    // return an augmenting flow
	flow<T> augmented_flow() {
		// find a path with fewest edges from source to sink
		std::queue<T> Q;
		std::unordered_map<T, T> parent;

		Q.push(_source);
		parent[_source] = _source;
		while (!Q.empty()) {
			T f = Q.front();
			Q.pop();

			for (auto &w : network<T>::Adj(f)) {
				if (parent.count(w) == 0) {
					Q.push(w);
					parent[w] = f;
					if (w == _sink) {
						break;
					}
				}
			}
		}

		if (parent.count(_sink) == 0) {				// no more augmenting path
			return flow<T>(_source, _sink);			// return empty flow
		}

		// find minimum capacity on the path from source to sink
		double w(std::numeric_limits<double>::infinity());
		for (T v = _sink; v != _source; v = parent.at(v)) {
			w = std::min(w, network<T>::cost(parent.at(v), v));
		}

		// create a flow on this path with flow value w
		flow<T> ans(_source, _sink);
		for (auto &v : network<T>::V()) {
			ans.addVertex(v);
		}

		for (T v = _sink; v != _source; v = parent.at(v)) {
			ans.addEdge(parent.at(v), v, w);
		}

		// update this flow
		for (T v = _sink; v != _source; v = parent.at(v)) {
			T p = parent.at(v);
			double newcost = network<T>::cost(p, v) - w;
			if (newcost == 0.0) {
				network<T>::removeEdge(p, v);
			} else {
				network<T>::setCost(p, v, newcost);
			}

			if (network<T>::isEdge(v, p)) {
				network<T>::setCost(v, p, network<T>::cost(p,v)+w);
			} else {
				network<T>::addEdge(v, p, w);
			}
		}

		return ans;
	}

    // return the max flow using Edmonds-Karp algorithm (Ford-Fulkerson algorithm)
	flow<T> max_flow() const {
		flownetwork<T> residual(*this);
		flow<T> ans(_source, _sink);

		for (auto &v : network<T>::V()) {
			ans.addVertex(v);
		}

		flow<T> f = ans;
		do {
			f = residual.augmented_flow();
			ans += f;
		} while (!f.empty());

		return ans;
	}

private:
	T _source, _sink;
};

// input flownetwork
template <class T>
std::istream& operator >> (std::istream &is, flownetwork<T> &F) {
	std::size_t n, m;
	T v, w;
	double c;

	is >> n >> m;
	assert(n >= 2);

	is >> v >> w;
	flownetwork<T> ans(v, w);
	for (std::size_t i = 2; i < n; i++) {
		is >> v;
		ans.addVertex(v);
	}

	for (std::size_t i = 0; i < m; i++) {
		is >> v >> w >> c;
		ans.addEdge(v, w, c);
	}

	F = ans;

	return is;
}

// output flownetwork
template <class Vertex>
std::ostream& operator << (std::ostream &os, const flownetwork<Vertex> &N) {
	os << "\nFlow Netowrk:" << std::endl;
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

#endif // FLOWNETWORK_H
