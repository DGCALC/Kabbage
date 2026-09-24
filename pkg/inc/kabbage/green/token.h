//
// Created by Lucius Schoenbaum on 1/21/22.
//

#pragma once

#include <types/DGCALC.h>

namespace Green {
// Base type for GreenTree Token types
template <typename Oid_T>
struct Token {
public:

    // pointer to negative trace (or zero if positive Token)
    Oid_T trace;
    // whether owned (!0), or not (0)
    Oid_T own;

public: // ctor:

    Token():
            trace(0),
            own(0)
    {}

public: // API:

    virtual string str() const {
        string out = "[" + std::to_string(own);
        if (trace != 0) out += ", t:" + std::to_string(trace);
        out += "]";
        return out;
    }

};
}

