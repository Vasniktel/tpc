//
// Created by vasniktel on 17.04.19.
//

#ifndef TPC__UTIL_H
#define TPC__UTIL_H

#define STR(x) #x
#define FILE_LINE __FILE__":"STR(__LINE__)

#define EXPR_CHECK(expr, then)                                            \
if (expr) {                                                               \
  fprintf(stderr, "Error in expression '"#expr"' in file "FILE_LINE"\n"); \
  then;                                                                   \
}

#define EXPR_CHECK_NE(fn, val, then) \
EXPR_CHECK((fn != val), then)

#define EXPR_CHECK_EQ(fn, val, then) \
EXPR_CHECK((fn == val), then)

#define ALLOC_CHECK(to, fn, then) \
EXPR_CHECK_EQ((to = fn), NULL, then)

#endif //TPC__UTIL_H
