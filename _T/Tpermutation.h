//
// Created by Lucius Traylor Schoenbaum on 2/4/21.
//

#pragma once

#include <Tbasis.h>

#include <usci/object/permutation.h>

#define size 5
using permutation = permutationofsize<size>;

class Tpermutation {

    static int num_fail;
    static int num_tests;

    static void check(const permutation & p) {
        cout << "checking permutation...\n";
        cout << "p = " << p.str() << "\n";
        cout << "p*p = " << (p*p).str() << "\n";
        cout << "p^-1 = " << (p^(-1)).str() << "\n";
//        if (back != f) {
//            cout << "BAD\n";
//            ++num_fail;
//        }
//        ++num_tests;
//        cout << "\n\n";
    };

public: // API:

    static void Tdryrun() {
        permutation p{4, 3, 2, 1, 0};
        check(p);
        permutation q("(0)(3, 2)(2, 3)");
        check(q);
        permutation pp{1, 2, 3, 4, 0};
        check(pp);
        permutation qq("(0)(1, 2)(2, 3)(4)");
        check(qq);
    }


};

int Tpermutation::num_fail = 0;
int Tpermutation::num_tests = 0;

#undef size