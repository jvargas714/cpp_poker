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

class cyclic_index_buff {
public:
    cyclic_index_buff() { index = 0; }

    cyclic_index_buff(int sz) {
        for (int i = 0; i < sz; i++) {
            cyc_vect.push_back(i);
        }
        index = 0;
    }

    int next() {
        if (index == cyc_vect.size() - 1) {
            index = 0;
            return cyc_vect[cyc_vect.size() - 1];
        } else {
            return cyc_vect[index++];
        }
    }

    // removes element at index i
    void remove(int i) {
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

    void show_contents() {
        for (int &i : cyc_vect) {
            std::cout << i << std::endl;
        }
    }

private:
    std::vector<int> cyc_vect;
    int index;
};

#endif /* HEADERS_CYCLIC_BUFF_H_ */
