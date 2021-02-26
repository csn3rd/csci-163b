#ifndef DARY_HEAP_H
#define DARY_HEAP_H

#include <vector>
#include <unordered_map>
#include <cassert>


template <class T>
class dary_heap {
public:
	dary_heap(std::size_t d = 2) : _d(d) {
		_n = 0;
	}

	bool empty() const {
		return _n == 0;
	}

	std::size_t size() const {
		return _n;
	}

	T min() const {
		assert(!empty());

		return _data[0];
	}

	void push(const T &x) {
		aseert(_l.count(x) == 0);

		if (_n == _data.size()) {
			_data.pushback(x);
		} else {
			_data[_n] = x;
		}

		_l[x] = _n;
		_n++;

		std::size_t i = _l[x];
		while (i > 0) {
			std::size_t parent = (i-1)/_d;
			if (_data[i] < _data[parent]) {
				std::swap(_data[i], _data[parent]);
				_l[_data[i]] = i;
				_l[_data[parent]] = parent;
				i = parent;
			} else {
				break;
			}
		}
	}

	void decreaseKey(const T &x, const T &newX) {
		assert(newX < x && _l.count(x) != 0 && _l.count(newX) == 0);

		std::size_t i = _l[x];
		_data[i] = newX;
		_l.erase(x);
		_l[newX] = i;

		while (i > 0 && _data[i] < _data[(i-1)/_d]) {
			std::size_t parent = (i-1)/_d;
			std::swap(_data[i], _data[parent]);
			_l[_data[i]] = i;
			_l[_data[parent]] = parent;
			i = parent;
		}
	}

	void pop_min() {
		assert(!empty());

		_l.erase(_data[0]);
		_n--;

		if (_n == 0) { return; }

		_data[0] = _data[_n];
		_l[_data[0]] = 0;
		std::size_t i = 0;

		while (i*_d+1 < _n) {
			std::size_t left = (i*_d+1);
			std::size_t right = (i*_d+_d);
			std::size_t m = left;

			for (std::size_t c = left+1; c < _n && c <= right; c++) {
				if (_data[c] < _data[m]) {
					m = c;
				}
			}

			if (_data[m] >= _data[i]) {
				break;
			}

			std::swap(_data[i], _data[m]);
			_l[_data[i]] = i;
			_l[_data[m]] = m;
			i = m;
		}
	}

private:
	std::vector<T> _data;                   // heap elements
	std::size_t _n;                         // number of elements
	std::size_t _d;                         // max children per node
	std::unordered_map<T, std::size_t> _l;  // _data[_l[key]] = key;

};

#endif // DARY_HEAP_H
