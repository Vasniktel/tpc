#include <stdio.h>
#include <stdlib.h>

#include "../include/tpc.h"
#include "../include/scanner.h"
#include "../include/parser.h"

/*  Grammar:

    function   ::= '[' arg-list ']' expression

    arg-list   ::= / nothing /
                | variable arg-list

    expression ::= term
                | expression '+' term
                | expression '-' term

    term       ::= factor
                | term '*' factor
                | term '/' factor

    factor     ::= number
                | variable
                | '(' expression ')'

    Assembly (two registers):
    "IM n"     // load the constant value n into R0
    "AR n"     // load the n-th input argument into R0
    "SW"       // swap R0 and R1
    "PU"       // push R0 onto the stack
    "PO"       // pop the top value off of the stack into R0
    "AD"       // add R1 to R0 and put the result in R0
    "SU"       // subtract R1 from R0 and put the result in R0
    "MU"       // multiply R0 by R1 and put the result in R0
    "DI"       // divide R0 by R1 and put the result in R0
 */

int main() {
  char* assembly = compile("[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)");
  printf("%s", assembly);
  free(assembly);
  return 0;
}
