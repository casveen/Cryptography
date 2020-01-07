#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <iostream>
#include <vector>
using namespace std;

typedef unsigned int uint;

uint factorial(uint in) {
    // unsafe and ineffective, made for simple use
    return (in == 1 || in == 0) ? 1 : in * factorial(in - 1);
}

uint binomial(uint n, uint m) {
    // very unefficient and unsafe, use only for small n and m
    return factorial(m) / (factorial(n) * factorial(m - n));
}

template<class T>
void choices_of_inner(vector<vector<T>> &write_to, vector<T> choose_from, int at, int from,
                      int choices_num) {
    uint step_size=
        factorial(choose_from.size() - 1) / factorial(choose_from.size() + at - choices_num);
    for (uint i= 0; i < choose_from.size(); ++i) {
        for (uint j= 0; j < step_size; ++j) {
            write_to[from + step_size * i + j][at]= choose_from[i];
        }
        if (at < choices_num - 1) {
            vector<T> new_choose_from= vector<T>(choose_from);
            new_choose_from.erase(new_choose_from.begin() + i);   // ineffficient
            choices_of_inner(write_to, new_choose_from, at + 1, from + i * step_size, choices_num);
        }
    }
}

template<class T>
vector<vector<T>> choices_of(vector<T> choose_from, int choices_num) {
    // given a vector of elements, return a vector of all possible choices of size
    // choices from the given set
    // assumes choose from contains all-unique elements.

    // init vec vec by copying to_choose onto all
    vector<vector<T>> choices;
    vector<T>         choose_from_truncated= vector<T>(choose_from);
    choose_from_truncated.erase(choose_from_truncated.begin() + choices_num,
                                choose_from_truncated.end());
    for (uint i= 0; i < factorial(choose_from.size()) / factorial(choose_from.size() - choices_num);
         ++i) {
        choices.push_back(vector<T>(choose_from_truncated));
    }
    // run recursive algorithm
    choices_of_inner(choices, vector<T>(choose_from), 0, 0, choices_num);
    return choices;
}
#endif
