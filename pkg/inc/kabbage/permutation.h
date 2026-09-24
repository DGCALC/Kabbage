//
// Created by Lucius Traylor Schoenbaum on 2/3/21.
//

/*
 * permutationofsize<size>
 *
 * Class which implements a permutation on size letters,
 * or an element of the group S_size.
 * When using, should use a typedef or using clause, as in
 * using permutation = permutationofsize<10>;
 *
 * The class assumes that the permutation permutes the values
 * {0, 1, 2, ..., size-1}.
 * If you assume a different set is being permuted,
 * such as {1, 2, ..., size} which is a common base set,
 * there is no guarantee of anything.
 * The base set is chosen for convenience when using permutation
 * to permute components of the componentsurface class.
 *
 * Supports operations:
 * - pull out a random permutation of given size.
 * - create a permutation from a list like 5, 3, 2, 0, 4, 1.
 * - check that it is a valid permutation.
 * - to disable this check. todo
 *
 * The input string is:
 * 1, 2, 3
 * >>>> implement as Args...<int> and check that the number of args = size
 * or cyclic notatation:
 * (1, 3)(4, 5)(6, 3)
 * >>>> implement as string and use parsing, assume base ten.
 * or
 * (4, 2, 3)(6, 5)(4, 2, 3, 1)
 * etc.
 *
 */

#pragma once

#include <cassert>
#include <string>
#include <utt/string>

template <int size>
class permutationofsize {

    int data[size];

public: // ctor:

    // identity
    explicit permutationofsize() {
        for (int i = 0; i < size; ++i) {
            data[i] = i;
        }
    }

    // constructor, given an explicit permutation
    explicit permutationofsize(std::initializer_list<int> lst, bool check_validity = true) {
        int i = 0;
        for (auto j: lst) {
            data[i++] = j;
        }
        if (check_validity) {
            if (lst.size() != size) {
                assert(0==1);
            }
            check_for_duplication();
        }
    }

    // The input must satisfy the CF-grammar
    // (all whitespace ignored)
    // int > ...an integer in base ten...
    // cycle > int
    // cycle > int, cycle
    // closed_cycle > (int)
    // permutation > closed_cycle
    // permutation > closed_cycle permutation
    // The interpretation is as a composition of cyclic permutations
    // on the set 0, 1, ..., N-1 where N is the size,
    // which proceeds from right (first to be evaluated)
    // to left (last to be evaluated).
    // For example, (0)(1, 2)(2, 3) takes {0, 1, 2, 3} to {0, 2, 3, 1}.
    // This is slightly unnatural from a certain point of view,
    // but it allows composability as functions in Euler function notation,
    // using operator*.
    explicit permutationofsize(const std::string & input, bool check_validity = true) {
        utt::string rawinput;
        rawinput.throwout(")(");
        rawinput.aim(input);
        utt::string cycle;
        cycle.throwout(',');
        // initialize data (initially in order)
        for (int i = 0; i < size; ++i) {
            data[i] = i;
        }
        int tmp, tmpindex;
        int tmp2, tmp2index;
        int orig, origindex;
        bool firstvalue;
        while (!rawinput.empty()) {
            cycle.aim(rawinput.pop_between_marks("(", ")"));
            firstvalue = true;
            while (!cycle.empty()) {
                if (firstvalue) {
                    tmp = atoi(cycle.pop().c_str());
                    orig = tmp;
                    origindex = index(orig);
                    firstvalue = false;
                }
                else {
                    tmp = tmp2;
                }
                tmpindex = index(tmp);
                if (!cycle.empty()) {
                    //\\ tmp is not the last in the cycle.
                    tmp2 = atoi(cycle.pop().c_str());
                    tmp2index = index(tmp2);
                    // swap tmp and tmp2
                    data[tmpindex] = tmp2;
                    data[tmp2index] = tmp;
                }
                else {
                    //\\ tmp is the last in the cycle.
                    // switch orig and tmp
                    data[origindex] = tmp;
                    data[tmpindex] = orig;
                    break;
                }
            }
        }
    }

public: // copy:

    permutationofsize(const permutationofsize & copyin) {
        for (int i = 0; i < size; ++i) {
            this->data[i] = copyin.data[i];
        }
    }

    permutationofsize & operator=(const permutationofsize & rhs) {
        if (this == &rhs) return *this;
        for (int i = 0; i < size; ++i) {
            this->data[i] = rhs.data[i];
        }
        return *this;
    }

public: // dtor:
    ~permutationofsize() = default;
private: // methods:

    // This method finds the index the slow-and-stupid way.
    // For a large permutation, you will need a hash table,
    // but there's a crossing point below which you definitely don't.
    // Also, this method is used only (check???) in one of the ctors
    // and in the function to find the inverse -
    // both of these are in a sense one-time costs,
    // so as long as you are not creating a permutation per cycle,
    // you should be fine with this. (But you can decide!)
    // There is no checking since it is assumed that the
    // validity of the permutation was checked at construction,
    // or else the user made the choice not to check for well-formedness.
    int index(int value) const {
        for (int i = 0; i < size; ++i) {
            if (data[i] == value) return i;
        }
        // If this occurs, something is wrong.
        return 0;
    }

    // check for duplications (ill-formed permutation)
    void check_for_duplication() const {
        int val;
        for (int i = 0; i < size; ++i) {
            val = data[i];
            for (int j = i+1; j < size; ++j) {
                if (data[j] == val) assert(1==0);
            }
        }
    }

    permutationofsize inverse() const {
        permutationofsize out;
        for (int i = 0; i < size; ++i) {
            out.data[i] = index(i);
        }
        return out;
    }

public: // API:

    // p(5) returns what p takes 5 to.
    int & operator()(int index) {
        return data[index];
    }

    const int & operator()(int index) const {
        return data[index];
    }

    // p^(-1) finds the inverse,
    // p^n for n ≥ 1 composes with itself,
    // p^0 returns the identity.
    friend permutationofsize operator^(const permutationofsize & ain, const int pow) {
        if (pow < 0) {
            if (pow < -1) assert (0 == 1);
            return ain.inverse();
        }
        else {
            if (pow == 0) return permutationofsize();
            permutationofsize out = ain;
            for (int i = 1; i < pow; ++i) {
                out = out * ain;
            }
            return out;
        }
    }

    friend permutationofsize operator*(const permutationofsize & lhs, const permutationofsize & rhs) {
        permutationofsize out = rhs;
        // This is easier than you think.
        for (int i = 0; i < size; ++i) {
            out.data[i] = lhs.data[rhs.data[i]];
        }
        return out;
    }

//    // random permutation
//    void random() {
//        // todo...This is not so easy as you think.
//    }

    // simple is easiest for small permutations
    std::string str_element(int i) const {
        return std::to_string(data[i]);
    }

    // for larger permutations, the simpler version is not helpful
    std::string str_index_element(int i) const {
        return std::to_string(i) + " → " + std::to_string(data[i]);
    }

    std::string str(bool show_indices = false) const {
        std::string out;
        if (show_indices) {
            for (int i = 0; i < size; ++i) {
                out += "[" + str_index_element(i) + "]\n";
            }
        }
        else {
            out = "[";
            for (int i = 0; i < size - 1; ++i) {
                out += str_element(i) + ", ";
            }
            out += str_element(size - 1) + "]";
        }
        return out;
    }

};

