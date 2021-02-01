#ifndef DFS_H
#define DFS_H

#include <unordered_map>
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
  }

  void dfs_one(const digraph<Vertex> &D, const Vertex &v) {
    _pre[v] = ++_time;
    for (auto &w : D.adj(v)) {
      if (_pre.count(w) == 0) {
        _P[w] = v;
        dfs_one(D, w);
      }
    }
    _post[v] = ++_time;
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

  // 0: tree 1: back 2: forward 3: cross
  int edge_type(const Vertex &a, const Vertex &b) const {
    assert(_pre.count(a) && _pre.count(b));
    if (_P.at(b) == a) {
      return 0;
    }
    if (_pre.at(a) < _pre.at(b)) {
      return 2;
    }
    if (_post.at(a) < _post.at(b)) {
      return 1;
    }
    return 3;
  }

private:
  std::unordered_map<Vertex, std::size_t> _pre, _post;
  std::unordered_map<Vertex, Vertex> _P;
  std::size_t _time;
};

#endif // DFS_H
