//
// Created by Lucius Schoenbaum on 3/29/22.
//

/*
 * Sort-in-place an array T* using mergesort.
 *
 */

#pragma once

template <typename T /* operator< */>
class MergeSort {

private: // fields:

    T * arr;

public: // fields:

    std::size_t calls;
    std::size_t comparisons;

public: // ctor:

    MergeSort(T * arr, std::size_t size):
        arr(arr),
        calls(0),
        comparisons(0)
    {
        rec(0, size-1);
    }

private: // methods:

// Below, is the quicksort code (copy-pasted). Idea is to keep the code similar between all these guys. ...Oh, and the API as well.

    // void rec(int left, int right) {
    //     ++calls;
    //     ++comparisons;
    //     if (left < right) {
    //         T pivot = arr[right];
    //         int partitionPoint = partition(left, right, pivot);
    //         rec(left, partitionPoint-1);
    //         rec(partitionPoint+1, right);
    //     }
    // }
    //
    // int partition(int left, int right, T pivot) {
    //     int leftPointer = left - 1;
    //     int rightPointer = right;
    //     while(true) {
    //         while(arr[++leftPointer] < pivot) ++comparisons;
    //         while(rightPointer > 0 && arr[--rightPointer] > pivot) comparisons += 2;
    //         if (leftPointer >= rightPointer) {
    //             comparisons++;
    //             break;
    //         }
    //         else
    //             swap(leftPointer, rightPointer);
    //     }
    //     swap(leftPointer, right);
    //     return leftPointer;
    // }
    //
    // void swap(std::size_t idx1, std::size_t idx2) {
    //     T tmp = arr[idx1];
    //     arr[idx1] = arr[idx2];
    //     arr[idx2] = tmp;
    // }

};



