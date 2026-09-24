//
// Created by Lucius Schoenbaum on 3/29/22.
//

#pragma once


#include <Tbasis.h>

#include <kabbage/sort/quicksort.h>


class Tquicksort {

public: // API:

    static void Tdryrun() {
        const std::size_t sz = 5;
        int arr[sz] = {5, 3, 2, 4, 1};
        for (int i = 0; i < sz; ++i) {
            cout << arr[i] << " ";
        }
        cout << endl;
        QuickSort qs(arr, 5);
        cout << "# calls: " << qs.calls << endl;
        cout << "# comparisons: " << qs.comparisons << endl;
        for (int i = 0; i < sz; ++i) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

};