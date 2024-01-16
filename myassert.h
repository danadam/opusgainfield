#ifndef DANADAM_MYASSERT__GUARD_H
#define DANADAM_MYASSERT__GUARD_H

#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>

#include <cstdio>

namespace boost {

[[noreturn]] inline void
assertion_failed(char const * expr, char const * function, char const * file, long line) {
    fprintf(stderr, "%s:%ld: %s: BOOST_ASSERT(%s) failed.\n", file, line, function, expr);
    std::exit(EXIT_FAILURE);
}

[[noreturn]] inline void
assertion_failed_msg(char const * expr, char const * msg, char const * function, char const * file, long line) {
    fprintf(stderr, "%s:%ld: %s: BOOST_ASSERT_MSG(%s) failed: %s.\n", file, line, function, expr, msg);
    std::exit(EXIT_FAILURE);
}

} // namespace

#endif
