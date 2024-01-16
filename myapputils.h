#ifndef DANADAM_MYAPPUTILS_GUARD_H
#define DANADAM_MYAPPUTILS_GUARD_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <string_view>

#include "myassert.h"

inline
std::string
progname(std::string_view argv0) {
    auto file_pos = argv0.find_last_of('/');
    file_pos++;     // if it was npos then now it is 0
    BOOST_ASSERT(file_pos < argv0.size());
    return std::string{ argv0.substr(file_pos) };
}

[[noreturn]] inline void
die(std::string_view argv0, std::string_view msg) {
    fprintf(stderr, "%s: %.*s\n", progname(argv0).c_str(), static_cast<int>(msg.size()), msg.data());
    std::exit(EXIT_FAILURE);
}

template<typename F>
[[noreturn]] void
die_with_help(std::string_view argv0, std::string_view msg, F&& help_cb) {
    fprintf(stderr, "%s: %.*s\n\n", progname(argv0).c_str(), static_cast<int>(msg.size()), msg.data());
    help_cb();
    std::exit(EXIT_FAILURE);
}

#endif
