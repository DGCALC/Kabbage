//
// Created by Lucius Schoenbaum on 9/2/21.
//

// Edit this list by adding arguments
// like argv for command line control of Catch2.
/*
 * Example:
 * $ foo -?
 * Corresponds to
 * const char * catch_argv[]{"path_of_executable", "-?", nullptr};
 *
 */
const char * catch_argv[]{
        "path_of_executable",
        "-?",
        nullptr
};

#include <catch2/catch.hpp>

#include <Tsort/Tquicksort.h>

