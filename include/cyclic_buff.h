/*
 * cyclic_buff.h
 *
 *  Created on: Jun 21, 2016
 *      Author: jay
 */

#ifndef HEADERS_CYCLIC_BUFF_H_
#define HEADERS_CYCLIC_BUFF_H_

#include <vector>
#include <algorithm>
#include <iostream>

template<typename T>
class cyclic_index_buff {
public:
    cyclic_index_buff() : index(0) {}

    explicit cyclic_index_buff(size_t sz) {
        for (int i = 0; i < sz; i++) {
            cyc_vect.push_back(i);
        }
        index = 0;
    }

    T& next() {
        if (index == cyc_vect.size() - 1) {
            index = 0;
            return cyc_vect[cyc_vect.size() - 1];
        } else {
            return cyc_vect[index++];
        }
    }

    // removes element at index i
    void remove(const size_t& i) {
        if (index == i && index == cyc_vect.size() - 1) {
            index = 0;
        } else if (i < index) {
            index--;
        }
        cyc_vect.erase(cyc_vect.begin() + i);
    }

    void add(int i) {
        cyc_vect.push_back(i);
    }

    void show_contents() const {
        for (const T&el : cyc_vect) {
            std::cout << el << std::endl;
        }
    }

private:
    std::vector<T> cyc_vect;
    uint64_t index;
};

#endif /* HEADERS_CYCLIC_BUFF_H_ */
