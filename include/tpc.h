//
// Created by vasniktel on 15.04.19.
//

#ifndef TPC__TPC_H
#define TPC__TPC_H

#include "parser.h"

AST* pass1(const char* program);
AST* pass2(AST* ast);
char* pass3(AST* ast);
char* compile(const char* program);

#endif //TPC__TPC_H
