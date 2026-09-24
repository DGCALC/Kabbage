//
// Created by Lucius Schoenbaum on 1/15/22.
//

/*
 * GreenTree
 *
 * Unopinionated threadsafe recycling tree/graph data structure.
 *
 *
 */

/*
 * Size is hard-coded, but varies with choice of Oid_T:
 * 2^(  7 + 2*ilog2(sizeof(Oid_T))  )
 * where |Oid_T| is the width in bits.
 *
 * Examples:
 * Oid_T = char ---> size = 128
 * Oid_T = 16bits ---> size = 512
 * Oid_T = 32bits ---> size = 2048
 * Oid_T = 64bits ---> size = 8192
 *
 * For special applications, this can be changed in order to
 * max out the address space, with a size given by:
 * 2^(sizeof(Oid_T)*8) - 1
 * This tree can be created with the "huge" template parameter,
 * as in:
 * Green::Tree<T, Oid_T, true> mytree;
 *
 */

#pragma once


#include <types/DGCALC.h>
#include <green/types.h>

#include <mutex>
#include <atomic>

namespace Green {
template <typename T, typename Oid_T, Allocation alloc = normal>
class Tree {

private: // types:

    using aOid_T = std::atomic<Oid_T>;

public: // fields:

    static const Oid_T size =
            (alloc == huge)?
            static_cast<Oid_T>(-1)
            :(alloc == normal)?
            DGCALC::ipow2<Oid_T>(7 + 2*DGCALC::ilog2<Oid_T>(sizeof(Oid_T)))
            :(alloc == large)?
            DGCALC::ipow2<Oid_T>(7 + 3*DGCALC::ilog2<Oid_T>(sizeof(Oid_T)))
            : /* small */
            DGCALC::ipow2<Oid_T>(7 + DGCALC::ilog2<Oid_T>(sizeof(Oid_T)));
    static const constexpr Oid_T npos = -1;

protected: // data:

    // data: were once O's, still called Os to go with Oid etc.
    T Os[size];
    //@ whether the class is currently being used
    //+ in a multi-threaded environment
    std::atomic<bool> multi;

private: // data

    //@ The top of the oid trace: at time t, points to next_oid(t)
    aOid_T vvoid;
    //@ the number of occupied Oids
    Oid_T num_oids_;
    //@ a mutex to manage Os data
    std::mutex mtx;

public: // ctor:

    Tree() {
        // set the Point At Zero, Oext (O-external)
        Os[0].trace = 0;
        // create the negative trace, the trace of all free indices.
        for (auto i = 1; i < size; ++i) {
            //@ sic: start with 1; trace of 0 is 0
            Os[i].trace = i + 1;
        }
        // sic: trace of last is -1
        Os[size - 1].trace = -1;
        // set pointer to top of negative trace
        vvoid = 1;
        // start the oid counter
        num_oids_ = 1;
        // multiprocessing: must be set via call to multi_start();
        multi = false;
    }

public: // Operations API:

    //@ the number of Oids currently being used
    //) The count includes Point at Zero, Oext.
    Oid_T num_oids() const {
        return num_oids_;
    }

    //@ start multi-threaded operations (guards up)
    bool multi_start() {
        multi = true;
    }

    //@ stop multi-threaded operations (guards down)
    bool multi_stop() {
        multi = false;
    }


    void mutexup() {
        if (multi) {
            mtx.lock();
        }
    }

    void mutexdown() {
        if (multi) {
            mtx.unlock();
        }
    }

    // Bracket notation.
    // When multi-processing,
    // you must only use brackets with bought indices.
    // Otherwise, behavior is undefined.
    // However, when running a single-threaded process,
    // it is fine to use getIdx and use these however desired.
    T & operator[](Oid_T oid) {
        return Os[oid];
    }
    const T & operator[](Oid_T oid) const {
        return Os[oid];
    }

    //@ buy a token
    // Once bought, the token cannot be moved
    // by any operation. Thus,
    // this routine will return the index to the user.
    // It should be understood that this index should be
    // handled *quickly*, so the tree can more easily
    // "breathe", so to say.
    // Possible actions: deplace the token, modify the token,
    // buy two tokens and swap them, etc.
    // Return 0 on success.
    // Return npos on failure.
    Oid_T buy(Oid_T oid) {
        Oid_T out = 0;
        mutexup();
        if (Os[oid].own == 1) out = npos;
        else Os[oid].own = 1;
        mutexdown();
        return out;
    }

    //@ unbuy a bought token
    //) You must unbuy or else delete whatever is bought.
    // Return 0 on success.
    // Return npos on error.
    Oid_T unbuy(Oid_T oid) {
        Oid_T out = 0;
        mutexup();
        if (Os[oid].own == 0) out = npos;
        else
            Os[oid].own = 0;
        mutexdown();
        return out;
    }

    //@ buy new T
    //) i.e. create new token and have it delivered to you.
    //+ You must unbuy it when you are finished with
    //+ any updates you wish to make.
    // Return token Oid_T != 0, != npos on success.
    // return npos on failure.
    Oid_T buyNew() {
        if (vvoid == npos) {
            // No new index available: at capacity.
            // todo error log
            // signal application.
            return npos;
        }
        //\\ vvoid != 0.
        Oid_T tmp = vvoid;
        mutexup();
        vvoid = Os[vvoid].trace;
        Os[tmp].own = 1;
        Os[tmp].trace = 0;
        ++num_oids_;
        mutexdown();
        return tmp;
    }

    //@ deplace token
    //) delete it and recycle the Oid space resource.
    // NOTE: to execute, you must buy the Oid.
    // If you execute without first buying, behavior is undefined.
    // Return 0 on success.
    // Return npos on failure.
    Oid_T unbuyDelete(Oid_T oid) {
        if (oid == 0)
            //\\ incompatible request: token at zero is fixed.
            return npos;
        //\\ oid != 0.
        Oid_T out = 0;
        mutexup();
        if (Os[oid].own != 1)
            //\\ incompatible request: token is not bought.
            out = npos;
        else {
            //@ push oid onto the negative trace.
            //) Note oid will be the next to be used!
            //+ So no persistent cracks appear.
            Os[oid].trace = vvoid;
            vvoid = oid;
            --num_oids_;
        }
        mutexdown();
        return out;
    }

    //@ string, for debugging purposes
    std::string str() const {
        std::string out;
        Oid_T tmp, cnt;
        out += "{size:: " + std::to_string(size) + "\ntokens:: ";
        out += "0: " + Os[0].str();
        for (auto i = 1; i < size; ++i) {
            if (Os[i].trace == 0) out += ", " + std::to_string(i) + ": " + Os[i].str();
        }
        out += "\nnum_oids:: " + std::to_string(num_oids());
        out += "\ntrace:: ";
        // look at current oid, to follow the negative trace
        tmp = vvoid;
        out += std::to_string(tmp);
        cnt = 0;
        // print head of the trace
        while (tmp != -1) {
            out += ", " + std::to_string(Os[tmp].trace);
            tmp = Os[tmp].trace;
            if (++cnt == 6) break;
        }
        out += ", ... ";
        // print tail of the trace
        tmp = size - 4;
        while (tmp != -1) {
            out += ", " + std::to_string(Os[tmp].trace);
            tmp = Os[tmp].trace;
        }
        return out + "}";
    }

};
}



