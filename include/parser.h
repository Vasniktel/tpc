//
// Created by vasniktel on 15.04.19.
//

#ifndef TPC__PARSER_H
#define TPC__PARSER_H

#include "scanner.h"
#define div divide

typedef enum Op {
  imm, arg, plus, min, mul, div
} Op;

typedef struct AST {
  Op op;
  struct AST* a, *b;
  int n;
} AST;

AST* ast_arg(int n);
AST* ast_imm(int n);
AST* ast_bin(Op op, AST* a, AST* b);

void ast_free(AST* ast);
void asts_free(AST* tree);

void asts_print(const AST* tree);

AST* parse(const Token* tokens);

#endif //TPC__PARSER_H
