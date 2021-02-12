#ifndef TSCC_H
#define TSCC_H

#include <unordered_map>
#include <stack>

template <class Vertex> class digraph;

template <class Vertex>
class TSCC {
public:
	TSCC(const digraph<Vertex> &D) {
        _ncc = _time = 0;
        for (auto &v : D.V()) {
            if (_pre.count(v) == 0) {
                tdfs_one(D, v);
            }
        }
	}

	void tdfs_one(const digraph<Vertex> &D, const Vertex &v) {
        _pre[v] = _low[v] = _time++;
        _S.push(v);
        for (auto &w : D.Adj(v)) {
            if (_pre.count(w) == 0) {
                tdfs_one(D, w);
            }

            _low[v] = std::min(_low[v], _low[w]);
        }

        if (_pre[v] == _low[v]) {
            Vertex top;
            std::size_t inf = D.n();

            do {
                top = _S.top();
                _S.pop();
                _C[top] = _ncc;
                _low[top] = inf;
            } while(top != v);
            _ncc++;
        }
	}

	// pre times for each vertex
	std::unordered_map<Vertex, std::size_t> pre() const {
		return _pre;
	}

	// low times for each vertex
	std::unordered_map<Vertex, std::size_t> low() const {
		return _low;
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
	std::unordered_map<Vertex, std::size_t> _pre, _low, _C;
	std::size_t _time, _ncc;
    std::stack<Vertex> _S;

};

#endif // TSCC_H
