//
// Created by Lucius Schoenbaum on 1/16/22.
//

#pragma once

#include <Tbasis.h>

#include <green/tree.h>
#include <green/token.h>

class TGreenTree {

    // Let's walk you through this.

    // Define a size type, "Oid_T", depending on the scale you want.
    // Now let GreenToken be introduced.

    using Oid_T = size_t;
    using GreenToken = Green::Token<Oid_T>;

    // Now define your token type, whatever you want.

    struct MyToken: public GreenToken {
        int value;
        string str() const override {
            string out = GreenToken::str() + "{v: " + std::to_string(value) + "}";
            return out;
        }
    };

    // Now let GreenTree be introduced.
    // You can let the logsize be the default if you wish,
    // a quarter of sizeof(Oid_T) which is 64/4 = 16, so you have 2^16 tokens to play with.

    using GreenTree = Green::Tree<MyToken, Oid_T>;

    // Now you can use your GreenTree.

public: // API:

    static void Tdryrun() {
        GreenTree tree;
        size_t cnt = 4;
        cout << tree.str() << endl;
        for (int i = 1; i <= cnt; ++i) {
            cout << "\nREV #" << std::to_string(i) << endl;
            Oid_T idx = tree.buyNew();
            tree[idx].value = i + 10;
            tree.unbuy(idx);
            cout << "token = " << tree[idx].str() << endl;
            cout << tree.str() << endl;
        }
        // The plain GreenTree tokens are not linked in any way.
        // We select one to delete in a non-realistic way,
        // to test out the deleting.
        cnt = cnt/2;
        tree.buy(cnt);
        tree.unbuyDelete(cnt);
        cout << "\nafter deleting:\n" << tree.str() << endl;
    }

    static void Tsize_calculation() {
        cout << "normal:\n";
        cout << "8: " << static_cast<uint16_t>(Green::Tree<MyToken, uint8_t>::size) << endl;
        cout << "16: " << Green::Tree<MyToken, unsigned short>::size << endl;
        cout << "32: " << Green::Tree<MyToken, unsigned int>::size << endl;
        cout << "64: " << Green::Tree<MyToken, unsigned long int>::size << endl;
        cout << "small:\n";
        cout << "8: " << static_cast<uint16_t>(Green::Tree<MyToken, uint8_t, Green::Allocation::small>::size) << endl;
        cout << "16: " << Green::Tree<MyToken, unsigned short, Green::Allocation::small>::size << endl;
        cout << "32: " << Green::Tree<MyToken, unsigned int, Green::Allocation::small>::size << endl;
        cout << "64: " << Green::Tree<MyToken, unsigned long int, Green::Allocation::small>::size << endl;
        cout << "large:\n";
        cout << "8: " << static_cast<uint16_t>(Green::Tree<MyToken, uint8_t, Green::Allocation::large>::size) << endl;
        cout << "16: " << Green::Tree<MyToken, unsigned short, Green::Allocation::large>::size << endl;
        cout << "32: " << Green::Tree<MyToken, unsigned int, Green::Allocation::large>::size << endl;
        cout << "64: " << Green::Tree<MyToken, unsigned long int, Green::Allocation::large>::size << endl;
        cout << "huge:\n";
        cout << "8: " << static_cast<uint16_t>(Green::Tree<MyToken, uint8_t, Green::Allocation::huge>::size) << endl;
        cout << "16: " << Green::Tree<MyToken, unsigned short, Green::Allocation::huge>::size << endl;
        cout << "32: " << Green::Tree<MyToken, unsigned int, Green::Allocation::huge>::size << endl;
        cout << "64: (does not compile)" << endl;
    }

};


