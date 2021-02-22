#ifndef DS_H
#define DS_H

#include <unordered_map>

template <class T>
struct node {
    T key;
    std::size_t rank;
    node<T> *parent;

    node(T x) {
        key = x;
        rank = 0;
        parent = this;
    }
};

template <class T>
class ds {
public:

    ds() { }

    ~ds() {
        for (auto &d : _data) {
            delete d.second;
        }
    }

    void make_set(const T &x) {
        assert(_data.count(x) == 0);
        _data[x] = new node<T>(x);
    }

    node<T>* find_set(const T &x) {
        assert(_data.count(x) != 0);
        if (_data[x] == _data[x]->parent) {
            return _data[x];
        }
        return find_set(_data[x]->parent->key);
    }

    bool join_sets(const T &x, const T &y) {
        node<T> *rx, *ry;
        rx = find_set(x);
        ry = find_set(y);

        if (rx == ry) { return false; }

        if (rx->rank < ry->rank) {
            rx->parent = ry;
        } else if (rx->rank > ry->rank) {
            ry->parent = rx;
        } else {
            ry->parent = rx;
            rx->rank++;
        }
        return true;
    }

private:
    std::unordered_map<T, node<T>*> _data; 
};

#endif // DS_H
