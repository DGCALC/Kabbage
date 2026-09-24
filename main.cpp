//
// Created by Lucius Traylor Schoenbaum on 8/14/20.
//

#define CATCH_CONFIG_RUNNER

#include <T.h>

int main() {

//    Tfixstring::Tdryrun();
//    Tfixstring::Tmaxcutoff();

//    TfixLUT::Tdryrun();
//    TfixLUT::Tsize20LUT();
//    TfixLUTsearch foo; foo.Tdryrun();

    Tquicksort::Tdryrun();

    int catch_argc = 0;
    while (catch_argv[++catch_argc] != nullptr);
//    Catch::Session().run(catch_argc, catch_argv);

}


